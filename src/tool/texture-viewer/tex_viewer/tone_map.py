"""HDR tonemapping operators and display conversion.

All operators work on float32 numpy arrays (H, W, 4) RGBA in linear space.
Output is uint8 RGBA (H, W, 4) ready for QImage.
"""

from __future__ import annotations
from dataclasses import dataclass, field
from enum import Enum
import numpy as np


class ToneMode(str, Enum):
    LINEAR   = 'Linear'
    REINHARD = 'Reinhard'
    REINHARD_EXT = 'Reinhard Ext'
    ACES     = 'ACES'
    FILMIC   = 'Filmic (UC2)'


class ChannelMode(str, Enum):
    RGB = 'RGB'
    R   = 'R'
    G   = 'G'
    B   = 'B'
    A   = 'A'


@dataclass
class ToneMapper:
    mode: ToneMode     = ToneMode.LINEAR
    channel: ChannelMode = ChannelMode.RGB
    exposure_ev: float = 0.0      # EV stops; 0 = no change
    gamma: float       = 2.2
    white_point: float = 4.0      # used by Reinhard Extended

    def apply(self, rgba: np.ndarray) -> np.ndarray:
        """rgba: float32 (H, W, 4) → uint8 (H, W, 4) for display."""
        rgb = rgba[:, :, :3].astype(np.float32)
        alpha = rgba[:, :, 3:4].astype(np.float32)

        # Channel isolation (done before tonemapping so values are meaningful)
        ch = self.channel
        if ch == ChannelMode.R:
            rgb = np.repeat(rgb[:, :, 0:1], 3, axis=2)
        elif ch == ChannelMode.G:
            rgb = np.repeat(rgb[:, :, 1:2], 3, axis=2)
        elif ch == ChannelMode.B:
            rgb = np.repeat(rgb[:, :, 2:3], 3, axis=2)
        elif ch == ChannelMode.A:
            rgb = np.repeat(alpha, 3, axis=2)

        # Exposure
        rgb = rgb * (2.0 ** self.exposure_ev)

        # Tonemapping
        m = self.mode
        if m == ToneMode.LINEAR:
            rgb = np.clip(rgb, 0.0, 1.0)
        elif m == ToneMode.REINHARD:
            rgb = rgb / (1.0 + rgb)
        elif m == ToneMode.REINHARD_EXT:
            w2 = self.white_point ** 2
            rgb = rgb * (1.0 + rgb / w2) / (1.0 + rgb)
        elif m == ToneMode.ACES:
            rgb = _aces(rgb)
        elif m == ToneMode.FILMIC:
            rgb = _uncharted2(rgb)
        rgb = np.clip(rgb, 0.0, 1.0)

        # Gamma
        if self.gamma > 0.0:
            rgb = np.power(rgb + 1e-7, 1.0 / self.gamma)
        rgb = np.clip(rgb, 0.0, 1.0)

        # Re-attach alpha (always linear clamp [0,1])
        alpha_out = np.clip(alpha, 0.0, 1.0)
        out = np.concatenate([rgb, alpha_out], axis=2)
        return (out * 255.0 + 0.5).astype(np.uint8)


# ---------------------------------------------------------------------------
# Tonemapping math
# ---------------------------------------------------------------------------
def _aces(x: np.ndarray) -> np.ndarray:
    """Narkowicz ACES filmic approximation."""
    a, b, c, d, e = 2.51, 0.03, 2.43, 0.59, 0.14
    return (x * (a * x + b)) / (x * (c * x + d) + e)


def _uncharted2_partial(x: np.ndarray) -> np.ndarray:
    A, B, C, D, E, F = 0.15, 0.50, 0.10, 0.20, 0.02, 0.30
    return ((x * (A * x + C * B) + D * E) / (x * (A * x + B) + D * F)) - E / F


def _uncharted2(x: np.ndarray, exposure_bias: float = 2.0) -> np.ndarray:
    """Uncharted 2 / John Hable filmic tonemapper."""
    curr = _uncharted2_partial(x * exposure_bias)
    white_scale = 1.0 / _uncharted2_partial(np.array(11.2, dtype=np.float32))
    return curr * white_scale
