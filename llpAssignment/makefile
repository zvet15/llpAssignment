all: clean client server

client:
	gcc -Wall client.c -o client 

server:
	gcc -Wall -g server.c -o server -lpthread

clean:
	-rm client server
