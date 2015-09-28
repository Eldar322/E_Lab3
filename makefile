PROCESS=0

all:
	mkdir -p bin
	gcc -DPROCESS=$(PROCESS) -pthread src/server.c -o bin/server
	gcc src/client.c -o bin/client

clean:
	rm -rf bin