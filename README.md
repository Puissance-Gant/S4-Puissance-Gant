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
