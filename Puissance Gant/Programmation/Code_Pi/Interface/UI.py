import sys
from PyQt5 import QtWidgets, uic

from TestFenetre import Ui_MainWindow


class MainWindow(QtWidgets.QMainWindow, Ui_MainWindow):
    def __init__(self, *args, obj=None, **kwargs):
        super(MainWindow, self).__init__(*args, **kwargs)
        self.setupUi(self)
        self.bouton.pressed.connect(boutonAppuye)
        self.bouton.released.connect(boutonRelache)
        
        
        



app = QtWidgets.QApplication(sys.argv)
def boutonAppuye():
    window.textEdit.setText("yes benis")
def boutonRelache():
    window.textEdit.setText("no benis")
window = MainWindow()


window.show()
app.exec()
