server:
	gcc server.c -o server

client:
	gcc -I/usr/include/python3.10 client.c -o client -L/usr/lib -lpython3.10

clean:
	rm server
	rm client

all: server client
