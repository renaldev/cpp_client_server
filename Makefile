all: run_server run_client

run_client: run_client.cpp
	g++ -std=c++11 run_client.cpp -o run_client 

run_server: run_server.o server.o
	g++ run_server.o server.o -o run_server

run_server.o: run_server.cpp server.h
	g++ -std=c++11 -c run_server.cpp

server.o: server.cpp server.h
	g++ -std=c++11 -c server.cpp
