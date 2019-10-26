all: sudoku

sudoku: main.o sudoku.o
	g++ -Wall -g main.o sudoku.o -o sudoku
main.o: main.cpp sudoku.h
	g++ -Wall -g -c main.cpp -o main.o
sudoku.o: sudoku.cpp sudoku.h
	g++ -Wall -g -c sudoku.cpp -o sudoku.o

clean:
	rm -f *.o
