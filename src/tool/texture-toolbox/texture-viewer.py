#!/usr/bin/env python3
"""Texture viewer — open DDS or EXR/HDR files.

Usage:
    tex-viewer.py [file]
"""

import sys
import os

sys.path.insert(0, os.path.join(os.path.dirname(__file__), 'texture-viewer'))

from PyQt5.QtWidgets import QApplication
from main_window import MainWindow


def main() -> int:
    app = QApplication(sys.argv)
    app.setApplicationName('Texture Viewer')
    app.setOrganizationName('garnet')

    file_arg = sys.argv[1] if len(sys.argv) > 1 else None
    win = MainWindow(initial_file=file_arg)
    win.show()

    return app.exec_()


if __name__ == '__main__':
    sys.exit(main())
