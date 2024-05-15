BUILD_DIR=./build
SRC=src
.PHONY:server client clean all
server:
	g++ $(SRC)/util.cpp server.cpp $(SRC)/Epoll.cpp $(SRC)/InetAddress.cpp $(SRC)/Socket.cpp $(SRC)/Connection.cpp \
	$(SRC)/Channel.cpp $(SRC)/EventLoop.cpp $(SRC)/Server.cpp $(SRC)/Acceptor.cpp \
	$(SRC)/Buffer.cpp -o $(BUILD_DIR)/server 

client:
	g++ $(SRC)/util.cpp $(SRC)/Buffer.cpp $(SRC)/Socket.cpp $(SRC)/InetAddress.cpp client.cpp -o $(BUILD_DIR)/client 

clean:
	rm $(BUILD_DIR)/server && rm $(BUILD_DIR)/client

all: server client
