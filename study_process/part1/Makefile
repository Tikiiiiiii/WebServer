.PHONY:server client clean all
server:
	g++ util.cpp client.cpp -o ../client 

client:
	g++ util.cpp server.cpp -o ../server

clean:
	rm ../server && rm ../client

all: server client