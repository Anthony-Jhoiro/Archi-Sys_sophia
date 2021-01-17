# Projet : Architecture système

> Trouvez la dernière mise à jour du projet ici : [https://github.com/Anthony-Jhoiro/Archi-Sys_sophia]

<!-- TODO : Introduction -->

---

## Structure de fichier

```
.
├── deamon
│   ├── deamon.c            # Fichier source principal pour le démon
│   ├── deamon.h            # Header pour les fonctions du démon
│   └── Makefile            # Makefile pour compiler le démon
├── invoker
│   ├── help.txt            # Texte à afficher avec la commande --help
│   ├── invoker.c           # Fichier source pricipal pour l'invocateur
│   ├── invoker.h           # Header pour l'invocateur
│   └── Makefile            # Makefile pour compiler l'invocateur
├── rapport.md              # Version Markdown de ce rapport
├── rapport.pdf             # Version PDF de ce rapport
├── README.md               # README du projet
└── tools
    ├── constants.h         # Constantes du projets
    ├── fifo_tools.c        # Sources pour les outils sur les fifos
    ├── log_tools.c         # Sources pour les outils sur les logs
    ├── Makefile            # Makefile pour les tools
    ├── tools.c             # Sources principale pour les outils
    └── tools.h             # Header pour les outils
```

Le dossier tools contient tout le code succeptible d'être utilisé par les 2 programmes.

## Fonctionnement principale

L'utilisateur va directement interagir avec le programme invocateur. Il va pouvoir executer différentes actions pour controller le démon ainsi qu'executer diverses actions sur le temps.

La communications avec le démon se fait par une fifo dont le nom est définit dans `tools/constants.h`.

### Listener

Quand le programme invocateur est en écoute, un timer est lancé en même temps. Il permet de renvoyer une erreur dans le cas où la fifo existe mais que le démon ne répond pas.

:[listener_diagramm](diagramms/listener_timeout.puml)
