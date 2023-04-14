import paho.mqtt.client as mqtt
import EnvoiOpenCR_TestLudo as OpenCR

MQTT_ADDRESS = "192.168.137.254"
MQTT_USER = 'puissance'
MQTT_PASSWORD = 'puissance'

MQTT_TOPIC_COMMANDES_AUTO = 'Eq7_PuissanceGant_S4/gant/commandes'
MQTT_TOPIC_COMMANDE_MANUEL = 'Eq7_PuissanceGant_S4/interface/commande'
MQTT_TOPIC_MANUEL_ACTIF = 'Eq7_PuissanceGant_S4/interface/manuel_actif'
MQTT_TOPIC_ARRET_URGENCE_ACTIF = 'Eq7_PuissanceGant_S4/interface/arret_urgence_actif'
MQTT_TOPIC_PUISSANCE = 'Eq7_PuissanceGant_S4/OpenCR/puissance'

modeManuelActif = True
arretUrgenceActif = False
#MQTT_TOPIC_COMMANDES_AUTO2 = 'Eq7_PuissanceGant_S4/gant/reset'

# ----------------------------------------------------------------------------
# Fonction MQTT 

def on_connect(client, userdata, flags, rc):
	print('Connected with result code ' + str(rc))
	client.subscribe(MQTT_TOPIC_COMMANDES_AUTO, 1)
	client.subscribe(MQTT_TOPIC_COMMANDE_MANUEL, 1)
	client.subscribe(MQTT_TOPIC_MANUEL_ACTIF, 1)
	client.subscribe(MQTT_TOPIC_ARRET_URGENCE_ACTIF, 1)



def on_message(client, userdata, msg):
	global MQTT_TOPIC_COMMANDES_AUTO, modeManuelActif, arretUrgenceActif
	msgOpenCR = OpenCR.recvFromOpenCR()
	if not (msgOpenCR == 'XXX'): # Envoyer la puissance au serveur
		mqtt_client.publish(MQTT_TOPIC_PUISSANCE, msgOpenCR, qos=1, retain=False)
	topic = msg.topic
	if topic == MQTT_TOPIC_ARRET_URGENCE_ACTIF:
		arretUrgenceActif = True if str(msg.payload).find('1') != -1 else False
		msgArretUrgence = '<1U>' if arretUrgenceActif else '<0U>'
		OpenCR.sendToOpenCR(msgArretUrgence)

	elif topic == MQTT_TOPIC_MANUEL_ACTIF:
		modeManuelActif = True if str(msg.payload).find('1') != -1 else False

	elif topic == MQTT_TOPIC_COMMANDES_AUTO and not modeManuelActif and not arretUrgenceActif:
		OpenCR.sendToOpenCR(str(msg.payload))

	elif topic == MQTT_TOPIC_COMMANDE_MANUEL and modeManuelActif and not arretUrgenceActif:
		OpenCR.sendToOpenCR(str(msg.payload))

def on_disconnect(self, client, userdata, flags):
	print('Connexion au serveur perdue. Tentative de reconnexion en cours')
# ----------------------------------------------------------------------------
# main 

def main():

	global mqtt_client
	OpenCR.setupSerialOpenCR(256000, "COM10")

	mqtt_client = mqtt.Client("pc")
	mqtt_client.enable_logger()
	mqtt_client.username_pw_set(MQTT_USER, MQTT_PASSWORD)
	mqtt_client.on_connect = on_connect
	mqtt_client.on_message = on_message
	mqtt_client.on_disconnect = on_disconnect
    
	mqtt_client.connect_async(host=MQTT_ADDRESS, port=1883)
	mqtt_client.loop_forever()


if __name__ == '__main__':
    print('MQTT to influxDB bridge')
    main()
		
		


