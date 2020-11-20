"""
"""

from PyQt5 import QtWidgets
from PyQt5.QtWidgets import QApplication, QMainWindow
import sys


class AppConfig:
    """
    """

    def __init__(self):
        self.origin_x_position = 100
        self.origin_y_position = 100
        self.application_width = 400
        self.application_height = 400

        self.application_name = "Home Weather Station Mini"
        self.application_version = "2020.11 Beta 1"
        self.application_title_window = self.application_name + \
            " - " + self.application_version


def main():
    app_config = AppConfig()
    app = QApplication(sys.argv)
    win = QMainWindow()
    win.setGeometry(
        app_config.origin_x_position, app_config.origin_y_position,
        app_config.application_width, app_config.application_height
    )
    win.setWindowTitle(app_config.application_title_window)

    win.show()
    sys.exit(app.exec_())


if __name__ == "__main__":
    main()
