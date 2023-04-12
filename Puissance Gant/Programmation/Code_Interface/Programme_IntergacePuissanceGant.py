import os
import sys
import pyqtgraph
from PyQt5 import QtWidgets

# Transfromer l'interface de Qt Designer (format .ui) en code python directement
# dans le code au lieu de passer par le terminal
os.system("pyuic5 InterfacePuissanceGant.ui -o InterfacePuissanceGant.py")

# import pyqtgraph as pygraph
from InterfacePuissanceGant import Ui_MainWindow
from commInterface import CommInterface


class MainWindow(QtWidgets.QMainWindow, Ui_MainWindow):
    def __init__(self, *args, obj=None, **kwargs):
        super(MainWindow, self).__init__(*args, **kwargs)
        self.setupUi(self)
        self.setupPlot()

        self.commInterface = CommInterface()
        self.commInterface.start()
        self.commInterface.msgCommandeAuto.connect(text_CommandeMoteurs)
        self.commInterface.msgConnexion.connect(etatConnexionServeur)
        self.commInterface.msgDelaiCommandeAuto.connect(label_delaiResist)
        self.commInterface.valEnergie.connect(plot_Energie)

        self.Label_CrochetVert.setVisible(False)

        self.LineEdit_ConsoleManuel.returnPressed.connect(consoleManuel_EnvoyerCommande)
        self.Button_ArretUrgence.clicked.connect(button_ArretUrgence)
        self.Button_ModeManuel.clicked.connect(button_ModeManuel)

    def setupPlot(self):
        tailleValEnergieTableau = 20
        self.valEnergieTableau = tailleValEnergieTableau * [0]
        self.Plot_Energie.setBackground('w')
        self.Plot_Energie.showGrid(x=False, y=True)
        self.Plot_Energie.setYRange(0, 600)
        self.Plot_Energie.setTitle("Puissance consommée (mW)", color='k')


# region code de boutons
def button_ModeManuel():
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


def button_ArretUrgence():
    if window.Button_ArretUrgence.isChecked():
        window.Text_EtatDuRobot.setText("Arrêt d'urgence activé." + "\n\n" + "Robot désactivé, moteurs bloqués.")
        window.Button_ModeManuel.setEnabled(False)
        window.LineEdit_ConsoleManuel.setEnabled(False)
    else:
        window.Button_ModeManuel.setEnabled(True)
        window.LineEdit_ConsoleManuel.setEnabled(True)
        window.Button_ModeManuel.setChecked(True)
        button_ModeManuel()




# endregion

#region code d'écriture de texte (lineEdit et textEdit)

# Envoyer une commande manuelle au robot selon le moteur sélectionné
def envoyerCommandeManuelle(msg):
    boutonActif = window.ButtonGroup_MoteursModeManuel.checkedButton().objectName()
    if boutonActif.__contains__('Pouce'):
        window.commInterface.envoyerCommandeManuelle(msg + 'A')
    elif boutonActif.__contains__('Index'):
        window.commInterface.envoyerCommandeManuelle(msg + 'B')
    elif boutonActif.__contains__('Majeur'):
        window.commInterface.envoyerCommandeManuelle(msg + 'C')
    elif boutonActif.__contains__('Incl'):
        window.commInterface.envoyerCommandeManuelle(msg + 'D')

# Lire et analyser la console d'entrée manuelle
def consoleManuel_EnvoyerCommande():
    val_entree = window.LineEdit_ConsoleManuel.text()
    texte = "Commande reçue : " + val_entree
    try:
        valeur = int(val_entree)
        if valeur < 0 or valeur > 100:
            raise Exception("valNonConforme")
        else:
            texte = texte + "\n" + "Commande envoyée au robot."
            envoyerCommandeManuelle(str(valeur))
    except Exception:
        texte = texte + "\n" + "Mauvais format. Entrer un entier entre 0 et 100 inclusivement"
    window.Text_EtatDuRobot.setText(texte)
    window.LineEdit_ConsoleManuel.clear()


# Afficher la commande actuelle des moteurs dans le tableau de l'interface
def text_CommandeMoteurs(msg):
    msgRecu = ""
    msgDemarre = False
    for c in msg:
        if msgDemarre:
            match c:
                case 'A':
                    window.Text_PourcentagePouce.setText(msgRecu + '%')
                    msgRecu = ""
                case 'B':
                    window.Text_PourcentageIndex.setText(msgRecu + '%')
                    msgRecu = ""
                case 'C':
                    window.Text_PourcentageMajeur.setText(msgRecu + '%')
                    msgRecu = ""
                case 'D':
                    window.Text_PourcentageInclinaison.setText(msgRecu + '%')
                    msgRecu = ""
                case 'E': # à retirer quand la comm n'aura plus de E
                    #window.Text_PourcentageRotation.setText(msgRecu + '%')
                    msgRecu = ""
                case '>':
                    msgRecu = ""
                case _:
                    msgRecu = msgRecu + c
        elif c == '<':
            msgDemarre = True



def label_delaiResist(msg):
    window.Label_DelaiResist.setText(msg)

# endregion

# region code pour l'accès à partir du code de communication

# Indique si l'interface est connectée au serveur MQTT.
def etatConnexionServeur(msg: bool):
    if msg:
        window.Label_CroixRouge.setVisible(False)
        window.Label_CrochetVert.setVisible(True)
    else:
        window.Label_CroixRouge.setVisible(True)
        window.Label_CrochetVert.setVisible(False)

# endregion

# region Code autre

def plot_Energie(val):
    window.valEnergieTableau.pop(0)
    window.valEnergieTableau.append(val)
    window.Plot_Energie.clear()
    window.Plot_Energie.plot(window.valEnergieTableau, pen=pyqtgraph.mkPen('k'))


# endregion


app = QtWidgets.QApplication(sys.argv)
window = MainWindow()

window.show()
app.exec()
