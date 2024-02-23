.PHONY: build run_cl run_server

build: clean-build
	mkdir build
	git submodule init
	git submodule update
	cd ./build; make all

run_cl:
	./build/client

run_server:
	./build/server

clean-build:
	rm -rf ./build/
	
all: server client 

server:
	g++ server.cpp -o server 

client:
	g++ client.cpp -o client 