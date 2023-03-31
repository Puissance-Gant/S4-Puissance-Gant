import paho.mqtt.client as mqtt
import EnvoiOpenCR_TestLudo as OpenCR

MQTT_ADDRESS = "192.168.137.254"
MQTT_USER = 'puissance'
MQTT_PASSWORD = 'puissance'
MQTT_TOPIC = 'Eq7_PuissanceGant_S4/gant/resistance'
#MQTT_TOPIC2 = 'Eq7_PuissanceGant_S4/gant/reset'

# ----------------------------------------------------------------------------
# Fonction MQTT 

def on_connect(client, userdata, flags, rc):
	print('Connected with result code '+ str(rc))
	client.subscribe(MQTT_TOPIC, 1)
	#client.subscribe(MQTT_TOPIC2, 1)



def on_message(client, userdata, msg):
	#print(str(msg.topic))
	msgOpenCR = OpenCR.recvFromOpenCR()
	if not (msgOpenCR == 'XXX'):
		print("openCr : " + msgOpenCR)

	OpenCR.sendToOpenCR(str(msg.payload))
	mqtt_client.publish('Eq7_PuissanceGant_S4/OpenCR/test', str("BENIS"), qos=0, retain=False)

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
    
	mqtt_client.connect_async(host=MQTT_ADDRESS, port=1883)
	mqtt_client.loop_forever()


if __name__ == '__main__':
    print('MQTT to influxDB bridge')
    main()
		
		


