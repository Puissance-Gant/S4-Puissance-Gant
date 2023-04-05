import paho.mqtt.client as mqtt

MQTT_ADDRESS = "192.168.137.254"
MQTT_USER = 'puissance'
MQTT_PASSWORD = 'puissance'
MQTT_TOPIC_RESIST = 'Eq7_PuissanceGant_S4/gant/resistance'
MQTT_TOPIC_CALIB = 'Eq7_PuissanceGant_S4/gant/reset'


# ----------------------------------------------------------------------------
# Fonction MQTT 

def on_connect(client, userdata, flags, rc):
    print('Connected with result code ' + str(rc))
    client.subscribe(MQTT_TOPIC_RESIST, 1)
    client.subscribe(MQTT_TOPIC_CALIB, 1)


def on_message(client, userdata, msg):
    # print(str(msg.topic))
    mqtt_client.publish('Eq7_PuissanceGant_S4/OpenCR/test', str("BENIS"), qos=0, retain=False)


# ----------------------------------------------------------------------------
# main 

def main():
    global mqtt_client

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
