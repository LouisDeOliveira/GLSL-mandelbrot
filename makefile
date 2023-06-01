COMP=g++
FLAGS=-lsfml-graphics -lsfml-window -lsfml-system
INC=-IC:\SFML-2.5.1\include
LIB=-LC:\SFML-2.5.1\lib

all: main.cpp
	$(COMP) main.cpp -c $(INC) 
	$(COMP) main.o -o main.exe $(LIB) $(FLAGS)
