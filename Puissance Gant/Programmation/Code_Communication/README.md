# Architecture de la communication
Le projet Puissance Gant utilise les protocoles MQTT et sériel pour faire communiquer les divers éléments. Le schéma suivant illustre les divers liens entre les contrôleurs et ordinateurs ainsi que les protocoles utilisés.
![architecture_comm_2 drawio](https://user-images.githubusercontent.com/78489934/229170679-2ddff895-6e02-4001-a754-1ca2375afe2f.png)

# Communication sans-fil par MQTT
Cette section contient le processus pour l'installation et la configuration d'un serveur MQTT. MQTT est un protocole de communication sans fil utilisé dans les situation oû la bande passante est peu fiable ou faible. Le protocole MQTT utilisé est Eclipse Mosquitto. Il recommender d'installer le mqtt explorer afin de vérifier l'état du serveur. 

http://mqtt-explorer.com/

## Installation du serveur sur le raspberry pi

Afin d'installer le serveur sur le raspberry pi, il faut tout d'abord intaller un système d'operation. Le système d'operation recommandé est ubuntu server 22.10. 

https://ubuntu.com/download/server

Par la suite, il faut installer un serveur MQTT sur le raspberry pi. Pour l'installation, il est recommendé de suivre le tutoriel suivant: 

https://diyi0t.com/microcontroller-to-raspberry-pi-wifi-mqtt-communication/

Il d'ailleurs utile d'installer *MQTT Explorer*, qui est une application qui permet de visualiser et de gérer les messages en temps réel. Celà facilite le débogage et le développement d'applications MQTT.

http://mqtt-explorer.com/
