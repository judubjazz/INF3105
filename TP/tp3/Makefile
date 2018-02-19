#Makefile pour TP3
# Julien Guité-Vinet GUIJ 09058407



#Configuration standard
OPTIONS =

#Configuration avec debugage
#OPTIONS = -g -O0 -Wall

#Configuration avec optimisation
#OPTIONS = -O3 -Wall

all:tp3

tp3:main.cpp Graphe.h Univers.h
	$(CXX) $(OPTIONS) -std=c++11 -o tp3 main.cpp

graphe:Graphe.h
	$(CXX) $(OPTIONS) -std=c++11 -c -o Graphe.o Graphe.h

univers:Univers.h
	$(CXX) $(OPTIONS) -std=c++11 -c -o Univers.o Univers.h

clean :
	rm -f tp3
  
diff:
	diff resultat_obtenu.txt resultat_attendu.txt | grep ">" | sed 's/^> //g' > diff_file.txt



