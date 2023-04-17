# Code du gant de contrôle
Cette section présente le projet PlatformIO qui concerne l'acquisition de données et la communication du gant à partir d'un ESP32.

## Acquisition de données
Une structure nommée *Hand* contient les numéros de pin ainsi que la plage de calibration des potentiomètres.

L'acquistion des données se fait avec l'aide de potentiomètres coulissants. Un microcontroleur ESP32-D et lit un diviseur de tension et la fonction analogRead(), qui retourne une valeur entre 0 et 4096. Une plage a dû être calibrée afin que le maximum et le minimum des potentiomètres équivalent aux valeurs voulues de positionnement des doigts de la main. Ensuite, les valeurs des résistance sont transformées en pourcentage et sont mises dans une variable String. Le format de la variable resemble à ceci:

<0A0B0C>

Le caractère < indique le début du message, la valeur avant A est la valeur en pourcentage du déplacement du pouce, la valeur avant B est la valeur en pourcentage  du déplacement de l'index, la valeur avant C est la valeur en pourcentage du déplacement du groupe et le caractère > indique la fin du message. 

Il est important de noter que le prototype de gant ne peut lire que la position des doigts. Le poignet ne peut être contrôlé qu'en mode manuel à partir de l'interface. Cependant, ajouter cette fonctionnalité dans le code (par exemple, avec un autre potentiomètre) serait simple.

## Communication
Le programme installé sur l'ESP32D initialise une connexion au serveur MQTT dès qu'une source d'alimentation y est connecté. Une fois la connexion établie, l'acquisition de données est démarrée en boucle.

Après chaque lecture de données de positionnement, on envoie le message au serveur MQTT selon le bon format, puis on attend 125ms. Ce délai a été établi afin d'éviter de surcharger la communication sérielle de l'OpenCR au niveau de la main robotisée. Sans ce bouchon de communication, on pourrait envoyer les nouvelles données à chaque 40ms environ.
