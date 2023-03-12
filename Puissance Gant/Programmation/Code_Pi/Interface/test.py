import sys
from PyQt5.QtWidgets import QApplication, QMainWindow, QLabel, QLineEdit, QVBoxLayout, QWidget, QPushButton


class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()

        self.setWindowTitle("My App")
        self.label = QLabel()

        self.button = QPushButton("Press Me!")
        self.button.setCheckable(True)
        self.button.pressed.connect(self.the_button_was_clicked)
        self.button.released.connect(self.the_button_was_released)

        layout = QVBoxLayout()
        layout.addWidget(self.button)
        layout.addWidget(self.label)
        container = QWidget()
        container.setLayout(layout)

        # Set the central widget of the Window.
        self.setCentralWidget(container)

    def the_button_was_clicked(self):
        self.label.setText("benis")

    def the_button_was_released(self):
        self.label.setText("no benis")


app = QApplication(sys.argv)

window = MainWindow()
window.show()

app.exec()