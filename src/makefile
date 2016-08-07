CC = g++ -std=c++11
OBJ = Scanner.o Token.o Compiler.o

all: $(OBJ)
	$(CC) -o JCC $(OBJ)

Compiler.o: Compiler.cpp Compiler.h
	$(CC) Compiler.cpp -c

Token.o: Token.cpp Token.h
	$(CC) Token.cpp -c

Scanner.o: Scanner.cpp Scanner.h
	$(CC) Scanner.cpp -c
	
clean: 
	rm $(OBJ) *.i a.out
