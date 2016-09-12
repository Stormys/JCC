CC = g++ -g -std=c++11
OBJ = Scanner.o Token.o Compiler.o

all: $(OBJ)
	$(CC) -o JCC $(OBJ) 

Compiler.o: src/Compiler.cpp src/Compiler.h
	$(CC) src/Compiler.cpp -c

Token.o: src/Token.cpp src/Token.h
	$(CC) src/Token.cpp -c

Scanner.o: src/Scanner.cpp src/Scanner.h src/Token.h src/Token.cpp
	$(CC) src/Scanner.cpp -c
	
clean: 
	rm $(OBJ)
