create_executable: client.o server.o
	gcc -o executable client.o server.o -lncurses
client.o: client.c header.h
	gcc -c client.c
server.o: server.c header.h
	gcc -c server.c
clear:
	rm executable client.o server.o