"""MainWindow for the texture viewer."""

from __future__ import annotations
import os
import numpy as np

from PyQt5.QtCore import Qt, QSettings
from PyQt5.QtGui import QFont, QKeySequence
from PyQt5.QtWidgets import (
    QMainWindow, QWidget, QSplitter, QDockWidget,
    QVBoxLayout, QHBoxLayout, QGridLayout, QFormLayout,
    QLabel, QPushButton, QComboBox, QSlider, QSpinBox,
    QDoubleSpinBox, QGroupBox, QFileDialog, QAction,
    QStatusBar, QButtonGroup, QSizePolicy, QScrollArea,
    QFrame,
)

from canvas import ImageCanvas
from tone_map import ToneMapper, ToneMode, ChannelMode
import dds as dds_mod


# ---------------------------------------------------------------------------
# EXR loader (reuse logic from hdri-to-cubemap)
# ---------------------------------------------------------------------------
def _load_exr(path: str) -> np.ndarray:
    """Load EXR → float32 (H, W, 4) RGBA."""
    import OpenEXR
    import Imath
    exr = OpenEXR.InputFile(path)
    header = exr.header()
    dw = header['dataWindow']
    w = dw.max.x - dw.min.x + 1
    h = dw.max.y - dw.min.y + 1
    pt = Imath.PixelType(Imath.PixelType.FLOAT)
    chans = list(header['channels'].keys())

    def read(c):
        return np.frombuffer(exr.channel(c, pt), dtype=np.float32).reshape(h, w)

    if all(c in chans for c in ('R', 'G', 'B')):
        r, g, b = read('R'), read('G'), read('B')
        a_raw = read('A') if 'A' in chans else np.ones((h, w), np.float32)
    else:
        available = chans[:3] + [chans[0]] * max(0, 3 - len(chans))
        r, g, b = (read(c) for c in available[:3])
        a_raw = np.ones((h, w), np.float32)

    return np.stack([r, g, b, a_raw], axis=-1)


# ---------------------------------------------------------------------------
# Helpers
# ---------------------------------------------------------------------------
def _spin(min_v, max_v, val=0, width=60) -> QSpinBox:
    s = QSpinBox()
    s.setRange(min_v, max_v)
    s.setValue(val)
    s.setFixedWidth(width)
    return s


def _dspin(min_v, max_v, val, step, dec, width=75) -> QDoubleSpinBox:
    s = QDoubleSpinBox()
    s.setRange(min_v, max_v)
    s.setValue(val)
    s.setSingleStep(step)
    s.setDecimals(dec)
    s.setFixedWidth(width)
    return s


def _label(text: str, mono: bool = False) -> QLabel:
    l = QLabel(text)
    if mono:
        l.setFont(QFont('Monospace', 9))
    return l


def _hline() -> QFrame:
    f = QFrame()
    f.setFrameShape(QFrame.HLine)
    f.setFrameShadow(QFrame.Sunken)
    return f


# ---------------------------------------------------------------------------
# MainWindow
# ---------------------------------------------------------------------------
class MainWindow(QMainWindow):
    def __init__(self, initial_file: str | None = None):
        super().__init__()
        self.setWindowTitle('Texture Viewer')
        self.resize(1280, 800)

        self._dds: dds_mod.DdsFile | None = None
        self._is_exr: bool = False
        self._exr_data: np.ndarray | None = None   # (H, W, 4) float32
        self._tone = ToneMapper()

        self._build_ui()
        self._build_menu()
        self._connect_signals()
        self._apply_dark_theme()

        self._settings = QSettings('garnet', 'tex-viewer')

        if initial_file:
            self._open_file(initial_file)

    # ------------------------------------------------------------------
    # UI construction
    # ------------------------------------------------------------------
    def _build_ui(self):
        self._canvas = ImageCanvas()
        self._panel = self._build_panel()

        splitter = QSplitter(Qt.Horizontal)
        splitter.addWidget(self._canvas)
        splitter.addWidget(self._panel)
        splitter.setStretchFactor(0, 1)
        splitter.setStretchFactor(1, 0)
        splitter.setSizes([900, 300])
        self.setCentralWidget(splitter)

        self._status = QStatusBar()
        self.setStatusBar(self._status)
        self._status_label = QLabel('No file loaded')
        self._status.addWidget(self._status_label, 1)
        self._zoom_label = QLabel('100%')
        self._status.addPermanentWidget(self._zoom_label)

    def _build_panel(self) -> QWidget:
        w = QWidget()
        w.setFixedWidth(290)
        lay = QVBoxLayout(w)
        lay.setContentsMargins(6, 6, 6, 6)
        lay.setSpacing(6)

        # --- Texture info ---
        self._info_group = QGroupBox('Texture Info')
        info_lay = QFormLayout(self._info_group)
        info_lay.setSpacing(3)
        self._lbl_file   = _label('—')
        self._lbl_dims   = _label('—')
        self._lbl_fmt    = _label('—')
        self._lbl_type   = _label('—')
        self._lbl_size   = _label('—')
        info_lay.addRow('File:',   self._lbl_file)
        info_lay.addRow('Dims:',   self._lbl_dims)
        info_lay.addRow('Format:', self._lbl_fmt)
        info_lay.addRow('Type:',   self._lbl_type)
        info_lay.addRow('Size:',   self._lbl_size)
        lay.addWidget(self._info_group)

        # --- Navigation ---
        nav_group = QGroupBox('Navigation')
        nav_lay = QFormLayout(nav_group)
        nav_lay.setSpacing(4)

        self._spin_mip   = _spin(0, 0)
        self._spin_face  = _spin(0, 0)
        self._spin_depth = _spin(0, 0)
        self._lbl_face_name = QLabel('')

        face_row = QHBoxLayout()
        face_row.addWidget(self._spin_face)
        face_row.addWidget(self._lbl_face_name)
        face_row.addStretch()

        nav_lay.addRow('Mip:', self._spin_mip)
        nav_lay.addRow('Face/Layer:', face_row)
        nav_lay.addRow('Depth Slice:', self._spin_depth)
        lay.addWidget(nav_group)

        # --- Channels ---
        chan_group = QGroupBox('Channels')
        chan_lay = QHBoxLayout(chan_group)
        chan_lay.setSpacing(4)
        self._btn_rgb = QPushButton('RGB')
        self._btn_r   = QPushButton('R')
        self._btn_g   = QPushButton('G')
        self._btn_b   = QPushButton('B')
        self._btn_a   = QPushButton('A')
        for btn in (self._btn_rgb, self._btn_r, self._btn_g,
                    self._btn_b, self._btn_a):
            btn.setCheckable(True)
            btn.setFixedWidth(42)
            chan_lay.addWidget(btn)
        self._btn_rgb.setChecked(True)
        self._chan_btns = QButtonGroup()
        for btn in (self._btn_rgb, self._btn_r, self._btn_g,
                    self._btn_b, self._btn_a):
            self._chan_btns.addButton(btn)
        self._chan_btns.setExclusive(True)
        lay.addWidget(chan_group)

        # --- Tonemapping ---
        tm_group = QGroupBox('Tonemapping')
        tm_lay = QFormLayout(tm_group)
        tm_lay.setSpacing(4)

        self._combo_mode = QComboBox()
        for m in ToneMode:
            self._combo_mode.addItem(m.value)

        self._spin_exposure = _dspin(-8.0, 8.0, 0.0, 0.25, 2, 80)
        self._spin_gamma    = _dspin(0.1, 4.0, 2.2, 0.05, 2, 80)
        self._spin_white    = _dspin(0.1, 20.0, 4.0, 0.5, 1, 80)
        self._lbl_white = QLabel('White point:')

        tm_lay.addRow('Mode:', self._combo_mode)
        tm_lay.addRow('Exposure (EV):', self._spin_exposure)
        tm_lay.addRow('Gamma:', self._spin_gamma)
        tm_lay.addRow(self._lbl_white, self._spin_white)
        lay.addWidget(tm_group)

        # --- Pixel info ---
        px_group = QGroupBox('Pixel Info')
        px_lay = QFormLayout(px_group)
        px_lay.setSpacing(3)
        self._lbl_pos = _label('—', mono=True)
        self._lbl_r   = _label('—', mono=True)
        self._lbl_g   = _label('—', mono=True)
        self._lbl_b   = _label('—', mono=True)
        self._lbl_a   = _label('—', mono=True)
        self._px_swatch = QLabel()
        self._px_swatch.setFixedSize(20, 20)
        self._px_swatch.setStyleSheet('background:#000; border:1px solid #555;')
        pos_row = QHBoxLayout()
        pos_row.addWidget(self._lbl_pos)
        pos_row.addStretch()
        pos_row.addWidget(self._px_swatch)
        px_lay.addRow('Pos:', pos_row)
        px_lay.addRow('R:', self._lbl_r)
        px_lay.addRow('G:', self._lbl_g)
        px_lay.addRow('B:', self._lbl_b)
        px_lay.addRow('A:', self._lbl_a)
        lay.addWidget(px_group)

        # --- View controls ---
        view_lay = QHBoxLayout()
        self._btn_fit = QPushButton('Fit (F)')
        self._btn_1to1 = QPushButton('1:1 (1)')
        view_lay.addWidget(self._btn_fit)
        view_lay.addWidget(self._btn_1to1)
        lay.addLayout(view_lay)

        lay.addStretch()
        return w

    def _build_menu(self):
        mb = self.menuBar()
        file_menu = mb.addMenu('&File')

        open_act = QAction('&Open…', self)
        open_act.setShortcut(QKeySequence.Open)
        open_act.triggered.connect(self._on_open)
        file_menu.addAction(open_act)

        file_menu.addSeparator()
        quit_act = QAction('&Quit', self)
        quit_act.setShortcut(QKeySequence.Quit)
        quit_act.triggered.connect(self.close)
        file_menu.addAction(quit_act)

        view_menu = mb.addMenu('&View')
        fit_act = QAction('Fit to Window', self)
        fit_act.setShortcut('F')
        fit_act.triggered.connect(self._canvas.fit_to_window)
        view_menu.addAction(fit_act)
        actual_act = QAction('Actual Size (1:1)', self)
        actual_act.setShortcut('1')
        actual_act.triggered.connect(self._canvas.zoom_actual)
        view_menu.addAction(actual_act)

    def _connect_signals(self):
        self._spin_mip.valueChanged.connect(self._on_nav_changed)
        self._spin_face.valueChanged.connect(self._on_face_changed)
        self._spin_depth.valueChanged.connect(self._on_nav_changed)

        self._combo_mode.currentTextChanged.connect(self._on_tone_changed)
        self._spin_exposure.valueChanged.connect(self._on_tone_changed)
        self._spin_gamma.valueChanged.connect(self._on_tone_changed)
        self._spin_white.valueChanged.connect(self._on_tone_changed)

        self._btn_rgb.clicked.connect(self._on_channel_changed)
        self._btn_r.clicked.connect(self._on_channel_changed)
        self._btn_g.clicked.connect(self._on_channel_changed)
        self._btn_b.clicked.connect(self._on_channel_changed)
        self._btn_a.clicked.connect(self._on_channel_changed)

        self._canvas.pixel_hovered.connect(self._on_pixel_hovered)
        self._btn_fit.clicked.connect(self._canvas.fit_to_window)
        self._btn_1to1.clicked.connect(self._canvas.zoom_actual)

    # ------------------------------------------------------------------
    # File I/O
    # ------------------------------------------------------------------
    def _on_open(self):
        start_dir = self._settings.value('last_dir', '') or ''
        path, _ = QFileDialog.getOpenFileName(
            self, 'Open Texture',
            start_dir,
            'Textures (*.dds *.exr *.hdr);;DDS (*.dds);;EXR (*.exr);;HDR (*.hdr);;All (*)')
        if path:
            self._open_file(path)

    def open_file(self, path: str):
        self._open_file(path)

    def _open_file(self, path: str):
        ext = path.lower().rsplit('.', 1)[-1]
        try:
            if ext == 'dds':
                self._load_dds(path)
            elif ext in ('exr', 'hdr'):
                self._load_exr(path)
            else:
                self._status_label.setText(f'Unsupported extension: {ext}')
                return
            self._settings.setValue('last_file', path)
            self._settings.setValue('last_dir', os.path.dirname(path))
        except Exception as exc:
            self._status_label.setText(f'Error: {exc}')

    def _load_dds(self, path: str):
        self._dds = dds_mod.load(path)
        self._is_exr = False
        self._exr_data = None

        d = self._dds
        file_kb = os.path.getsize(path) / 1024
        self._lbl_file.setText(os.path.basename(path))
        self._lbl_file.setToolTip(path)
        self._lbl_dims.setText(f'{d.width} × {d.height} × {d.depth}')
        self._lbl_fmt.setText(d.fmt_name + (' (sRGB)' if d.is_srgb else ''))
        type_names = {
            dds_mod.TexType.TEX2D:   '2D Texture',
            dds_mod.TexType.CUBEMAP: 'Cubemap (6 faces)',
            dds_mod.TexType.ARRAY:   f'2D Array [{d.array_size}]',
            dds_mod.TexType.TEX3D:   f'3D / Volume [{d.depth} slices]',
        }
        self._lbl_type.setText(type_names[d.tex_type])
        self._lbl_size.setText(f'{file_kb:.1f} KB')

        self._spin_mip.setRange(0, d.mip_count - 1)
        self._spin_mip.setValue(0)
        self._spin_face.setRange(0, d.array_size - 1)
        self._spin_face.setValue(0)
        max_depth = max(1, d.depth) - 1
        self._spin_depth.setRange(0, max_depth)
        self._spin_depth.setValue(0)
        self._spin_depth.setEnabled(d.tex_type == dds_mod.TexType.TEX3D)

        self._update_face_label()
        self._reload_subresource()
        self._update_status()

    def _load_exr(self, path: str):
        self._exr_data = _load_exr(path)
        self._is_exr = True
        self._dds = None

        h, w = self._exr_data.shape[:2]
        file_kb = os.path.getsize(path) / 1024
        self._lbl_file.setText(os.path.basename(path))
        self._lbl_file.setToolTip(path)
        self._lbl_dims.setText(f'{w} × {h}')
        self._lbl_fmt.setText('R32G32B32A32_FLOAT')
        self._lbl_type.setText('2D (EXR)')
        self._lbl_size.setText(f'{file_kb:.1f} KB')

        self._spin_mip.setRange(0, 0)
        self._spin_face.setRange(0, 0)
        self._spin_depth.setRange(0, 0)
        self._spin_depth.setEnabled(False)

        self._canvas.set_image(self._exr_data, self._tone)
        self._update_status()

    def _reload_subresource(self):
        if self._dds is None:
            return
        mip   = self._spin_mip.value()
        face  = self._spin_face.value()
        depth = self._spin_depth.value()
        try:
            rgba = self._dds.get_face(array_idx=face, mip=mip, depth_slice=depth)
            self._canvas.set_image(rgba, self._tone)
        except Exception as exc:
            self._status_label.setText(f'Decode error: {exc}')

    # ------------------------------------------------------------------
    # Slots
    # ------------------------------------------------------------------
    def _on_nav_changed(self):
        self._reload_subresource()
        self._update_status()

    def _on_face_changed(self):
        self._update_face_label()
        mip   = self._spin_mip.value()
        face  = self._spin_face.value()
        # When mip or face changes on a 3D tex, update depth range
        if self._dds and self._dds.tex_type == dds_mod.TexType.TEX3D:
            new_d = max(1, self._dds.depth >> mip) - 1
            self._spin_depth.setRange(0, new_d)
        self._reload_subresource()
        self._update_status()

    def _on_tone_changed(self):
        mode_str = self._combo_mode.currentText()
        try:
            mode = ToneMode(mode_str)
        except ValueError:
            mode = ToneMode.LINEAR
        is_reinhard_ext = (mode == ToneMode.REINHARD_EXT)
        self._lbl_white.setVisible(is_reinhard_ext)
        self._spin_white.setVisible(is_reinhard_ext)

        self._tone.mode        = mode
        self._tone.exposure_ev = self._spin_exposure.value()
        self._tone.gamma       = self._spin_gamma.value()
        self._tone.white_point = self._spin_white.value()
        self._canvas.set_tone_mapper(self._tone)

    def _on_channel_changed(self):
        mapping = {
            self._btn_rgb: ChannelMode.RGB,
            self._btn_r:   ChannelMode.R,
            self._btn_g:   ChannelMode.G,
            self._btn_b:   ChannelMode.B,
            self._btn_a:   ChannelMode.A,
        }
        for btn, ch in mapping.items():
            if btn.isChecked():
                self._tone.channel = ch
                break
        self._canvas.refresh_tone()

    def _on_pixel_hovered(self, px: int, py: int,
                           r: float, g: float, b: float, a: float):
        self._lbl_pos.setText(f'({px}, {py})')
        self._lbl_r.setText(f'{r:.6f}')
        self._lbl_g.setText(f'{g:.6f}')
        self._lbl_b.setText(f'{b:.6f}')
        self._lbl_a.setText(f'{a:.6f}')
        # Swatch uses tonemapped display colour (clamp to [0,1] with exposure)
        sc = min(1.0, max(0.0, (2.0 ** self._tone.exposure_ev)))
        sr = int(min(255, max(0, r * sc * 255)))
        sg = int(min(255, max(0, g * sc * 255)))
        sb = int(min(255, max(0, b * sc * 255)))
        self._px_swatch.setStyleSheet(
            f'background:rgb({sr},{sg},{sb}); border:1px solid #555;')

    # ------------------------------------------------------------------
    # Misc helpers
    # ------------------------------------------------------------------
    def _update_face_label(self):
        if self._dds is None:
            self._lbl_face_name.setText('')
            return
        idx = self._spin_face.value()
        self._lbl_face_name.setText(self._dds.face_label(idx))

    def _update_status(self):
        if self._dds:
            d = self._dds
            mip  = self._spin_mip.value()
            face = self._spin_face.value()
            w = max(1, d.width  >> mip)
            h = max(1, d.height >> mip)
            label = d.face_label(face)
            self._status_label.setText(
                f'{os.path.basename(d.path)}  |  '
                f'{w}×{h}  mip={mip}  face={label}  |  {d.fmt_name}')
        elif self._is_exr and self._exr_data is not None:
            h, w = self._exr_data.shape[:2]
            self._status_label.setText(
                f'{self._lbl_file.text()}  |  {w}×{h}  |  EXR RGBA32F')

    def _apply_dark_theme(self):
        self.setStyleSheet("""
            QMainWindow, QWidget { background-color: #2b2b2b; color: #ddd; }
            QGroupBox { border: 1px solid #555; border-radius: 4px;
                        margin-top: 8px; padding-top: 4px; color: #bbb; }
            QGroupBox::title { subcontrol-origin: margin; left: 6px;
                               padding: 0 3px; }
            QPushButton { background: #3c3f41; border: 1px solid #555;
                          border-radius: 3px; padding: 3px 8px; color: #ddd; }
            QPushButton:hover  { background: #4c5052; }
            QPushButton:checked { background: #214283; border-color: #4a90d9; }
            QPushButton:pressed { background: #1a3260; }
            QComboBox, QSpinBox, QDoubleSpinBox {
                background: #3c3f41; border: 1px solid #555;
                border-radius: 3px; padding: 2px 4px; color: #ddd; }
            QComboBox QAbstractItemView { background: #3c3f41; color: #ddd; }
            QLabel { color: #ccc; }
            QStatusBar { color: #aaa; }
            QMenuBar { background: #2b2b2b; color: #ccc; }
            QMenuBar::item:selected { background: #3c3f41; }
            QMenu { background: #3c3f41; color: #ccc; }
            QMenu::item:selected { background: #214283; }
            QSplitter::handle { background: #444; }
        """)
