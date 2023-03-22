import paho.mqtt.client as mqtt
import serial
import time

MQTT_ADDRESS = '192.168.119.137'
MQTT_USER = 'puissance'
MQTT_PASSWORD = 'puissance'
MQTT_TOPIC = 'Eq7_PuissanceGant_S4/+/+'

startMarker = '<'
endMarker = '>'
dataStarted = False
dataBuf = ""
messageComplete = False

# ----------------------------------------------------------------------------
# Autre fonction 

def setupSerialOpenCR(baudRate, serialPortName):   
    global  serialPortOpenCR
    serialPortOpenCR = serial.Serial(port= serialPortName, baudrate = baudRate, timeout=0, rtscts=True)
    print("Serial port " + serialPortName + " opened  Baudrate " + str(baudRate))
    
def sendToOpenCR(stringToSend):

    # this adds the start- and end-markers before sending
    global startMarker, endMarker, serialPortOpenCR
    
    stringWithMarkers = (startMarker)
    stringWithMarkers += stringToSend
    stringWithMarkers += (endMarker)

    serialPortOpenCR.write(stringWithMarkers.encode('utf-8')) # encode needed for Python3

# ----------------------------------------------------------------------------
# Fonction MQTT 

def on_connect(client, userdata, flags, rc):
	print('Connected with result code'+ str(rc))
	client.subscribe(MQTT_TOPIC)

def on_message(client, userdata, msg):
	print(str(msg.payload))
	sendToOpenCR(str(msg.payload))

# ----------------------------------------------------------------------------
# main 


def main():
	mqtt_client = mqtt.Client()
	mqtt_client.username_pw_set(MQTT_USER, MQTT_PASSWORD)
	mqtt_client.on_connect = on_connect
	mqtt_client.on_message = on_message
        
	mqtt_client.connect(MQTT_ADDRESS, 1883)
	mqtt_client.loop_forever()


if __name__ == '__main__':
    setupSerialOpenCR(115200, "COM7")
    print('MQTT to influxDB bridge')
    main()
