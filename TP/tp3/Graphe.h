/*  INF3105 -TP3- Structures de données et algorithmes
     Julien Guité-Vinet 
     GUIJ09058407
*/

#if !defined(__GRAPHE_H__)
#define __GRAPHE_H__

#include <queue>
#include <map>
#include <iostream>
#include <vector>

using namespace std;

// Le type S est le type utilisé pour identifier les sommets
template <class S, class A>
class Graphe{
public:
    int nbr_sommets;
    // créer le graphe.
    void ajouterSommet(const S& s);
    void ajouterAreteOrientee(const S& s1, const S& s2,A& cost);
    void ajouterAreteNonOrientee(const S& s1, const S& s2,A& cost);

    void parcoursRechercheProfondeur(const S& s) const;
    void parcoursRechercheProfondeur2 (const S& s) const;
    void parcoursRechercheLargueur(const S& s) const;
    void extraireCompostantesConnexes() const;



private:
    struct Sommet{
        mutable bool visite;
        map<S,A> voisins; // ensemble des sommets accessibles via les arêtes sortantes du sommet.

    };
public:
    map<S, Sommet> sommets; // identification --> sommet

    class Iterateur {
    public:

        Iterateur(Graphe& g): iter(g.sommets.begin()){  courrant = iter->first;}
        Iterateur(Graphe& g, map<S, Sommet>& sommets) : iter(g.sommets.end()){ courrant = iter->first;}
        Iterateur(typename map<S,A>::iterator i) : iter(i){}
        int& operator*() {return courrant;}
        bool operator!=(const Iterateur& o) const  {return o.courrant != courrant;}

        Iterateur& operator++() {++iter;courrant = iter->first; return *this;}
        const S& sommet() {return iter->first;}
        A& valeur() {return iter->second;}
    private:
        typename map<S,Sommet>::iterator iter;
        S courrant;
    };

    Iterateur debut() { return Iterateur(*this); }
    Iterateur fin() { return Iterateur(*this, sommets);}
};

template <class S,class A>
void Graphe<S,A>::ajouterSommet(const S& s){
    sommets[s] = Sommet();
}

template <class S,class A>
void Graphe<S,A>::ajouterAreteNonOrientee(const S& s1, const S& s2, A& cost){
    sommets[s1].voisins.insert(s2,cost);
    sommets[s1].voisins.insert(s1,cost);

}

template <class S,class A>
void Graphe<S,A>::ajouterAreteOrientee(const S& s1, const S& s2,A& cost){
    sommets[s1].voisins.insert(pair<S,A>(s2,cost));

}

template <class S,class A>
void Graphe<S,A>::parcoursRechercheProfondeur(const S& s) const{
    for (typename map<S,Sommet>::const_iterator it = sommets.begin(); it!=sommets.end(); ++it)
        it->second.visite = false;
    parcoursRechercheProfondeur2(s);
    // Vous devrez implémenter des marqueurs «visité» sur les sommets.
}

template <class S,class A>
void Graphe<S,A>::parcoursRechercheProfondeur2(const S& s) const{
    const Sommet& sommet = sommets.at(s);
    if(sommet.visite) return;
    sommet.visite = true;
    cout << s << " ";
    for (typename map<S,A>::const_iterator it = sommet.voisins.begin(); it!=sommet.voisins.end(); ++it)
        parcoursRechercheProfondeur2(*it);
}

template <class S, class A>
void Graphe<S,A>::parcoursRechercheLargueur(const S& s) const{
    for (typename map<S,Sommet>::const_iterator it = sommets.begin(); it!=sommets.end(); ++it)
        it->second.visite = false;
    queue<S> file;
    file.push(s);
    while(!file.empty()) {
        S suivant = file.front();
        file.pop();
        const Sommet& sommet = sommets.at(suivant);
        if (sommet.visite) continue;
        cout << suivant << " ";
        sommet.visite = true;
        for (typename map<S,A>::const_iterator it = sommet.voisins.begin(); it!=sommet.voisins.end(); ++it)
            file.push(*it);
    }
    cout << endl;

}
template <class S,class A>
void Graphe<S,A>::extraireCompostantesConnexes() const{
    // À compléter.
    // Ne faites que l'extraction simple en premier (plus simple que l'extraction de composantes *fortement* connexes).
}

#endif
