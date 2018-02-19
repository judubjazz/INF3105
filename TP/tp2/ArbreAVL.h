/*  INF3105 | Structures de données et algorithmes
 *  UQAM | Département d'informatique | Automne 2016
 *  ArbreAVL pour le TP2
 *  Julien Guité-Vinet GUIJ 09058407
 *  Bruno Gillard-Ouellette GILB 06058305
 */
#if !defined(__ARBREAVL__H__)
#define __ARBREAVL__H__
#include <assert.h>
#include <stdio.h>
#include "Pile.h"
#include <vector>
#include <string>

using namespace std;

template <class T>
class ArbreAVL
{
public:
    //Constructeurs et destructeur.
    ArbreAVL();
    ArbreAVL(const ArbreAVL&);
    ~ArbreAVL();

    //Fonction pour manipuler l'arbre.
    bool contient(const T&) const;

    int  hauteur() const;
    void inserer(const T&);
    bool vide() const;
    void vider();
    void enlever(const T&); // non requis pour le TP2.

    // Annonce l'existance d'une classe Iterateur.
    class Iterateur;

    // Fonction pour obtenir un itérateur (position dans l'arbre)
    Iterateur debut() const;
    Iterateur fin() const;
    Iterateur rechercher(const T&) const;
    Iterateur rechercherEgalOuSuivant(const T&) const;
    Iterateur rechercherEgalOuPrecedent(const T&) const;

    // Accès aux éléments de l'arbre via un intérateur.
    const T& operator[](const Iterateur&) const;
    T& operator[](const Iterateur&);

    // Copie d'un arbre AVL.
    ArbreAVL& operator = (const ArbreAVL&);

private:
    struct Noeud{
        Noeud(const T&);
        T contenu;
        int equilibre;
        Noeud *gauche;
        Noeud *droite;
    };
    Noeud* racine;

    // Fonctions internes.
    bool contient(const T&, Noeud*&) const;
    bool inserer(Noeud*&, const T&);
    void rotationGaucheDroite(Noeud*&);
    void rotationDroiteGauche(Noeud*&);
    void vider(Noeud*&);
    void copierNoeud(Noeud*&,Noeud*&);
    void copier(const Noeud*, Noeud*&) const;
    const T& max(Noeud*) const;
    bool enlever(Noeud*&, const T& e);

public:

    class Iterateur{
    public:
        Iterateur(const ArbreAVL& a);
        Iterateur(const Iterateur& a);
        Iterateur(const ArbreAVL& a, Noeud* c);

        operator bool() const;
        bool operator!() const;
        bool operator==(const Iterateur&) const;
        bool operator!=(const Iterateur&) const;

        const T& operator*() const;


        Iterateur& operator++();
        Iterateur operator++(int);
        Iterateur& operator = (const Iterateur&);
    private:
        const ArbreAVL& arbre_associe;
        Noeud* courant;
        Pile<Noeud*> chemin;

        friend class ArbreAVL;
    };
};


//-----------------------------------------------------------------------------

#include "ArbreAVL.h"

/**
 * affecte c au contenu
 * @tparam T
 * @param c
 */
template <class T>
ArbreAVL<T>::Noeud::Noeud(const T& c):
        gauche(NULL), droite(NULL), equilibre(0), contenu(c)
{
}

/**
 * construit un noeud
 * @tparam T
 */
template <class T>
ArbreAVL<T>::ArbreAVL() : racine(NULL)
{
}

/**
 * constructeur par copie
 * @tparam T
 * @param autre
 */
template <class T>
ArbreAVL<T>::ArbreAVL(const ArbreAVL<T>& autre) : racine(NULL)
{
    copier(autre.racine, racine);
}

/**
 * destructeur
 * @tparam T
 */
template <class T>
ArbreAVL<T>::~ArbreAVL()
{
    vider(racine);
}

/**
 * parcours l'arbre a la recherche d'un élément, si plus petit à gauche. sinon à droite
 * @tparam T
 * @param element
 * @return vrai si l'élément est dans l'arbre, faux sinon
 */
template <class T>
bool ArbreAVL<T>::contient(const T& element) const
{
    bool est_dans_larbre = false;
    if (racine) {
        Noeud* temp = racine;
        while (temp != NULL && !est_dans_larbre) {
            if (temp->contenu == element) {
                est_dans_larbre = true;
            } else if (temp->contenu < element) {
                temp = temp->droite;
            } else {
                temp = temp->gauche;
            }
        }
    }
    return est_dans_larbre;
}

/**
 * même chose que contient() mais avec un retour Noeud passé en paramètre
 * @tparam T
 * @param element
 * @param temp
 * @return
 */
template <class T>
bool ArbreAVL<T>::contient(const T& element, Noeud*& temp) const
{
    bool est_dans_larbre = false;
    if (racine) {
        temp = racine;
        while (temp != NULL && !est_dans_larbre ) {
            if (temp->contenu == element) {
                est_dans_larbre = true;
            } else if (temp->contenu < element) {
                temp = temp->droite;
            } else {
                temp = temp->gauche;
            }
        }
    }
    return est_dans_larbre;
}

/**
 * insère un élément dans l'arbre en appelant la fonction inserer (Noeud*&, const T& element)
 * @tparam T
 * @param element
 */
template <class T>
void ArbreAVL<T>::inserer(const T& element)
{
    inserer(racine, element);
}

/**
 * Crée un noeud si la racine est NULL
 * insère à gauche si l'élément est plus petit, sinon à droite
 * compare les facteur d'équilibre, l'équilibre ne peut dépasser 2 ou -2
 * si insère à gauche et facteur d'équilibre = 2 :
 * rotation gauchedroite si pas de noeud à gauche, sinon rotation droitegauche du noeud à gauche et rotation droitegauche du noeud
 * si insère à droite et facteur d'équilibre = -2 :
 * rotation  droitegauche si pas de noeud à droite ,sinon rotation gauchedroite du noeud à droite et rotation droite gauche du noeud courrant
 * @tparam T
 * @param noeud
 * @param element
 * @return vrai si l'élément est inséré, faux sinon
 */
template <class T>
bool ArbreAVL<T>::inserer(Noeud*& noeud, const T& element)
{
    if(noeud==NULL)
    {
        noeud = new Noeud(element);
        return true;
    }
    if(element < noeud->contenu){
        if(inserer(noeud->gauche, element))
        {
            noeud->equilibre++;
            if(noeud->equilibre == 0) return false;
            if(noeud->equilibre == 1) return true;
            assert(noeud->equilibre==2);
            if(noeud->gauche->equilibre == -1)
                rotationDroiteGauche(noeud->gauche);
            rotationGaucheDroite(noeud);
        }
        return false;
    }
    else {
        if(inserer(noeud->droite, element))
        {
            noeud->equilibre--;
            if(noeud->equilibre == 0) return false;
            if(noeud->equilibre == -1) return true;
            assert(noeud->equilibre== -2);
            if(noeud->droite->equilibre == 1)
                rotationGaucheDroite(noeud->droite);
            rotationDroiteGauche(noeud);
        }
        return false;
    }

    // element == noeud->contenu
    noeud->contenu = element;  // Mise à jour
    return false;
}

/**
 *Crée un noeud temporaire avec le gauche de la racine
 *le gauche de la racine pointe vers le droite du temporaire
 * le temporaire droite pointe vers la racine
 * la racine devient le temporaire
 * réquilibre les facteurs d'équilibre
 * @tparam T
 * @param racinesousarbre
 */
template <class T>
void ArbreAVL<T>::rotationGaucheDroite(Noeud*& racinesousarbre)
{
    Noeud *temp = racinesousarbre->gauche;
    int  ea = temp->equilibre;
    int  eb = racinesousarbre->equilibre;
    int  neb = -(ea>0 ? ea : 0) - 1 + eb;
    int  nea = ea + (neb < 0 ? neb : 0) - 1;

    temp->equilibre = nea;
    racinesousarbre->equilibre = neb;
    racinesousarbre->gauche = temp->droite;
    temp->droite = racinesousarbre;
    racinesousarbre = temp;
}

/**
 * Crée un noeud temporaire avec le noeud droit de la racine
 * le droite de la racine pointe vers le temporaire gauche
 * le temporaire gauche pointe vers la racine
 * la racine devient le temporaire
 * réétablit les facteurs d'équilibre
 * @tparam T
 * @param racinesousarbre
 */
template <class T>
void ArbreAVL<T>::rotationDroiteGauche(Noeud*& racinesousarbre)
{
    Noeud *temp = racinesousarbre->droite;
    int  ea = temp->equilibre;
    int  eb = racinesousarbre->equilibre;
    int  neb = -(ea<0 ? ea : 0) + 1 + eb;
    int  nea = ea + (neb < 0 ? neb : 0) + 1;

    temp->equilibre = nea;
    racinesousarbre->equilibre = neb;
    racinesousarbre->droite = temp->gauche;
    temp->gauche = racinesousarbre;
    racinesousarbre = temp;
}

/**
 * l'arbre est vide si la racine est null
 * @tparam T
 * @return vrai si la racine est null, faux sinon
 */
template <class T>
bool ArbreAVL<T>::vide() const
{
    return racine == NULL;
}

/**
 * appel la fonction vider(racine)
 * @tparam T
 */
template <class T>
void ArbreAVL<T>::vider(){
    vider(racine);
}

/**
 * tant que la racine n'est pas NULL, met le max gauche à NUll, et le max droit à NULL
 * @tparam T
 * @param noeud
 */
template <class T>
void ArbreAVL<T>::vider(Noeud*& noeud)
{
    Noeud* temp;
    if (noeud != NULL)
    {
        vider(noeud->gauche);
        vider(noeud->droite);
        temp=noeud;
        delete temp;
        noeud=NULL;
    }
}

template <class T>
void ArbreAVL<T>::copierNoeud(Noeud*& source,Noeud*& noeud)
{
    Noeud* temp;
    if (noeud==NULL)
    {
        return noeud;
    }
    else
    {
        temp = new Noeud();
        temp->contenu = noeud->contenu;
        temp->gauche = copierNoeud(noeud->gauche);
        temp->droite = copierNoeud(noeud->droite);
        return temp;
    }
}

/**
 * copie par référence avec récursivité tous les noeuds de gauche et droite
 * @tparam T
 * @param source le ptr source du noeud à copier
 * @param noeud le ptr par référence de la copie du noeud
 */
template <class T>
void ArbreAVL<T>::copier(const Noeud* source, Noeud*& noeud) const
{
    if (source!=NULL)
    {
        delete noeud;
        noeud = new Noeud(source->contenu);
        noeud->equilibre = source ->equilibre;
        copier(source->gauche,noeud->gauche);
        copier(source->droite,noeud->droite);

    }
}

/**
 * incrémente la hauteur vers le chemin le plus long
 * @tparam T
 * @return la hauteur de l'arbre
 */
template <class T>
int  ArbreAVL<T>::hauteur() const{
    Noeud* n = racine;
    int hauteur = 0;
    while (n !=NULL){
        hauteur++;
        n= n->equilibre == 1? n->gauche : n->droite;
    }
    return hauteur;
}

/**
 * parcours jusqu'au dernier noeud droit et retourne sa valeur
 * @tparam T
 * @param n l'arbre à analyser
 * @return la valeur max de l'arbre
 */
template <class T>
const T& ArbreAVL<T>::max(Noeud* n) const
{
    if (n==NULL)
    {
        return n;
    }
    else
    {
        while(n->right !=NULL)
        {
            n=n->right;
        }
        return n->contenu;
    }
}

//----------- L'enlèvement est optionnel (non requise pour le TP2) ----------//
template <class T>
void ArbreAVL<T>::enlever(const T& element)
{
    enlever(racine, element);
}

template <class T>
bool ArbreAVL<T>::enlever(Noeud*& noeud, const T& element)
{
    if(element < noeud->contenu)
    {
        if(enlever(noeud->gauche, element))
        {
            noeud->equilibre--;
            if(noeud->equilibre == 0) return false;
            if(noeud->equilibre == -1) return true;
            assert(noeud->equilibre == -2);
            if(noeud->droite->equilibre == 2)
                rotationDroiteGauche(noeud->gauche);
            rotationDroiteGauche(noeud);
        }
        return false;
    }
    else if(element > noeud->contenu)
    {
        noeud->equilibre--;
        if(noeud->equilibre == 0) return false;
        if(noeud->equilibre == -1) return true;
        assert(noeud->equilibre== -2);
        /*if(noeud->gauche->equilibre == -1)
            rotationDroiteGauche(noeud->gauche);*/
        rotationGaucheDroite(noeud);


        return false;
    }
    else if(element == noeud->contenu)
    {
        if (noeud->gauche==NULL && noeud->droite==NULL)
        {
            delete noeud;
            noeud = NULL;
            return true;
        }
        else
        {
            Noeud* temp = noeud;
            if ( noeud ->droite == NULL){
                noeud = temp->gauche;
            }else if (noeud->gauche == NULL){
                noeud = temp->droite;
            }
            return true;
        }
    }

}
//----------- L'enlèvement est optionnel (non requise pour le TP2) ----------//
/*
 * vers la dernière feuille à gauche
 */
template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::debut() const
{
    Pile<Noeud*> pile;
    Iterateur iter(*this);
    if (racine != NULL) {
        Noeud* temp = racine;
        while(temp->gauche !=NULL){
            pile.empiler(temp);
            temp = temp->gauche;
        }
        iter.courant = temp;
        iter.chemin = pile;
    }
    return iter;
}

/*
 * vers la dernière feuille à droite
 */
template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::fin() const
{
    Pile<Noeud*> pile;
    Iterateur iter(*this);
    if (racine != NULL) {
        Noeud* temp = racine;
        while(temp->droite !=NULL){
            pile.empiler(temp);
            temp = temp->droite;
        }
        iter.courant = temp;
    }
    return iter;
}

template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::rechercher(const T& e) const
{
    Iterateur it = this->debut();
    if (this->racine != NULL) {
        Noeud* temp;
        if(this->contient(e,temp)){
            it.courant = temp;
        }else{
            delete temp;
            it.courant = NULL;
        }
    }

    return it;
}

template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::rechercherEgalOuSuivant(const T& e) const
{
    Iterateur iter(*this);
    // À compléter.
    return iter;
}

template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::rechercherEgalOuPrecedent(const T& e) const
{
    Noeud* n = racine, *dernier=NULL;
    while(n){
        if(e < n->contenu){
        n = n->gauche;
        }else if(n->contenu < e){
            dernier = n;
            n = n->droite;
        }else{
        return rechercher(e);
    }
    }
    if(dernier!=NULL)
    return rechercher(dernier->contenu);
    return Iterateur(*this);
}

template <class T>
const T& ArbreAVL<T>::operator[](const Iterateur& iterateur) const
{
    assert(&iterateur.arbre_associe == this);
    assert(iterateur.courant);
    return iterateur.courant->contenu;
}

template <class T>
T& ArbreAVL<T>::operator[](const Iterateur& iterateur)
{
    assert(&iterateur.arbre_associe == this);
    assert(iterateur.courant);
    return iterateur.courant->contenu;
}

template <class T>
ArbreAVL<T>& ArbreAVL<T>::operator=(const ArbreAVL& autre) {
    if(this==&autre) return *this;
    vider();
    copier(autre.racine, racine);
    return *this;
}

//-----------------------
template <class T>
ArbreAVL<T>::Iterateur::Iterateur(const ArbreAVL& a)
        : arbre_associe(a), courant(NULL)
{
}

template <class T>
ArbreAVL<T>::Iterateur::Iterateur(const ArbreAVL<T>::Iterateur& a)
        : arbre_associe(a.arbre_associe)
{
    courant = a.courant;
    chemin = a.chemin;
}

/**
 * incrémente l'itérateur de l'arbre.
 * @tparam T
 * @return l'iterateur à la position suivante
 */
template <class T>
typename ArbreAVL<T>::Iterateur& ArbreAVL<T>::Iterateur::operator++()
{
    assert(courant);
    Noeud* suivant = courant->droite;
    while(suivant){

        chemin.empiler(suivant);
        suivant = suivant->gauche;

    }
    if(!chemin.vide()) {
        courant = chemin.depiler();
    }else {
        courant = NULL;
    }
    return *this;
}

// Post-incrément
template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::Iterateur::operator++(int)
{
    Iterateur copie(*this);
    operator++();
    return copie;
}

template <class T>
ArbreAVL<T>::Iterateur::operator bool() const
{
    return courant!=NULL;
}

template <class T>
bool ArbreAVL<T>::Iterateur::operator!() const{
    return courant==NULL;
}

template <class T>
bool ArbreAVL<T>::Iterateur::operator==(const Iterateur& o) const{
    assert(&arbre_associe==&o.arbre_associe);
    return courant==o.courant;
}

template <class T>
bool ArbreAVL<T>::Iterateur::operator!=(const Iterateur& o) const{
    assert(&arbre_associe==&o.arbre_associe);
    return courant!=o.courant;
}

template <class T>
const T& ArbreAVL<T>::Iterateur::operator *() const
{
    assert(courant!=NULL);
    return courant->contenu;
}

template <class T>
typename ArbreAVL<T>::Iterateur& ArbreAVL<T>::Iterateur::operator = (const Iterateur& autre){
    assert(&arbre_associe==&autre.arbre_associe);
    courant = autre.courant;
    chemin = autre.chemin;
    return *this;
}
#endif




