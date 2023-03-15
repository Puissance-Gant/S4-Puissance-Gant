import sys
from PyQt5 import QtWidgets, uic

# import pyqtgraph as pygraph
from InterfacePuissanceGant import Ui_MainWindow


class MainWindow(QtWidgets.QMainWindow, Ui_MainWindow):
    def __init__(self, *args, obj=None, **kwargs):
        super(MainWindow, self).__init__(*args, **kwargs)
        self.setupUi(self)
        self.Button_ModeManuel.clicked.connect(button_ModeManuel)
        self.LineEdit_ConsoleManuel.returnPressed.connect(consoleManuel_EnvoyerCommande)
        # self.Button_ModeManuel.released.connect(boutonRelache)


app = QtWidgets.QApplication(sys.argv)


def button_ModeManuel(self):
    if window.Button_ModeManuel.isChecked():
        window.Text_EtatDuRobot.setText("Mode manuel activé \nEntrer le pourcentage du moteur choisi \n\nFormat : 50")
    else:
        window.Text_EtatDuRobot.setText("Mode automatique activé")


def consoleManuel_EnvoyerCommande():
    texte = "Commande reçue : " + window.LineEdit_ConsoleManuel.text()
    window.Text_EtatDuRobot.setText(texte)
    window.LineEdit_ConsoleManuel.clear()

    # Ajouter vérification du mode manuel
    # ajouter du code pour faire changer la commande en entier.
    # On indiquera si la commande est acceptable et on l'enverra si oui


window = MainWindow()

window.show()
app.exec()
