#Makefile pour TP2
# Julien Guité-Vinet GUIJ 09058407
# Bruno Gillard-Ouellette GILB 06058305


#Configuration standard
OPTIONS =

#Configuration avec debugage
#OPTIONS = -g -O0 -Wall

#Configuration avec optimisation
#OPTIONS = -O3 -Wall

all:tp2

tp2:main.cpp Pile.h ArbreAVL.h ArbreMap.h 
	$(CXX) $(OPTIONS) -std=c++11 -o tp2 main.cpp


ArbreAVL:ArbreAVL.h Pile.h 
	$(CXX) $(OPTIONS) -std=c++11 -c -o ArbreAVL.o ArbreAVL.h

Pile:  Pile.h
	$(CXX) $(OPTIONS) -std=c++11 -c -o Pile.o Pile.h

ArbreMap:ArbreMap.h
	$(CXX) $(OPTIONS) -std=c++11 -c -o ArbreMap.o ArbreMap.h


clean :
	rm -f Pile.o
	rm -f ArbreAVL.o
	rm -f ArbreMap.o
	rm -f main.o
	rm -f ArbreAVL.h.gch
	rm -f ArbreMap.h.gch
	rm -f Pile.h.gch
	rm -f ArbreAvlTest.o
	rm -f ArbreMapTest.o
	rm -f tp2

diff:
	diff resultat_obtenu.txt resultat_attendu.txt | grep ">" | sed 's/^> //g' > diff_file.txt
