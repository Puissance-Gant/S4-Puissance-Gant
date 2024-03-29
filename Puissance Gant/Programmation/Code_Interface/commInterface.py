from PyQt5.QtCore import QThread, pyqtSignal
import paho.mqtt.client as mqtt
import time


class CommInterface(QThread):
    MQTT_ADDRESS = "192.168.137.254"
    MQTT_USER = 'puissance'
    MQTT_PASSWORD = 'puissance'
    MQTT_TOPIC_COMMANDE = 'Eq7_PuissanceGant_S4/gant/commandes'
    MQTT_TOPIC_MANUEL_ACTIF = 'Eq7_PuissanceGant_S4/interface/manuel_actif'
    MQTT_TOPIC_MANUEL_COMMANDE = 'Eq7_PuissanceGant_S4/interface/commande'
    MQTT_TOPIC_ARRET_URGENCE_ACTIF = 'Eq7_PuissanceGant_S4/interface/arret_urgence_actif'
    MQTT_TOPIC_PUISSANCE = 'Eq7_PuissanceGant_S4/OpenCR/puissance'

    msgCommandeAuto = pyqtSignal(str)
    msgDelaiCommandeAuto = pyqtSignal(str)
    delaiCommandeAuto = time.time()
    msgConnexion = pyqtSignal(bool)
    valEnergie = pyqtSignal(int)

    # ----------------------------------------------------------------------------
    # Fonctions MQTT

    # Initialiser la connexion au lancement du programme
    def __init__(self):
        QThread.__init__(self)
        print('MQTT to influxDB bridge')

        global mqtt_client

        mqtt_client = mqtt.Client("interface")
        mqtt_client.enable_logger()
        mqtt_client.username_pw_set(self.MQTT_USER, self.MQTT_PASSWORD)
        mqtt_client.on_connect = self.on_connect
        mqtt_client.on_disconnect = self.on_disconnect
        mqtt_client.on_message = self.on_message

    # Lancer la connexion au serveur lorsqu'on démarre le Qthread
    def run(self):
        mqtt_client.connect_async(host=self.MQTT_ADDRESS, port=1883)
        mqtt_client.loop_forever()

    # Arrêter la connexion lorsqu'on arrête le Qthread
    def kill(self):
        mqtt_client.disconnect()
        self.terminate()

    # on_connect est appelé lorsque le serveur est atteint
    def on_connect(self, client, userdata, flags, rc):
        print('Connected with result code ' + str(rc))
        client.subscribe(self.MQTT_TOPIC_COMMANDE, 1)
        client.subscribe(self.MQTT_TOPIC_CALIB, 1)
        client.subscribe(self.MQTT_TOPIC_PUISSANCE, 1)

        self.msgConnexion.emit(True)

    # on_disconnect est appelé lorsque la connexion au serveur est perdue
    def on_disconnect(self, client, userdata, flags):
        print('Disconnected from broker')
        self.msgConnexion.emit(False)

    # Recevoir un message du serveur
    def on_message(self, client, userdata, msg):
        match msg.topic:
            # Recevoir un message de commande automatique
            case self.MQTT_TOPIC_COMMANDE:
                self.msgCommandeAuto.emit(str(msg.payload))
                # Noter le délai entre chaque message
                delai = f"{1000*(time.time() - self.delaiCommandeAuto):.1f}"
                self.msgDelaiCommandeAuto.emit(delai)
                self.delaiCommandeAuto = time.time()

            case self.MQTT_TOPIC_PUISSANCE:
                #print(msg.payload)
                self.valEnergie.emit(abs(int(msg.payload)))

            case _:
                return
        # mqtt_client.publish('Eq7_PuissanceGant_S4/OpenCR/test', str("BENIS"), qos=0, retain=False)

    # Envoyer une commande manuelle au serveur
    def envoyerCommandeManuelle(self, commande):
        msg = '<' + str(commande) + '>'
        mqtt_client.publish(self.MQTT_TOPIC_MANUEL_COMMANDE, msg, qos=1, retain=False)

    # Indiquer au serveur si le mode manuel est activé
    def envoyerManuelActif(self, actif):
        if actif:
            msg = '1'
        else:
            msg = '0'
        mqtt_client.publish(self.MQTT_TOPIC_MANUEL_ACTIF, msg, qos=2, retain=False)

    # Indiquer au serveur si l'arrêt d'urgence est activé
    def envoyerArretUrgenceActif(self, actif: bool):
        if actif:
            msg = '1'
        else:
            msg = '0'
        mqtt_client.publish(self.MQTT_TOPIC_ARRET_URGENCE_ACTIF, msg, qos=2, retain=False)

