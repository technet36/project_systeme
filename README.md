# Système et Réseaux - Projet Introduction au Système
L3 - MIAGE 19/10/2018   
Maxence Morand     
Thiam Cheikh    

## Compilation
1. Assurer-vous que vous ayez tout les fichiers necessaires
  * structure.h
  * server.c
  * library.h
  * library.c
  * io.h
  * io.c
  * player.h
  * player.c  
vous pouvez executer le fichier "game"
2. Si vous avez le fichier ```makefile``` vous pouvez executer la commande make  
sinon vous pouvez utilisesr la commande  
```gcc server.c library.c io.c player.c -o game```  
  
## Exécution  
Apres avoir lancé le programme il faut entrée successivement le noms des quatres joueurs.  
*L'affichage depuis les fils rend le prompt peu lisible, c'est un vestige du programme quand il les joueurs avaient leur propre terminale*  
  
Ensuite chaque tours suit le même patern:  
* ENTRER pour lancer le dé  
* Choix du cheval [0,4]  
