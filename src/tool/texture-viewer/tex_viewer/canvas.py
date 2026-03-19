"""ImageCanvas — QWidget that displays a float32 RGBA image with zoom/pan.

Signals
-------
pixel_hovered(int, int, float, float, float, float)
    (px_x, px_y, r, g, b, a) in image-space raw float values.
    Emitted on mouse move when an image is loaded.
"""

from __future__ import annotations
import numpy as np
from PyQt5.QtCore import Qt, QPoint, QRect, QSize, pyqtSignal
from PyQt5.QtGui import QImage, QPixmap, QPainter, QColor, QPen, QFont
from PyQt5.QtWidgets import QWidget, QSizePolicy

from .tone_map import ToneMapper


class ImageCanvas(QWidget):
    pixel_hovered = pyqtSignal(int, int, float, float, float, float)

    _MIN_ZOOM = 1.0 / 64
    _MAX_ZOOM = 64.0
    _CHECKER_SIZE = 8

    def __init__(self, parent=None):
        super().__init__(parent)
        self.setMouseTracking(True)
        self.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
        self.setMinimumSize(64, 64)
        self.setFocusPolicy(Qt.StrongFocus)
        self.setStyleSheet('background-color: #2b2b2b;')

        self._raw: np.ndarray | None = None    # float32 (H, W, 4)
        self._pixmap: QPixmap | None = None    # tonemapped display
        self._tone: ToneMapper = ToneMapper()
        self._zoom: float = 1.0
        self._pan: QPoint = QPoint(0, 0)
        self._drag_start: QPoint | None = None
        self._drag_pan_start: QPoint | None = None

        self._checker_pixmap: QPixmap | None = None

    # ------------------------------------------------------------------
    # Public API
    # ------------------------------------------------------------------
    def set_image(self, rgba: np.ndarray, tone_mapper: ToneMapper | None = None):
        """Load a float32 (H, W, 4) RGBA array and refresh display."""
        self._raw = rgba
        if tone_mapper is not None:
            self._tone = tone_mapper
        self._rebuild_pixmap()
        self.fit_to_window()

    def set_tone_mapper(self, tone_mapper: ToneMapper):
        self._tone = tone_mapper
        if self._raw is not None:
            self._rebuild_pixmap()
            self.update()

    def refresh_tone(self):
        """Re-apply tonemapping with the current ToneMapper state."""
        if self._raw is not None:
            self._rebuild_pixmap()
            self.update()

    def fit_to_window(self):
        """Scale and center image to fit the widget."""
        if self._pixmap is None:
            return
        pw, ph = self._pixmap.width(), self._pixmap.height()
        ww, wh = self.width(), self.height()
        if pw == 0 or ph == 0 or ww == 0 or wh == 0:
            return
        scale = min(ww / pw, wh / ph)
        self._zoom = scale
        self._center_image()
        self.update()

    def zoom_actual(self):
        """Reset to 1:1 pixel size."""
        self._zoom = 1.0
        self._center_image()
        self.update()

    # ------------------------------------------------------------------
    # Qt events
    # ------------------------------------------------------------------
    def paintEvent(self, _event):
        painter = QPainter(self)
        painter.setRenderHint(QPainter.SmoothPixmapTransform, self._zoom < 1.0)
        self._draw_checker(painter)
        if self._pixmap:
            pw = int(self._pixmap.width()  * self._zoom)
            ph = int(self._pixmap.height() * self._zoom)
            dst = QRect(self._pan.x(), self._pan.y(), pw, ph)
            painter.drawPixmap(dst, self._pixmap)
        if self._pixmap is None:
            painter.setPen(QColor(120, 120, 120))
            painter.drawText(self.rect(), Qt.AlignCenter, 'No image loaded')

    def resizeEvent(self, event):
        super().resizeEvent(event)
        self._checker_pixmap = None  # rebuild on next paint

    def wheelEvent(self, event):
        delta = event.angleDelta().y()
        factor = 1.15 if delta > 0 else 1.0 / 1.15
        self._zoom_at(event.pos(), factor)

    def mousePressEvent(self, event):
        if event.button() in (Qt.LeftButton, Qt.MiddleButton):
            self._drag_start = event.pos()
            self._drag_pan_start = QPoint(self._pan)
            self.setCursor(Qt.ClosedHandCursor)

    def mouseMoveEvent(self, event):
        if self._drag_start is not None:
            delta = event.pos() - self._drag_start
            self._pan = self._drag_pan_start + delta
            self.update()
        self._emit_pixel(event.pos())

    def mouseReleaseEvent(self, event):
        if event.button() in (Qt.LeftButton, Qt.MiddleButton):
            self._drag_start = None
            self._drag_pan_start = None
            self.unsetCursor()

    def mouseDoubleClickEvent(self, _event):
        self.fit_to_window()

    def keyPressEvent(self, event):
        if event.key() == Qt.Key_F:
            self.fit_to_window()
        elif event.key() in (Qt.Key_1, Qt.Key_0):
            self.zoom_actual()
        elif event.key() == Qt.Key_Plus:
            self._zoom_at(self._widget_center(), 1.5)
        elif event.key() == Qt.Key_Minus:
            self._zoom_at(self._widget_center(), 1.0 / 1.5)
        else:
            super().keyPressEvent(event)

    # ------------------------------------------------------------------
    # Private helpers
    # ------------------------------------------------------------------
    def _rebuild_pixmap(self):
        if self._raw is None:
            self._pixmap = None
            return
        uint8 = self._tone.apply(self._raw)  # (H, W, 4) uint8
        h, w = uint8.shape[:2]
        # QImage expects bytes in RGBA order (which is what we have)
        buf = uint8.tobytes()
        qimg = QImage(buf, w, h, w * 4, QImage.Format_RGBA8888)
        self._pixmap = QPixmap.fromImage(qimg.copy())  # .copy() detaches from buf

    def _center_image(self):
        if self._pixmap is None:
            return
        pw = int(self._pixmap.width()  * self._zoom)
        ph = int(self._pixmap.height() * self._zoom)
        self._pan = QPoint((self.width()  - pw) // 2,
                           (self.height() - ph) // 2)

    def _zoom_at(self, pos: QPoint, factor: float):
        old = self._zoom
        self._zoom = max(self._MIN_ZOOM, min(self._MAX_ZOOM, old * factor))
        # Keep the point under the cursor fixed
        img_x = (pos.x() - self._pan.x()) / old
        img_y = (pos.y() - self._pan.y()) / old
        self._pan = QPoint(int(pos.x() - img_x * self._zoom),
                           int(pos.y() - img_y * self._zoom))
        self.update()

    def _widget_center(self) -> QPoint:
        return QPoint(self.width() // 2, self.height() // 2)

    def _emit_pixel(self, widget_pos: QPoint):
        if self._raw is None or self._zoom == 0:
            return
        img_x = (widget_pos.x() - self._pan.x()) / self._zoom
        img_y = (widget_pos.y() - self._pan.y()) / self._zoom
        h, w = self._raw.shape[:2]
        ix, iy = int(img_x), int(img_y)
        if 0 <= ix < w and 0 <= iy < h:
            r, g, b, a = self._raw[iy, ix]
            self.pixel_hovered.emit(ix, iy, float(r), float(g), float(b), float(a))

    def _draw_checker(self, painter: QPainter):
        """Draw a grey checkerboard background (transparency indicator)."""
        if self._checker_pixmap is None or \
                self._checker_pixmap.size() != self.size():
            self._checker_pixmap = self._make_checker(
                self.width(), self.height(), self._CHECKER_SIZE)
        painter.drawPixmap(0, 0, self._checker_pixmap)

    @staticmethod
    def _make_checker(w: int, h: int, sz: int) -> QPixmap:
        img = QImage(w, h, QImage.Format_RGB32)
        c1 = QColor(60, 60, 60)
        c2 = QColor(45, 45, 45)
        p = QPainter(img)
        for row in range(0, h, sz):
            for col in range(0, w, sz):
                color = c1 if ((row // sz + col // sz) % 2 == 0) else c2
                p.fillRect(col, row, sz, sz, color)
        p.end()
        return QPixmap.fromImage(img)
