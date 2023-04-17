import paho.mqtt.client as mqtt
import LectureEcritureOpenCR as OpenCR

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

# ----------------------------------------------------------------------------
# Fonctions MQTT

# on_connect est appelé lorsque le serveur est rejoint
def on_connect(client, userdata, flags, rc):
	print('Connected with result code ' + str(rc))

	# S'abonner aux topics lorsqu'on se connecte au serveur
	client.subscribe(MQTT_TOPIC_COMMANDES_AUTO, 1)
	client.subscribe(MQTT_TOPIC_COMMANDE_MANUEL, 1)
	client.subscribe(MQTT_TOPIC_MANUEL_ACTIF, 1)
	client.subscribe(MQTT_TOPIC_ARRET_URGENCE_ACTIF, 1)


# on_message est appelé lorsqu'un message est reçu et que le client y est abonné
def on_message(client, userdata, msg):
	global MQTT_TOPIC_COMMANDES_AUTO, modeManuelActif, arretUrgenceActif
	msgOpenCR = OpenCR.recvFromOpenCR()
	if not (msgOpenCR == 'XXX'): # Envoyer la puissance au serveur. C'est le seul message envoyé par l'OpenCR
		mqtt_client.publish(MQTT_TOPIC_PUISSANCE, msgOpenCR, qos=1, retain=False)

	topic = msg.topic
	if topic == MQTT_TOPIC_ARRET_URGENCE_ACTIF: # L'utilisateur a appuyé sur le bouton d'arrêt d'urgence de l'interface
		arretUrgenceActif = True if str(msg.payload).find('1') != -1 else False
		msgArretUrgence = '<1U>' if arretUrgenceActif else '<0U>'
		OpenCR.sendToOpenCR(msgArretUrgence)

	elif topic == MQTT_TOPIC_MANUEL_ACTIF: # L'utilisateur a appuyé sur le bouton du mode manuel/automatique
		modeManuelActif = True if str(msg.payload).find('1') != -1 else False

	elif topic == MQTT_TOPIC_COMMANDES_AUTO and not modeManuelActif and not arretUrgenceActif: # On a reçu une commande du gant
		OpenCR.sendToOpenCR(str(msg.payload))

	elif topic == MQTT_TOPIC_COMMANDE_MANUEL and modeManuelActif and not arretUrgenceActif: # On a reçu une commande de l'interface
		OpenCR.sendToOpenCR(str(msg.payload))

# on_disconnect est appelé lorsqu'on perd la connexion au serveur
def on_disconnect(self, client, userdata, flags):
	print('Connexion au serveur perdue. Tentative de reconnexion en cours')
# ----------------------------------------------------------------------------

def main():
	global mqtt_client

	# Démerrer la connexion sérielle avec l'OpenCR. Il faut modifier le port COM (aller dans le
	# gestionnaire de périphériques pour voir où est connecté l'OpenCR)
	OpenCR.setupSerialOpenCR(256000, "COM10")

	# Fonctions de configuration initiale de la connextion MQTT
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
		
		


