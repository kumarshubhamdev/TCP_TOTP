TARGET:exe
exe: server.o 
	gcc server.o -o server.exe -L . lib.a -lm -lcrypto

 
 
server.o:server.c
	gcc -c server.c -o server.o