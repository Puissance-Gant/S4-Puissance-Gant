from PyQt5.QtCore import QThread, pyqtSignal
import paho.mqtt.client as mqtt
import time


class CommInterface(QThread):
    MQTT_ADDRESS = "192.168.137.254"
    MQTT_USER = 'puissance'
    MQTT_PASSWORD = 'puissance'
    MQTT_TOPIC_RESIST = 'Eq7_PuissanceGant_S4/gant/resistance'
    MQTT_TOPIC_CALIB = 'Eq7_PuissanceGant_S4/gant/reset'

    msgResist = pyqtSignal(str)
    msgDelaiResist = pyqtSignal(str)
    delaiResist = time.time()
    msgConnexion = pyqtSignal(bool)
    valEnergie = pyqtSignal(int)

    # ----------------------------------------------------------------------------
    # Fonction MQTT
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
        client.subscribe(self.MQTT_TOPIC_RESIST, 1)
        client.subscribe(self.MQTT_TOPIC_CALIB, 1)

        self.msgConnexion.emit(True)

    def on_disconnect(self, client, userdata, flags):
        print('Disconnected from broker')
        self.msgConnexion.emit(False)



    def on_message(self, client, userdata, msg):
        # print(str(msg.topic))
        if msg.topic == self.MQTT_TOPIC_RESIST:  # Message contenant la commande de l'ESP32
            self.msgResist.emit(str(msg.payload))
            delai = f"{1000*(time.time() - self.delaiResist):.1f}"
            self.msgDelaiResist.emit(delai + " ms")

            # Déplacer ce code lorsqu'on aura la communication avec l'OpenCR
            self.valEnergie.emit(int(1000*(time.time() - self.delaiResist)))
            self.delaiResist = time.time()
        mqtt_client.publish('Eq7_PuissanceGant_S4/OpenCR/test', str("BENIS"), qos=0, retain=False)


    # ----------------------------------------------------------------------------
    # main




#    if __name__ == '__main__':
#        print('MQTT to influxDB bridge')
#        main()
