Voici la section de code de l'interface utilisateur

Les objets de l'interface ont un préfixe indiquant leur type et un nom indiquant leur utilité (ex. Texte_AngleIndex est l'objet de boîte de texte qui sert à indiquer l'angle du moteur de l'index)

pyuic5 InterfacePuissanceGant.ui -o InterfacePuissanceGant.py //Mettre le programme qt designer en programme python (ne pas modifier le programme python)

pyrcc5 icones.qrc -o icones_rc.py //transformer le qrc d'icones de qt designer en ficher .py (si modifié) -> mettre le .py dans le dossier général

Pour le graphique, aller voir le tutoriel dans les favoris