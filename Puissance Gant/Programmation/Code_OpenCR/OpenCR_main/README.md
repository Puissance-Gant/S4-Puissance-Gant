# Code de l'OpenCR
Cette section indique comment faire fonctionner le code de l'OpenCR et inclut les étapes d'installation d'ArduinoIDE et des librairies nécessaires.

## openCR_main.ino
Ce fichier est le code principal de l'OpenCR. Celui-ci doit être ouvert avec l'ArduinoIDE (nous avons utilisé la version 2.0.0). Suivre le [guide d'installation d'ArduinoIDE pour l'OpenCR.](https://emanual.robotis.com/docs/en/parts/controller/opencr10/#install-on-windows)
Ce code prépare les moteurs à l'utilisation et démarre la communication. Peu de logique y est présente. Vous pourrez changer les identifiants des moteurs selon les vôtres (voir la section contrôleMoteurs pour plus de détails sur les identifiants)

## lectureEtEcritureSerielle.hpp
Ce fichier est une librairie personnelle contenant les commandes de communication sérielle entre l'OpenCR et l'appareil qui lui est connecté. La base de la logique provient de [ce lien](https://forum.arduino.cc/t/pc-arduino-comms-using-python-updated/574496) et a été adaptée aux besoins du projet. Nous utilisons un système de caractères de départ/fin de message et de caractères de variables. Un message tel que <125A35B> indiquerait que la valeur 125 est à envoyer au "cas" A et la valeur 35 au "cas" B. Les valeurs peuvent représenter une position de moteur, une vitesse ou autre dépendant du cas choisi. Ces cas sont décrits dans le code.

## controleMoteurs.hpp
Cette librairie contient le code de contrôle des moteurs. La librairie **Dynamixel2Arduino** par Robotis est à installer à partir du *Arduino library manager* pour faire fonctionner les moteurs.
Il est à noter que les moteurs ont un identifiant indivuel (ID) qui permet leur utilisation en *daisy-chain*. Afin de modifier leur ID, il faut télécharger le **Dynamixel Wizard 2.0**(DW) et téléverser le code de communication USB sur l'OpenCR (File\Examples\OpenCR\10 etc.\usb_to_dxl dans ArduinoIDE). En connectant un moteur à la fois à l'OpenCR, on peut lancer une recherche (scan) dans le DW. Pour accélérer la recherche, il est possible de changer les paramètre pour chercher seulement des moteurs avec le protocole 2.0 et un *baud rate* de 57000. Une fois le moteur trouvé, vous pouvez changer son ID. Nous avons utilisé les deux derniers chiffres du numéro de série du moteur, par exemple. 
