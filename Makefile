FLAGS=-lncurses -lpthread -std=c++11

all: model

trabalho1: main.cpp oo_model.cpp oo_model.hpp
	g++ -otrabalho1 main.cpp oo_model.cpp $(FLAGS)
	
server: server.cpp oo_model.cpp oo_model.hpp
	g++ -oserver server.cpp oo_model.cpp $(FLAGS)

client: client.cpp oo_model_client.cpp oo_model_client.hpp
	g++ -oclient client.cpp oo_model_client.cpp $(FLAGS)
