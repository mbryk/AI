checkers: main.o game.o board.o player.o move.o
	g++ -o checkers main.o game.o board.o player.o move.o
	
main.o: main.cpp
	g++ -c main.cpp

game.o: game.cpp game.h
	g++ -c game.cpp

board.o: board.cpp board.h
	g++ -c board.cpp

player.o: player.cpp player.h
	g++ -c player.cpp

move.o: move.cpp move.h
	g++ -c move.cpp

debug:
	g++ -g -o checkDebug main.cpp game.cpp board.cpp player.cpp move.cpp

clean:
	rm -f *.out *.o *.stackdump *~

