# Interface utilisateur
Cette interface est à lancer sur un appareil indépendant de la main et du gant. L'équipe a choisi d'utiliser un ordinateur portable et Python 3.10 avec l'éditeur de code PyCharm. Visual Studio Code reconnaissait mal la librairie utilisée.

Pour utiliser l'interface actuelle, aller à la section **Utiliser l'interface actuelle** plus bas.

## Installer PyQt5
La programmation de l'interface est plus aisée à faire sur un ordinateur avec Windows. Python 3.10 est la version choisie, elle est disponible sur le *Microsoft Store*. Il sera aussi nécessaire d'installer la librairie PyQt5. Sur Windows, ouvrir un terminal (cmd dans la barre de recherche Windows) et taper `pip install pyqt5`, puis `pip install pyqt5-tools`. 

## Installer Qt Designer
Suivre [ce lien](https://build-system.fman.io/qt-designer-download) pour installer Qt Designer. Ce programme permet de créer l'interface de manière visuelle et d'implémenter un peu de logique directement. La plus grande partie de la programmation de la logique devra être faite par après, cependant.

## Créer le code python de l'interface
Pour exporter votre interface en code Python, vous devrez ouvrir un terminal dans le dossier où le fichier .ui de votre interface se trouve (taper cmd dans le chemin de l'explorateur de fichiers). Ensuite, écrire `pyuic5 NomFicher.ui -o NomFicher.py` dans le terminal. Cela va créer un fichier python à partir de votre fichier .ui, qui lorsque lancé, donnera le résultat exact que vous avez sur Qt Designer. 

Un problème que nous avons rencontré est que la commande ne va pas trouver l'exécutable de pyuic5 dans les dossiers d'installation de python. Le même problème peut se produire si vous avez des compléments différents de Qt (par exemple des icônes). Il faut alors aller chercher l'exécutable dans les fichiers de la librairie (dossier %appdata%, caché profondément dans les dossiers de votre version de python.)

## Utiliser l'interface actuelle
Le fichier *Programme_InterfacePuissanceGant* contient la logique des interactions entre les boutons, les affichages et le code de communication. Lancer ce fichier à partir de PyCharm ou VS Code lance ouvre l'interface.

![image](https://user-images.githubusercontent.com/78489934/232552940-eb062670-31dc-4013-81fb-422f7f5985a5.png)

L'interface contient plusieurs sections, notamment pour afficher la position des moteurs, l'état de la communication, le panneau du mode manuel ainsi que la consommation énergétique. 

Il est important de noter que l'interface du projet Puissance Gant requiert la [connexion au serveur MQTT](https://github.com/Puissance-Gant/S4-Puissance-Gant/tree/main/Puissance%20Gant/Programmation/Code_Communication). Sans cela, lancer le fichier créera une multitude d'erreur. Cela se fait actuellement dans le fichier commInterface.py.
