
# Projet Système et Réseaux - L3 MIAGE
# Maxence Morand
# Thiam Cheikh

all : server.o library.o io.o player.o
	gcc server.o library.o player.o io.o -o game

server.o :   server.c structures.h
	gcc -c server.c

library.o : library.c library.h  structures.h
	gcc -c library.c

io.o : io.c io.h
	gcc -c io.c

player.o : player.c player.h io.c io.h structures.h
	gcc -c player.c io.c