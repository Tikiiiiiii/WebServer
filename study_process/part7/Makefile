BUILD_DIR=./build
SRC=$(wildcard src/*.cpp)
G++FLAGS= -pthread
BUILD= -o $(BUILD_DIR)/server

.PHONY:server client clean all test
server:
	g++ $(G++FLAGS) $(SRC) server.cpp $(BUILD) 

client:
	g++ src/util.cpp src/Buffer.cpp src/Socket.cpp client.cpp -o $(BUILD_DIR)/client 

clean:
	rm $(BUILD_DIR)/server && rm $(BUILD_DIR)/client && rm $(BUILD_DIR)/test

test:
	g++ src/util.cpp src/Buffer.cpp src/Socket.cpp src/ThreadPool.cpp \
	$(G++FLAGS) test.cpp -o $(BUILD_DIR)/test

all: server client test
