#!/usr/bin/env python3
"""Texture viewer — open DDS or EXR/HDR files.

Usage:
    tex-viewer.py [file]
"""

import sys
import os

sys.path.insert(0, os.path.dirname(__file__))

from PyQt5.QtWidgets import QApplication
from main_window import MainWindow


def main() -> int:
    app = QApplication(sys.argv)
    app.setApplicationName('Texture Viewer')
    app.setOrganizationName('garnet')

    win = MainWindow()
    win.show()

    if len(sys.argv) > 1:
        win.open_file(sys.argv[1])

    return app.exec_()


if __name__ == '__main__':
    sys.exit(main())
