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
L'équipe Puissance Gant avait pour but de concevoir une main robotisée ainsi qu'un système de contrôle à distance intuitif. Le produit final consiste en une main à trois doigts indépendemment contrôlés par un gant auquel des résistances flexibles et une centrale inertielle (*IMU*) sont attachées.

Une carte ESP32 récolte les informations des capteurs et les envoie vers le Raspberry Pi par communication sans fil (protocole MQTT). Un ordinateur reçoit les données à distance et les transfère à une carte OpenCR par protocole sériel. Les moteurs Dynamixel sont ensuite contrôlés par la carte OpenCR et font bouger la main avec une latence minimale. Une interface fonctionne en parallèle afin d'afficher l'état du système et d'envoyer des commandes manuelles à l'OpenCR.

Le protocole MQTT permet l'utilisation de la main indépendemment de la distance, car il ne faut qu'une connexion internet pour transférer les informations.

***AJOUTER DES INFORMATIONS SUR LA PERFORMANCE DE LA MAIN***
***EX : MASSE TENUE, TEMPS DE RÉACTION ETC.***

## Le projet en action

*To do : ajouter des photos / vidéos du projet*
