CC = g++ -g -std=c++11
OBJ = Scanner.o Token.o Compiler.o NonTerminal.o Parser.o

all: $(OBJ)
	$(CC) -o jcc $(OBJ) 

Compiler.o: src/Compiler.cpp src/Compiler.h
	$(CC) src/Compiler.cpp -c

Token.o: src/Token.cpp src/Token.h
	$(CC) src/Token.cpp -c

NonTerminal.o: src/NonTerminal.h src/NonTerminal.cpp
	$(CC) src/NonTerminal.cpp -c

Parser.o: src/Parser.cpp src/Parser.h src/NonTerminal.h src/NonTerminal.cpp
	$(CC) src/Parser.cpp -c

Scanner.o: src/Scanner.cpp src/Scanner.h src/Token.h src/Token.cpp
	$(CC) src/Scanner.cpp -c
	
clean: 
	rm $(OBJ)
