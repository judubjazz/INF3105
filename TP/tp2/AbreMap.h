/*  INF3105 - Structures de données et algorithmes
    UQAM | Faculté des sciences | Département d'informatique
    Automne 2016 | TP2
    http://ericbeaudry.ca/INF3105/tp2/
*/
#if !defined(__ARBREMAP__H__)
#define __ARBREMAP__H__

#include "ArbreAVL.h"

template <class K, class V>
class ArbreMap
{

private:
    class Entree{
    public:
        Entree (const K& c):cle(c),valeur(){}
        K cle;
        V valeur;
        bool operator < (const Entree& e) const { return cle < e.cle; }
        bool operator == (const Entree& e) const { return cle == e.cle; }

    };
    ArbreAVL<Entree> entrees;
public:
    class Iterateur {
    public:
        Iterateur(ArbreMap& a) : iter(a.entrees.debut()) {}
        Iterateur(typename ArbreAVL<Entree>::Iterateur i) : iter(i) {}
        operator bool() const {return iter.operator bool();};
        Iterateur& operator++() {++iter; return *this;}
        const K& cle() const {return (*iter).cle;}
        V& valeur() {return (V&) (*iter).valeur;}
    private:
        typename ArbreAVL<Entree>::Iterateur iter;
    };
    Iterateur debut() { return Iterateur(*this); }
    Iterateur fin() { return Iterateur(entrees.fin());}
    Iterateur rechercher(const K& cle) { return Iterateur(entrees.rechercher(cle));}
    Iterateur rechercherEgalOuSuivant(const K& cle) {return
                Iterateur(entrees.rechercherEgalOuSuivant(cle));}
    Iterateur rechercherEgalOuPrecedent(const K& cle) {return
                Iterateur(entrees.rechercherEgalOuPrecedent(cle));}
public:
    bool contient(const K&) const;

    void enlever(const K&);
    void vider();
    bool vide() const;

    const V& operator[] (const K&) const;
    V& operator[] (const K&);

};
/**
 * Vide l'arbreMap
 * @tparam K la cle
 * @tparam V la valeur de la cle
 */
template <class K, class V>
void ArbreMap<K,V>::vider(){
    entrees.vider();
}

/**
 * retourne vrai si l'arbreMap est vide, faux sinon
 * @tparam K
 * @tparam V
 * @return vrai si vide, faux sinon
 */
template <class K, class V>
bool ArbreMap<K,V>::vide() const{

    return entrees.vide();
}

/**
 *
 * @tparam K
 * @tparam V
 * @param c
 */
template <class K, class V>
void ArbreMap<K,V>::enlever(const K& c)
{
    // À compléter
}

/**
 * retourne vrai si K est dans l'arbre
 * @tparam K
 * @tparam V
 * @param c
 * @return vrai si Kest dans l'arbre, faux sinon
 */
template <class K, class V>
bool ArbreMap<K,V>::contient(const K& c) const
{
    return entrees.contient(Entree(c)) != NULL;
}

/**
 * Positionne l'iterateur dans l'arbre à l'endroit de la clé recherchée
 * retourne la valeur de cette clé
 * ne peut modifier la valeur
 * @tparam K
 * @tparam V
 * @param c
 * @return la valeur de la clé recherché dans []
 */
template <class K, class V>
const V& ArbreMap<K,V>::operator[] (const K& c)  const
{
    typename ArbreAVL<Entree>::Iterateur iter = entrees.rechercher(c);
    return entrees[iter].valeur;
}

/**
 * Positionne l'iterateur dans l'arbre à l'endroit de la clé recherchée
 * retourne la valeur de cette clé
 * peut modifier la valeur
 * @tparam K
 * @tparam V
 * @param c
 * @return
 */
template <class K, class V>
V& ArbreMap<K,V>::operator[] (const K& c)
{
    typename ArbreAVL<Entree>::Iterateur iter = entrees.rechercher(c);
    if(!iter){
        entrees.inserer(Entree(c));
        iter = entrees.rechercher(c);
// Choix alternatif : creation explicite (Ex: TreeMap.put() en Java).
    }
    V test = entrees[iter].valeur;
    return entrees[iter].valeur;
}

#endif

