# S4-Puissance-Gant

Voici le répertoire du projet de session de l'équipe Puissance Gant :gloves::robot:

## Membres
<pre>
- Comtois, Ludovic    (coml1502)
- Corduneanu, Andrei  (cora5428)
- Delev, Milan        (delm1301)
- Den Hartog, Daniel  (deno2501)
- Martin, Frédérick   (marf0702)
- Theoret, Marek      (them0901)
</pre>

## Objectif du projet
L'équipe Puissance Gant avait pour but de concevoir une main robotisée ainsi qu'un système de contrôle à distance intuitif. Le produit final consiste en une main à trois doigts indépendemment contrôlés par un gant auquel des potentiomètres coulissants ont été attaché. Une interface utilisateur permet d'observer l'état du robot et de le contrôler manuellement.

### Le projet en action


ajouter la vidéo


## Fonctionnement global
Le fonctionnement spécifique de chaque composante est décrite dans leur sous-dossier respectif.

Il y a trois composantes principales qui communiquent entre elles à l'aide d'un serveur de communication sans-fil par WI-FI. L'état du gant est lu par une carte ESP32 qui envoie la position des doigts au système. Un ordinateur reçoit ces informations et les transmet à une carte OpenCR qui contrôle les moteurs de la main. Un deuxième ordinateur affiche une interface utilisateur qui permet d'afficher des informations utiles et de contrôler la main manuellement.

### Mécanique du gant
![diag_mec](https://user-images.githubusercontent.com/106932488/232348632-7e09cc01-8458-43dc-8fac-c0af37f97980.png)

Description du montage:
- Chaque curseur de potentiomètre est attaché à un ressort le retenant à sa position zéro, soit vers l'arrière (vers le poignet).
- Chaque curseur est ensuite tiré par un câble, chacun parcourant au moins trois corridors d'alignement:
  - Au moins un corridor est placé sur le boitier, servant de premier point d'appui au câble pour ensuite se diriger vers le doigt assigné.
  - Au moins deux corridors sont placés sur chacun des doigts à mesurer et chacun est fixé sur un bague que l'utilisateur doit porter.
  - Les corridors sur les bagues distales servent de point d'ancrage au câble.
  - Plus de corridors peuvent être installés afin de maximiser la linéarité de la translation axiale du câble.
- En fléchissant les doigts, le câble est tiré, faisant varier la position du curseur du potentiomètre.
- Les ressorts doivent être suffisamment forts pour retenir les doigts au repos en position ouverte.

### Électronique du gant
![diag_elec](https://user-images.githubusercontent.com/106932488/232349645-7fee61fd-5cd1-4bf5-9e84-28c4cc82ab0e.png)

Description du montage:
La mesure de la position des doigts se fait par la lecture des diviseurs de tension de chaque doigt
- Une résistance fixe est placée en série à un potentiomètre. Ici, 10 k et 100 k ohms respectivement.
- On applique la tension (Vcc; 3,3 V) à la résistance fixe et on branche le point du curseur à la masse (GND)
- La borne extrême du potentiomètre (résistance totale fixe) peut ne pas être branchée, cependant:
  - En fonction du modèle de potentiomètre, il est possible de placer la résistance fixe entre les bornes non-variables pour concevoir le diviseur de  tension, rendant le montage plus compact.
  - Dans le cas décrit ci-haut, la résistance fixe doit être de 11k afin d'équivaloir, avec la 100 k du potentiomètre, à 10 k ohms (circuit parallèle).
- La borne Vout (point de lecture) est entre la résistance fixe et le potentiomètre.
- La borne Vout de chaque diviseur de tension est branchée directement en input au ESP32D aux points GPIO 36, 39 et 34 pour le pouce, l'index et le majeur respectivement.
- Les points du ESP32D peuvent varier en fonction de votre programme.


### Performance
La latence globale du contrôle, donc entre le mouvement de la main de l'utilisateur et le mouvement de la main est d'environ 200ms et est plutôt constant. La main peut d'ailleurs tenir une banane de 200g sans la faire tomber.

### Pistes d'amélioration
<pre>
- Changer la forme de la main robotisée et l'emplacement des doigts afin d'optimiser la prise d'objets. 
- Utiliser un meilleur système de tension pour la prise d'objets, qui empêcherait les doigts de bouger lors de la prise d'objets.
- Changer la carte OpenCR pour une carte qui peut se connecter au WI-FI. (Par exemple, un ESP32 et une carte-soeur de contrôle des moteurs)
- Rendre le serveur accessible sur tous les réseaux. Les appareils doivent actuellement être sur le même réseau WI-FI que le serveur pour s'y connecter.
- Changer le système d'acquisition de données du gant. Par exemple, un exosquelette. (Faire attention aux résistances flexibles, elles sont souvent peu fiables.)
</pre>
