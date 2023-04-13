from PyQt5.QtCore import QThread, pyqtSignal
import paho.mqtt.client as mqtt
import time


class CommInterface(QThread):
    MQTT_ADDRESS = "192.168.137.254"
    MQTT_USER = 'puissance'
    MQTT_PASSWORD = 'puissance'
    MQTT_TOPIC_COMMANDE = 'Eq7_PuissanceGant_S4/gant/commandes'
    MQTT_TOPIC_CALIB = 'Eq7_PuissanceGant_S4/gant/calibration'
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

    def run(self):
        mqtt_client.connect_async(host=self.MQTT_ADDRESS, port=1883)
        mqtt_client.loop_forever()

    def kill(self):
        mqtt_client.disconnect()
        self.terminate()

    def on_connect(self, client, userdata, flags, rc):
        print('Connected with result code ' + str(rc))
        client.subscribe(self.MQTT_TOPIC_COMMANDE, 1)
        client.subscribe(self.MQTT_TOPIC_CALIB, 1)

        self.msgConnexion.emit(True)

    def on_disconnect(self, client, userdata, flags):
        print('Disconnected from broker')
        self.msgConnexion.emit(False)

    def on_message(self, client, userdata, msg):
        # print(str(msg.topic))
        match msg.topic:
            case self.MQTT_TOPIC_COMMANDE:
                #if msg.topic == self.MQTT_TOPIC_COMMANDE:  # Message contenant la commande de l'ESP32
                self.msgCommandeAuto.emit(str(msg.payload))
                delai = f"{1000*(time.time() - self.delaiCommandeAuto):.1f}"
                self.msgDelaiCommandeAuto.emit(delai)

            case self.MQTT_TOPIC_CALIB:
                print(msg.payload)

            case self.MQTT_TOPIC_PUISSANCE:
                self.valEnergie.emit(int(1000 * (time.time() - self.delaiCommandeAuto)))
                self.delaiCommandeAuto = time.time()

            case _:
                print('Autre topic')
        #elif mst.topif =
        mqtt_client.publish('Eq7_PuissanceGant_S4/OpenCR/test', str("BENIS"), qos=0, retain=False)

    def envoyerCommandeManuelle(self, commande):
        msg = '<' + str(commande) + '>'
        mqtt_client.publish(self.MQTT_TOPIC_MANUEL_COMMANDE, msg, qos=1, retain=False)

    def envoyerManuelActif(self, actif):
        if actif:
            msg = '1'
        else:
            msg = '0'
        mqtt_client.publish(self.MQTT_TOPIC_MANUEL_ACTIF, msg, qos=2, retain=False)
    # Indiquer au serveur que l'arrêt d'urgence est activé
    def envoyerArretUrgenceActif(self, actif: bool):
        if actif:
            msg = '1'
        else:
            msg = '0'
        mqtt_client.publish(self.MQTT_TOPIC_ARRET_URGENCE_ACTIF, msg, qos=2, retain=False)
    # ----------------------------------------------------------------------------
    # main




#    if __name__ == '__main__':
#        print('MQTT to influxDB bridge')
#        main()
