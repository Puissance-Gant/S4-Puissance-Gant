# Communication sérielle et par MQTT de la main robotisée
Ces deux fichiers sont à placer dans un ordinateur auquel on a connecté une carte OpenCR qui contrôle la main robotisée.
Ensembles, ils permettent la communication entre la carte OpenCR et le serveur MQTT.

## commMQTT_OpenCR.py
Ce fichier de code est le fichier principal. Celui-ci lance la communication sans-fil avec le serveur MQTT. De plus, il peut appeler les fonctions de communication sérielle avec l'OpenCR afin d'envoyer et de recevoir des messages. On peut donc lire et envoyer des messages au serveur à partir de ce fichier.

Il faut installer la librairie MQTT avec la commande `pip install paho-mqtt` dans votre éditeur de code. Nous avons utilisé Visual Studio Code pour ce code, mais d'autres éditeurs sont compatibles aussi.

## LectureEcritureSerielOpenCR.py
Ce fichier contient les fonctions de communication sérielle avec l'OpenCR. La librairie pyserial est nécessaire pour son fonctionnement : `pip install pyserial`.
On peut ici lire et envoyer des messages à l'OpenCR. Une fonction importante est *verifierPosMoteurs( )*, qui diminue grandement la taille des messages envoyés en évitant d'envoyer une nouvelle position à l'OpenCR si celle-ci est à ±3% de la dernière valeur envoyée. De cela on peut filtrer les valeurs moins utilies.

L'optimisation des messages envoyés est nécessaire, car le *buffer* de lecture de l'OpenCR est très lent et bloque l'exécution du code lorsque la lecture se fait plus lentement que l'envoi des données. C'est d'ailleurs la zone qui ralentit le plus le temps de réaction global du système, puisqu'on force l'ESP32D du gant de contrôle à se limiter à des envois espacés de 125ms.
