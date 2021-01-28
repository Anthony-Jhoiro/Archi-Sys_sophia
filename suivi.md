# Difficultés rencontrées

## Mise en pratique du cours

Nous avons eu des soucis pour utiliser les fifo, dans le cours, fprintf et fscanf sont utilisé, mais cela ne marchait pas chez nous, nous avons donc du utilliser read et write.

## Problème d'écoute sur la fifo

Puisqu'il ne peut pas y avoir 2 écouteurs sur une même fifo (or le deamon et l'invoker ecoute sur la même fifo), nous avions mis un timer de 200 µs pour la réponse du deamon, le temps que le deamon ferme la fifo avant que l'invoker écoute. Nous avons pu corriger celà, en fermant l'écoute du deamon, juste après la lecture, et pas au moment d'envoyer la réponse.

