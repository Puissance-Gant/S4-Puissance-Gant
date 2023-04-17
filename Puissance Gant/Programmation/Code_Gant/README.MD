# Code du gant de contrôle
Cette section présente le projet PlatformIO qui concerne l'acquisition de données et la communication du gant à partir d'un ESP32.

## Acquisition de données

L'acquistion de donnée ce fait avec l'aide d'un gant et de résistance coulissante. Un microcontroleur ESP32-D et des résistance coulissante. Les résistances 



## Communication

Le code de l'expressif 32D permet de faire la communication entre le serveur et les résistances du gant. Il y a une structure de donnée qui s'appelle Hand. Cette structure contient le numéro de pin des différents doigts, la valeur des doigts et la valeur minumum et maximum des résistance. 

Le code commence par une lecture des pin du pouce, de l'index et du groupe de doigt avec la fonction analogRead(). Ceci sort une valeur entre 0 et 4096 pour chacun des doigts. Ensuite, les valeurs des résistance sont mis dans une variable String. Le format de la variable resemble à ceci:

""<0A0B0C>""

Le caractère < indique le début du message, la valeur avant A est la valeur en pourcentage du déplacement du pouce, la valeur avant B est la valeur en pourcentage  du déplacement de l'index, la valeur avant C est la valeur en pourcentage  du déplacement du groupe et le caractère > indique la fin du message. 

Finalement, le message est envoyer au topic "Eq7_PuissanceGant_S4/gant/commandes" dans le serveur MQTT. 
