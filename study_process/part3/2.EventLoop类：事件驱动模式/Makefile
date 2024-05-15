BUILD_DIR=./build
SRC=src
.PHONY:server client clean all
server:
	g++ $(SRC)/util.cpp server.cpp $(SRC)/Epoll.cpp $(SRC)/InetAddress.cpp $(SRC)/Socket.cpp  \
	$(SRC)/Channel.cpp $(SRC)/EventLoop.cpp $(SRC)/Server.cpp -o $(BUILD_DIR)/server 

client:
	g++ $(SRC)/util.cpp client.cpp -o $(BUILD_DIR)/client 

clean:
	rm $(BUILD_DIR)/server && rm $(BUILD_DIR)/client

all: server client
