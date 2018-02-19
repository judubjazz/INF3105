/*  INF3105 -TP3- Structures de données et algorithmes
     Julien Guité-Vinet 
     GUIJ09058407
*/

#if !defined(__UNIVERS_H__)
#define __UNIVERS_H__

#include <queue>
#include <map>
#include <iostream>
#include <vector>
#include <assert.h>

using namespace std;

// Le type S est le type utilisé pour identifier les sommets
template <class U,class C>
class Univers{
public:
    Univers();
    Univers(const U& largeur,const  U& longeur);
    ~Univers();

public:
    int nbr_porte;
    U largeur;
    U longeur;
    pair<int,int>  porte;
    pair<int, int> tresor_1;
    pair<int, int> tresor_2;
    pair<int, int> tresor_3;
    char** univers;
    C** distancesfinal;
    C** distances;
};

template <class U, class C>
Univers<U,C>::Univers(const U& largeur, const U& longeur){
    this->largeur = largeur;
    this->longeur = longeur;
    this->nbr_porte = 0;
    // alloue la longeur
    univers = new char * [longeur];
    distances = new C* [longeur];
    distancesfinal = new C*[longeur*largeur];
    // vérifie l'allocation
    if (univers && distances) {
        for (int i = 0; i < longeur; i++) {
            // alloue la largeur
            univers[i] = new char [largeur];
            distances[i] = new C [largeur];
            // verifie l'allocation
            assert(distances[i]);
            assert(univers[i]);
        }
    }
    if (distancesfinal){
        for (int i = 0; i < longeur * largeur ; i++){
            distancesfinal[i] = new C [largeur*longeur];
            assert(distancesfinal[i]);
        }
    }
}
template <class U, class C>
Univers<U,C>::~Univers(){
/*    for (int i = 0; i < longeur * largeur ; ++i){
        delete [] distancesfinal[i];
    }
    for (int i = longeur; i > 0 ; --i) {
        delete [] distances[i];
        delete [] univers[i];
    }*/
    delete []univers;
    delete []distances;
    delete []distancesfinal;
}


#endif
