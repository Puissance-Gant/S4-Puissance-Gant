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

### Photos de montage complété
Bien que rudimentaire, le montage illustré ci-contre sert de référence pour l'assemblage. À noter que les tiges des curseurs traversent le plafond du boitier dans une fente de la langueur du parcours du curseur et que les poteniomètres sont boulonnés par le haut.
![gant_montage02](https://user-images.githubusercontent.com/106932488/232541479-90515e92-f242-44ab-a620-5a7b31f6410a.jpg)

![gant_montage01](https://user-images.githubusercontent.com/106932488/232541500-479af071-c367-46f8-825e-dd28737307af.jpg)

