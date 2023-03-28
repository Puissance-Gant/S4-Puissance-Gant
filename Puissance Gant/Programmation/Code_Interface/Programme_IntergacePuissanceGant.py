import sys, os
from PyQt5 import QtWidgets

# Transfromer l'interface de Qt Designer (format .ui) en code python directement
# dans le code au lieu de passer par le terminal
os.system("pyuic5 InterfacePuissanceGant.ui -o InterfacePuissanceGant.py")

# import pyqtgraph as pygraph
from InterfacePuissanceGant import Ui_MainWindow


class MainWindow(QtWidgets.QMainWindow, Ui_MainWindow):
    def __init__(self, *args, obj=None, **kwargs):
        super(MainWindow, self).__init__(*args, **kwargs)
        self.setupUi(self)
        self.Label_CrochetVert.setVisible(False)

        self.LineEdit_ConsoleManuel.returnPressed.connect(consoleManuel_EnvoyerCommande)
        self.Button_ArretUrgence.clicked.connect(button_ArretUrgence)
        self.Button_ModeManuel.clicked.connect(button_ModeManuel)
        self.Button_DemarrageServeur.clicked.connect(button_DemarrageServeur)


# region code de boutons
def button_ModeManuel(self):
    # ===================
    # Ajouter le changement de vitesse pour avoir tous les moteurs à 50%
    # Possiblement fait avec un tableau de valeurs de position des moteurs envoyé à chaque
    # ===================
    if window.Button_ModeManuel.isChecked():
        window.Text_EtatDuRobot.setText("Mode manuel activé \nEntrer le pourcentage du moteur choisi \n\nFormat : 50")

        window.LineEdit_ConsoleManuel.setReadOnly(False)
        window.LineEdit_ConsoleManuel.clear()
        window.LineEdit_ConsoleManuel.setPlaceholderText("Écrire une commande ici")
    else:
        window.Text_EtatDuRobot.setText("Mode automatique activé")

        window.LineEdit_ConsoleManuel.setReadOnly(True)
        window.LineEdit_ConsoleManuel.clear()
        window.LineEdit_ConsoleManuel.setPlaceholderText("Mode automatique activé, console bloquée")


def button_ArretUrgence(self):
    if window.Button_ArretUrgence.isChecked():
        window.Text_EtatDuRobot.setText("Arrêt d'urgence activé." + "\n\n" + "Robot désactivé, moteurs bloqués.")
        window.Button_ModeManuel.setEnabled(False)
        window.LineEdit_ConsoleManuel.setEnabled(False)
    else:
        window.Text_EtatDuRobot.setText("Arrêt d'urgence désactivé." + "\n\n" + "Robot réactivé en mode manuel.")
        window.Button_ModeManuel.setEnabled(True)
        window.LineEdit_ConsoleManuel.setEnabled(True)
        window.Button_ModeManuel.setChecked(True)


def button_DemarrageServeur():
    # inclure l'état de connexion du serveur. Actuellement fait avec le label du crochet
    # Ajouter la logique de connexion du serveur
    if window.Label_CroixRouge.isVisible():
        window.Label_CroixRouge.setVisible(False)
        window.Label_CrochetVert.setVisible(True)
    else:
        window.Label_CroixRouge.setVisible(True)
        window.Label_CrochetVert.setVisible(False)

# endregion

#region code d'écriture de texte (lineEdit et textEdit)
def consoleManuel_EnvoyerCommande():
    val_entree = window.LineEdit_ConsoleManuel.text()
    texte = "Commande reçue : " + val_entree
    try:
        valeur = int(val_entree)
        if valeur < 0 or valeur > 100:
            raise Exception("valNonConforme")
        else:
            texte = texte + "\n" + "Commande envoyée au robot."
    except Exception:
        texte = texte + "\n" + "Mauvais format. Entrer un entier entre 0 et 100 inclusivement"
    window.Text_EtatDuRobot.setText(texte)
    window.LineEdit_ConsoleManuel.clear()

# endregion

# region code pour l'accès à partir du code de communication

# endregion


app = QtWidgets.QApplication(sys.argv)
window = MainWindow()

window.show()
app.exec()
