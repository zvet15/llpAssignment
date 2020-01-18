build_all: debug release 
	
	

clean_all:
	-rm -rf debug
	-rm -rf release

rebuild_all: clean_all build_all

debug:
	-mkdir debug
	gcc -Wall -g -O0 server.c -o debug/server -lpthread
	gcc -Wall -g -O0 client.c -o debug/client 
	gcc -Wall -g -O0 client.c -DLOG -o debug/clientlog 

release:
	-mkdir release
	gcc -Wall server.c -o release/server -lpthread
	gcc -Wall client.c -o release/client 
	gcc -Wall client.c -DLOG -o release/clientlog 