/*  INF3105 -TP3- Structures de données et algorithmes
     Julien Guité-Vinet 
     GUIJ09058407
*/

#include <iostream>
#include <limits>
#include <fstream>
#include <sstream>
#include <cmath>
#include <algorithm>
#include "Graphe.h"
#include "Univers.h"

using namespace std;

/**
 * Calcule le coût d'un déplacement dans l'univers
 * coût déplacement orthogonal = 1
 * coût déplacement diagonal = 1.4
 * Si arrivé est Eau, déplacement impossible, instancie arrivé à infinity
 * Si arrivé est forêt, le coût est doublé
 * Si départ est route et arrivé est route, le coût est de moité
 * débordement de la matrice permis sur l'abscisse
 * débordement de la matrice non-permis sur l'ordonné
 * @param univers la matrice de la map
 * @param o l'origine
 * @param d la destination
 * @param case_depart_r la case départ est une route?
 */
void CalculerPoidsCaseDepartNFP (Univers<int,double>& univers, int& o, int& d, bool& case_depart_r){
    int a;
    double inf = numeric_limits<double>::infinity();

    // à droite
    if (o == univers.largeur - 1) {
        //passe à travers la matrice
        a = -univers.largeur + 1;
    } else {
        a = 1;
    }

    if (univers.univers[o + a][d] == 'N' || univers.univers[o + a][d] == 'P') {
        univers.distances[o + a][d] = 1;

    } else if (univers.univers[o + a][d] == 'F') {
        univers.distances[o + a][d] = 2;

    } else if (univers.univers[o + a][d] == 'R') {

        if (!case_depart_r) {
            univers.distances[o + a][d] = 1;
        } else {
            univers.distances[o + a][d] = .50;
        }

    } else if (univers.univers[o + a][d] == 'E') {
        univers.distances[o + a][d] = inf;

    }
    //en bas
    if (d != univers.longeur - 1) {

        if (univers.univers[o][d + 1] == 'N' || univers.univers[o][d + 1] == 'P') {
            univers.distances[o][d + 1] = 1;

        } else if (univers.univers[o][d + 1] == 'F') {
            univers.distances[o][d + 1] = 2;

        } else if (univers.univers[o][d + 1] == 'R') {
            if (!case_depart_r) {
                univers.distances[o][d + 1] = 1;
            } else {
                univers.distances[o][d + 1] = .50;
            }

        } else if (univers.univers[o][d + 1] == 'E') {
            univers.distances[o][d + 1] = inf;

        }
    }
    //à gauche
    if (o == 0) {
        a = -univers.largeur + 1;
    } else {
        a = 1;
    }
    if (univers.univers[o - a][d] == 'N' || univers.univers[o - a][d] == 'P') {
        univers.distances[o - a][d] = 1;

    } else if (univers.univers[o - a][d] == 'F') {
        univers.distances[o - a][d] = 2;

    } else if (univers.univers[o - a][d] == 'R') {
        if (!case_depart_r) {
            univers.distances[o - a][d] = 1;
        } else {
            univers.distances[o - a][d] = .50;
        }

    } else if (univers.univers[o - a][d] == 'E') {
        univers.distances[o - a][d] = inf;

    }

    //en haut
    if (d != 0) {

        if (univers.univers[o][d - 1] == 'N' || univers.univers[o][d - 1] == 'P') {
            univers.distances[o][d - 1] = 1;

        } else if (univers.univers[o][d - 1] == 'F') {
            univers.distances[o][d - 1] = 2;

        } else if (univers.univers[o][d - 1] == 'R') {
            if (!case_depart_r) {
                univers.distances[o][d - 1] = 1;
            } else {
                univers.distances[o ][d - 1] = .50;
            }

        } else if (univers.univers[o][d - 1] == 'E') {
            univers.distances[o][d - 1] = inf;

        }
    }
    //diagaonal à gauche en haut
    if (o == 0 && d != 0) {
        //passe à travers la matrice
        a = -univers.largeur + 1;

        if (univers.univers[o - a][d - 1] == 'N' || univers.univers[o - a][d - 1] == 'P') {
            univers.distances[o - a][d - 1] = 1.40;

        } else if (univers.univers[o - a][d - 1] == 'F') {
            univers.distances[o - a][d - 1] = 2.80;

        } else if (univers.univers[o - a][d - 1] == 'R') {
            if (!case_depart_r) {
                univers.distances[o - a][d - 1] = 1.40;
            } else {
                univers.distances[o - a][d - 1] = .70;
            }

        } else if (univers.univers[o - a][d - 1] == 'E') {
            univers.distances[o - a][d - 1] = inf;

        }
    } else if (o != 0 && d != 0) {
        a = 1;

        if (univers.univers[o - a][d - 1] == 'N' || univers.univers[o - a][d - 1] == 'P') {
            univers.distances[o - a][d - 1] = 1.40;

        } else if (univers.univers[o - a][d - 1] == 'F') {
            univers.distances[o - a][d - 1] = 2.80;

        } else if (univers.univers[o - a][d - 1] == 'R') {
            if (!case_depart_r) {
                univers.distances[o - a][d - 1] = 1.40;
            } else {
                univers.distances[o - a][d - 1] = .70;
            }

        } else if (univers.univers[o - a][d - 1] == 'E') {
            univers.distances[o - a][d - 1] = inf;
        }
    }
    //diagonal à droite en haut
    if (o == univers.largeur - 1 && d != 0) {
        a = -univers.largeur + 1;

        if (univers.univers[o + a][d - 1] == 'N' || univers.univers[o + a][d - 1] == 'P') {
            univers.distances[o + a][d - 1] = 1.40;

        } else if (univers.univers[o + a][d - 1] == 'F') {
            univers.distances[o + a][d - 1] = 2.80;

        } else if (univers.univers[o + a][d - 1] == 'R') {
            if (!case_depart_r) {
                univers.distances[o + a][d - 1] = 1.40;
            } else {
                univers.distances[o + a][d - 1] = .70;
            }

        } else if (univers.univers[o + a][d - 1] == 'E') {
            univers.distances[o + a][d - 1] = inf;

        }
    } else if (o != univers.largeur - 1 && d != 0) {
        a = 1;

        if (univers.univers[o + a][d - 1] == 'N' || univers.univers[o + a][d - 1] == 'P') {
            univers.distances[o + a][d - 1] = 1.40;

        } else if (univers.univers[o + a][d - 1] == 'F') {
            univers.distances[o + a][d - 1] = 2.80;

        } else if (univers.univers[o + a][d - 1] == 'R') {
            if (!case_depart_r) {
                univers.distances[o + a][d - 1] = 1.40;
            } else {
                univers.distances[o + a][d - 1] = .70;
            }

        } else if (univers.univers[o + a][d - 1] == 'E') {
            univers.distances[o + a][d - 1] = inf;

        }
    }
    //diagonal en bas à gauche
    if (o == 0 && d != univers.longeur - 1) {
        a = -univers.largeur + 1;

        if (univers.univers[o - a][d + 1] == 'N' || univers.univers[o - a][d + 1] == 'P') {
            univers.distances[o - a][d + 1] = 1.40;

        } else if (univers.univers[o - a][d + 1] == 'F') {
            univers.distances[o - a][d + 1] = 2.80;

        } else if (univers.univers[o - a][d + 1] == 'R') {
            if (!case_depart_r) {
                univers.distances[o - a][d + 1] = 1.40;
            } else {
                univers.distances[o - a][d + 1] = .70;
            }

        } else if (univers.univers[o - a][d + 1] == 'E') {
            univers.distances[o - a][d + 1] = inf;

        }
    } else if (o != 0 && d != univers.longeur - 1) {
        a = 1;

        if (univers.univers[o - a][d + 1] == 'N' || univers.univers[o - a][d + 1] == 'P') {
            univers.distances[o - a][d + 1] = 1.40;

        } else if (univers.univers[o - a][d + 1] == 'F') {
            univers.distances[o - a][d + 1] = 2.80;

        } else if (univers.univers[o - a][d + 1] == 'R') {
            if (!case_depart_r) {
                univers.distances[o - a][d + 1] = 1.40;
            } else {
                univers.distances[o - a][d + 1] = .70;
            }

        } else if (univers.univers[o - a][d + 1] == 'E') {
            univers.distances[o - a][d + 1] = inf;

        }
    }
    //diagonal en bas à droite
    if (o == univers.largeur - 1 && d != univers.longeur - 1) {
        a = -univers.largeur + 1;

        if (univers.univers[o + a][d + 1] == 'N' || univers.univers[o + a][d + 1] == 'P') {
            univers.distances[o + a][d + 1] = 1.40;

        } else if (univers.univers[o + a][d + 1] == 'F') {
            univers.distances[o + a][d + 1] = 2.80;

        } else if (univers.univers[o + a][d + 1] == 'R') {
            if (!case_depart_r) {
                univers.distances[o + a][d + 1] = 1.40;
            } else {
                univers.distances[o + a][d + 1] = .70;
            }

        } else if (univers.univers[o + a][d + 1] == 'E') {
            univers.distances[o + a][d + 1] = inf;

        }
    } else if (o != univers.largeur - 1 && d != univers.longeur - 1) {
        a = 1;

        if (univers.univers[o + a][d + 1] == 'N' || univers.univers[o + a][d + 1] == 'P') {
            univers.distances[o + a][d + 1] = 1.40;

        } else if (univers.univers[o + a][d + 1] == 'F') {
            univers.distances[o + a][d + 1] = 2.80;

        } else if (univers.univers[o + a][d + 1] == 'R') {
            if (!case_depart_r) {
                univers.distances[o + a][d + 1] = 1.40;
            } else {
                univers.distances[o + a][d + 1] = .70;
            }

        } else if (univers.univers[o + a][d + 1] == 'E') {
            univers.distances[o + a][d + 1] = inf;

        }
    }

}
/**
 * initialise les données du  tableau de distances à infinity
 * @param distances
 * @param largeur
 * @param longeur
 */
void ResetTableau (Univers<int,double>& univers){
    double inf = numeric_limits<double>::infinity();
    for (int o = 0; o < univers.largeur; o++) {
        for (int d = 0; d < univers.longeur; d++) {
            univers.distances[o][d] = inf;
        }
    }
}
/**
 * Calcule la distance entre tous les points de l'univers pour tous les sommets
 * Impossible de se déplacer sur l'eau, instancie la distance à infinity
 * Crée les sommets et ajoute les arcs
 * @param univers la matrice de la map
 * @param largeur la largeur de la map
 * @param longeur la longeur de la map
 * @param graphe  les sommets et leurs arcs
 */
void FloydWarshall (Univers<int,double>& univers, int& largeur, int& longeur, Graphe <pair<int,int>, double>& graphe) {
    graphe.nbr_sommets=0;

    //le sommet de départ
    pair<int,int> p;
    //le sommet d'arrivé
    pair<int,int> p2;

    double inf = numeric_limits<double>::infinity();
    bool case_depart_r = false;

    for (int o = 0; o < largeur; o++) {
        for (int d = 0; d < longeur; d++) {
            //reset le tableau pour autre sommet
            ResetTableau(univers);

            //déplacement sur eau impossible
            if (univers.univers[o][d] == 'E') {

                univers.distances[o][d] = inf;
                p.first = o;
                p.second = d;
                graphe.ajouterSommet(p);
                for (int i = 0 ; i < largeur; i++){
                    for (int j = 0 ; j < longeur; j++){
                        p2.first = i;
                        p2.second = j;
                        graphe.ajouterAreteOrientee(p,p2,univers.distances[i][j]);
                        univers.distancesfinal[graphe.nbr_sommets][(i *longeur) +j ] = univers.distances [i][j];
                    }
                }
                graphe.nbr_sommets++;

            //départ sur plaine N, porte P, ou forêt F
            } else if (univers.univers[o][d] == 'N' || univers.univers[o][d] == 'P' || univers.univers[o][d] == 'F') {
                case_depart_r = false;
                if (univers.univers[o][d] == 'P'){univers.distances[univers.porte.first][univers.porte.second]=0;}
                CalculerPoidsCaseDepartNFP(univers, o , d,case_depart_r);
                p.first = o;
                p.second = d;
                graphe.ajouterSommet(p);
                for (int i = 0 ; i < largeur; i++){
                    for (int j = 0 ; j < longeur; j++){
                        p2.first = i;
                        p2.second = j;
                        graphe.ajouterAreteOrientee(p,p2,univers.distances[i][j]);
                        univers.distancesfinal[graphe.nbr_sommets][(i *longeur) +j ] = univers.distances [i][j];
                    }
                }
                graphe.nbr_sommets++;

            //départ sur route
            } else if (univers.univers[o][d] == 'R') {
                case_depart_r = true;
                CalculerPoidsCaseDepartNFP(univers , o , d,case_depart_r);
                p.first = o;
                p.second = d;
                graphe.ajouterSommet(p);
                for (int i = 0 ; i < largeur; i++){
                    for (int j = 0 ; j < longeur; j++){
                        p2.first = i;
                        p2.second = j;
                        graphe.ajouterAreteOrientee(p,p2,univers.distances[i][j]);
                        univers.distancesfinal[graphe.nbr_sommets][(i *longeur) +j ] = univers.distances [i][j];
                    }
                }
                graphe.nbr_sommets++;
            }
        }
    }
// Algorithme Floyd-Warshall
    for (int k = 0; k < graphe.nbr_sommets; k++) {
        for (int i = 0; i < graphe.nbr_sommets; i++) {
            for (int j = 0; j < graphe.nbr_sommets; j++) {
                if (univers.distancesfinal[i][k] + univers.distancesfinal[k][j] < univers.distancesfinal[i][j]) {
                    univers.distancesfinal[i][j] = univers.distancesfinal[i][k] + univers.distancesfinal[k][j];
                }
            }
        }
    }
                        //fonctionne mais vraiment trop lent
 /*
    for (Graphe <pair<int,int>, double>::Iterateur iterk = graphe.debut();iterk != graphe.fin(); ++iterk) {
        for (Graphe <pair<int,int>, double>::Iterateur iteri = graphe.debut();iteri != graphe.fin(); ++iteri) {
            for (Graphe <pair<int,int>, double>::Iterateur iterj = graphe.debut();iterj != graphe.fin(); ++iterj) {


                if (graphe.sommets[iteri.sommet()].voisins[iterk.sommet()] + graphe.sommets[iterk.sommet()].voisins[iterj.sommet()] < graphe.sommets[iteri.sommet()].voisins[iterj.sommet()] ){
                    graphe.sommets[iteri.sommet()].voisins[iterj.sommet()] = graphe.sommets[iteri.sommet()].voisins[iterk.sommet()] + graphe.sommets[iterk.sommet()].voisins[iterj.sommet()];
                }
            }
        }
    }*/
}
static inline bool EstUnDigit(char c) { return !isdigit(c); }
/**
 *Vérifie si la string contient seulement des numéros
 * @param str
 * @return vrai si la string contient seulement des numéros, faux sinon
 */
bool EstUnNumero(const string &str) {
    return find_if(str.begin(), str.end(), EstUnDigit) == str.end(); }
/**
 *Vérifie si la string est de la même largeur que l'univers et contient seulement les lettre NFPER
 * @param mot
 * @param largeur
 * @return vrai si la string est de la même largeur que l'univers et contient seulement les lettre NFPER, faux sinon
 */
bool EstUnMotValide (string& mot, int& largeur){

    bool est_valide = false;
    if (mot.size() == largeur) {
        for (int i = 0; i < mot.size(); i++) {
            est_valide = mot[i] == 'P' || mot[i] == 'N' || mot[i] == 'E' || mot[i] == 'R' || mot[i] == 'F';
            if (!est_valide){
                i = mot.size(); //fin de boucle
                cout << "mauvaise description de l'univers" << endl;
            }
        }
    }else{
        cout << "mauvaise largeur de l'univers" << endl;
    }
    return est_valide;
}
/**
 * Vérifie les coordonnées des trésors
 * Si non-numérique, affiche un message d'erreur
 * Si il y a plus que trois trésors, affiche un message d'erreur
 * Si les coordonnées des trésors ne rentre pas dans la map, affiche un message d'erreur
 * @param mot une string de lettre d'une map d'un univers
 * @param ligne le numéro de ligne de la string de lettre d'une map de l'univers
 * @param ligne_debut_tresor le numéro de ligne des coordonnée du premier trésor
 * @param i un istringstream
 * @param univers
 */
void VerifierCoordonneeTresor(string& mot, int& ligne, int& ligne_debut_tresor, istringstream& i, Univers<int,double>& univers){
    int tresor_coordonnee_x;
    int tresor_coordonnee_y;
    if (ligne == ligne_debut_tresor){

        //première coordonnée du premier trśeor
        if (EstUnNumero(mot)){
            tresor_coordonnee_x = stoi(mot);
            if (tresor_coordonnee_x > univers.largeur - 1){
                cout << "entrez une coordonnée x pour le premier trésor plus petit que la largeur de l'univers" <<endl;
                exit(-1);
            }
            univers.tresor_1.first = stoi(mot);
        }else {
            cout << "entrez une coordonnée x valide pour le premier trésor" <<endl;
            exit(-1);
        }

        //deuxième coordonnée du premier trśeor
        i >> mot;
        if (EstUnNumero(mot)){
            tresor_coordonnee_y = stoi(mot);
            if (tresor_coordonnee_y > univers.longeur -1){
                cout << "entrez une coordonnée y pour le premier trésor plus petit que la longeur de l'univers" <<endl;
                exit(-1);
            }
            univers.tresor_1.second = stoi(mot);
        }else {
            cout << "entrez une coordonnée y valide pour le premier trésor" <<endl;
            exit(-1);
        }
        i.str(string());

    }else if (ligne == ligne_debut_tresor + 1){
        if (EstUnNumero(mot)){
            tresor_coordonnee_x = stoi(mot);
            if (tresor_coordonnee_x > univers.largeur - 1){
                cout << "entrez une coordonnée x pour le deuxième trésor plus petit que la largeur de l'univers" <<endl;
                exit(-1);
            }
            univers.tresor_2.first = stoi(mot);
        }else {
            cout << "entrez une coordonnée x valide pour le deuxième trésor" <<endl;
            exit(-1);
        }

        i >> mot;
        if (EstUnNumero(mot)){
            tresor_coordonnee_y = stoi(mot);
            if (tresor_coordonnee_y > univers.longeur - 1){
                cout << "entrez une coordonnée y pour le deuxième trésor plus petit que la longeur de l'univers" <<endl;
                exit(-1);
            }
            univers.tresor_2.second = stoi(mot);
        }else {
            cout << "entrez une coordonnée y valide pour le deuxième trésor" <<endl;
            exit(-1);
        }
        i.str(string());
    }else if (ligne == ligne_debut_tresor + 2){
        if (EstUnNumero(mot)){
            tresor_coordonnee_x = stoi(mot);
            if (tresor_coordonnee_x > univers.largeur - 1){
                cout << "entrez une coordonnée x pour le troisième trésor plus petit que la largeur de l'univers" <<endl;
                exit(-1);
            }
            univers.tresor_3.first = stoi(mot);
        }else {
            cout << "entrez une coordonnée x valide pour le troisième trésor" <<endl;
            exit(-1);
        }

        i >> mot;
        if (EstUnNumero(mot)){
            tresor_coordonnee_y = stoi(mot);
            if (tresor_coordonnee_y > univers.longeur -1){
                cout << "entrez une coordonnée y pour le troisième trésor plus petit que la longeur de l'univers" <<endl;
                exit(-1);
            }
            univers.tresor_3.second = stoi(mot);
        }else {
            cout << "entrez une coordonnée y valide pour le troisième trésor" <<endl;
            exit(-1);
        }
        i.str(string());
    }else if (ligne == ligne_debut_tresor + 3) {
        cout << "il y a trop de trésors" << endl;
        exit(-1);
    }
}

/**
 * Vérifie le nombre de porte dans l'univers
 * Si plus grand que deux, affiche un message d'erreur
 * @param mot une string de lettre d'une map d'un univers
 * @param ligne le numéro de ligne de la string de lettre d'une map de l'univers
 * @param univers
 */
void VerifierNombrePorte(string& mot, int& ligne,Univers<int,double>& univers){

    size_t found = mot.find('P');
    if (found!= string::npos) {
        univers.porte.first = found;
        univers.porte.second = ligne - 2;
        univers.nbr_porte++;
    }
    //il peut y avoir un autre P sur la même ligne
    if (univers.nbr_porte == 1){
        found = mot.find('P', found + 1);
        if (found!= string::npos) {
            univers.nbr_porte++;
        }
    }
    if (univers.nbr_porte == 2){
        cout << "Il y a trop de portes!!" <<endl;
        exit (-1);
    }
}
/**
 * Retourne vrai si un trésor est dans l'eau, sinon faux
 * @param univers2
 * @return
 */
bool EstUnTresorDansEau(Univers<int,double>& univers2){
    bool est_dans_eau = false;
    if(univers2.univers[univers2.tresor_1.first][univers2.tresor_1.second] == 'E' || univers2.univers[univers2.tresor_2.first][univers2.tresor_2.second] == 'E' || univers2.univers[univers2.tresor_3.first][univers2.tresor_3.second] == 'E'){
        est_dans_eau = true;
    }
    return  est_dans_eau;
}
/**
 * retourne le plus court chemin pour trois trésors
 * @param ordre_1_2_3
 * @param ordre_1_3_2
 * @param ordre_2_1_3
 * @param ordre_2_3_1
 * @param ordre_3_1_2
 * @param ordre_3_2_1
 * @return
 */
double PlusCourtChemin(double ordre_1_2_3, double ordre_1_3_2, double ordre_2_1_3,double ordre_2_3_1,double ordre_3_1_2,double ordre_3_2_1){
    return min(min(ordre_3_1_2,ordre_3_2_1),min(min(ordre_1_2_3,ordre_1_3_2),min(ordre_2_1_3,ordre_2_3_1)));

}
/**
 * Affiche les résultats de l'algorithme de floyd-warchall
 * si aucun trésor accessible, affiche un message d'erreur
 * si 2 tresors accesibles, affiche le plus court chemin pour 2 trésors
 * si 1 trésor accesible, affiche le plus court chemin pour le trésor
 * si 3 trésors accesibles, affiche le plus court chemin pour les 3 trésors
 * @param univers
 * @param graphe
 */
void AfficherResultats(Univers<int,double>& univers,Graphe <pair<int,int>, double>& graphe){
    bool tresor1_entoure_eau = false;
    bool tresor2_entoure_eau = false;
    bool tresor3_entoure_eau = false;
    bool tous_tresors_accesible = false;
    double inf  = numeric_limits<double>::infinity();

    //les indices d'une matrice convertis en tableau
    int indice_porte = univers.porte.first * univers.longeur + univers.porte.second;
    int indice_tresor1= univers.tresor_1.first * univers.longeur + univers.tresor_1.second;
    int indice_tresor2= univers.tresor_2.first * univers.longeur + univers.tresor_2.second;
    int indice_tresor3= univers.tresor_3.first * univers.longeur + univers.tresor_3.second;

    //les distances finales
    double t1_p =0;
    double t1_t2=0;
    double t1_t3=0;
    double t2_p =0;
    double t2_t1=0;
    double t2_t3=0;
    double t3_p =0;
    double t3_t1=0;
    double t3_t2=0;

    //depuis porte
/*    double p_t1 = graphe.sommets[univers.porte].voisins[univers.tresor_1];
    double p_t2 = graphe.sommets[univers.porte].voisins[univers.tresor_2];
    double p_t3 = graphe.sommets[univers.porte].voisins[univers.tresor_3];*/
    double p_t1 = univers.distancesfinal[indice_porte][indice_tresor1];
    double p_t2 = univers.distancesfinal[indice_porte][indice_tresor2];
    double p_t3 = univers.distancesfinal[indice_porte][indice_tresor3];
    if (p_t1 == inf){tresor1_entoure_eau = true;p_t1 = 0;}
    if (p_t2 == inf){tresor2_entoure_eau = true;p_t2 = 0;}
    if (p_t3 == inf){tresor3_entoure_eau = true;p_t3 = 0;}

    if (!tresor1_entoure_eau && !tresor2_entoure_eau && !tresor3_entoure_eau ){tous_tresors_accesible = true;}

    if (tresor1_entoure_eau && tresor2_entoure_eau && tresor3_entoure_eau) {
        cout << "Tous les trésor sont au fond de l'eau!!! Link doit trouver le tuba pour nager" << endl;
        exit(-1);
    }else if (tresor1_entoure_eau && tresor2_entoure_eau) {
      //  t3_p = graphe.sommets[univers.tresor_3].voisins[univers.porte];
        t3_p = univers.distancesfinal[indice_tresor3][indice_porte];
        cout << "Porte -> T3 : " << p_t3 << endl;
        cout << "T3 -> Porte : " << t3_p << endl;
        cout << "Total : " << p_t3 + t3_p << endl;
        cout << endl;
    }else if (tresor1_entoure_eau && tresor3_entoure_eau) {
       // t2_p = graphe.sommets[univers.tresor_2].voisins[univers.porte];
        t2_p = univers.distancesfinal[indice_tresor2][indice_porte];
        cout << "Porte -> T2 : " << p_t2 << endl;
        cout << "T2 -> Porte : " << t2_p << endl;
        cout << "Total : " << p_t2 + t2_p << endl;
        cout << endl;
    }else if (tresor2_entoure_eau && tresor3_entoure_eau) {
        //t1_p = graphe.sommets[univers.tresor_1].voisins[univers.porte];
        t1_p = univers.distancesfinal[indice_tresor1][indice_porte];
        cout << "Porte -> T1 : " << p_t1 << endl;
        cout << "T1 -> Porte : " << t1_p << endl;
        cout << "Total : " << p_t1 + t1_p << endl;
        cout << endl;
    }else {

        //depuis tresor_1
        if (!tresor1_entoure_eau) {
            t1_p = univers.distancesfinal[indice_tresor1][indice_porte];
            t1_t2 = univers.distancesfinal[indice_tresor1][indice_tresor2];
            t1_t3 = univers.distancesfinal[indice_tresor1][indice_tresor3];
           /* t1_p = graphe.sommets[univers.tresor_1].voisins[univers.porte];
            t1_t2 = graphe.sommets[univers.tresor_1].voisins[univers.tresor_2];
            t1_t3 = graphe.sommets[univers.tresor_1].voisins[univers.tresor_3];*/
        }

        //depuis tresor_2
        if (!tresor2_entoure_eau) {
            t2_p = univers.distancesfinal[indice_tresor2][indice_porte];
            t2_t1 = univers.distancesfinal[indice_tresor2][indice_tresor1];
            t2_t3 = univers.distancesfinal[indice_tresor2][indice_tresor3];
            /*t2_p = graphe.sommets[univers.tresor_2].voisins[univers.porte];
            t2_t1 = graphe.sommets[univers.tresor_2].voisins[univers.tresor_1];
            t2_t3 = graphe.sommets[univers.tresor_2].voisins[univers.tresor_3];*/
        }

        //depuis tresor_3
        if (!tresor3_entoure_eau) {
            t3_p = univers.distancesfinal[indice_tresor3][indice_porte];
            t3_t1 = univers.distancesfinal[indice_tresor3][indice_tresor1];
            t3_t2 = univers.distancesfinal[indice_tresor3][indice_tresor2];
            /*t3_p = graphe.sommets[univers.tresor_3].voisins[univers.porte];
            t3_t1 = graphe.sommets[univers.tresor_3].voisins[univers.tresor_1];
            t3_t2 = graphe.sommets[univers.tresor_3].voisins[univers.tresor_2];*/
        }

        double ordre_1_2_3 = p_t1 + t1_t2 + t2_t3 + t3_p;
        double ordre_1_3_2 = p_t1 + t1_t3 + t3_t2 + t2_p;
        double ordre_2_1_3 = p_t2 + t2_t1 + t1_t3 + t3_p;
        double ordre_2_3_1 = p_t2 + t2_t3 + t3_t1 + t1_p;
        double ordre_3_1_2 = p_t3 + t3_t1 + t1_t2 + t2_p;
        double ordre_3_2_1 = p_t3 + t3_t2 + t2_t1 + t1_p;

        double plus_court_chemin = PlusCourtChemin(ordre_1_2_3, ordre_1_3_2, ordre_2_1_3, ordre_2_3_1, ordre_3_1_2,
                                                   ordre_3_2_1);
        if (ordre_1_2_3 == plus_court_chemin) {
            if (tous_tresors_accesible) {
                cout << "Porte -> T1 : " << p_t1 << endl;
                cout << "T1 -> T2 : " << t1_t2 << endl;
                cout << "T2 -> T3 : " << t2_t3 << endl;
                cout << "T3 -> Porte : " << t3_p << endl;
                cout << "Total : " << plus_court_chemin << endl;
                cout << endl;
            } else if (tresor1_entoure_eau){
                cout << "Porte -> T2 : " << p_t2 << endl;
                cout << "T2 -> T3 : " << t2_t3 << endl;
                cout << "T3 -> Porte : " << t3_p << endl;
                cout << "Total : " << plus_court_chemin << endl;
                cout << endl;
            }

        }
        if (ordre_1_3_2 == plus_court_chemin) {
            if (tous_tresors_accesible) {
                cout << "Porte -> T1 : " << p_t1 << endl;
                cout << "T1 -> T3 : " << t1_t3 << endl;
                cout << "T3 -> T2 : " << t3_t2 << endl;
                cout << "T2 -> Porte : " << t2_p << endl;
                cout << "Total : " << plus_court_chemin << endl;
            } else if (tresor1_entoure_eau){
                cout << "Porte -> T3 : " << p_t3 << endl;
                cout << "T3 -> T2 : " << t3_t2 << endl;
                cout << "T2 -> Porte : " << t2_p << endl;
                cout << "Total : " << plus_court_chemin << endl;
            }
        }
        if (ordre_2_1_3 == plus_court_chemin) {
            if (tous_tresors_accesible) {
                cout << "Porte -> T2 : " << p_t2 << endl;
                cout << "T2 -> T1 : " << t2_t1 << endl;
                cout << "T1 -> T3 : " << t1_t3 << endl;
                cout << "T3 -> Porte : " << t3_p << endl;
                cout << "Total : " << plus_court_chemin << endl;
                cout << endl;
            } else if (tresor2_entoure_eau) {
                cout << "Porte -> T1 : " << p_t1 << endl;
                cout << "T1 -> T3 : " << t1_t3 << endl;
                cout << "T3 -> Porte : " << t3_p << endl;
                cout << "Total : " << plus_court_chemin << endl;
                cout << endl;
            }
        }
        if (ordre_2_3_1 == plus_court_chemin) {
            if (tous_tresors_accesible) {
                cout << "Porte -> T2 : " << p_t2 << endl;
                cout << "T2 -> T3 : " << t2_t3 << endl;
                cout << "T3 -> T1 : " << t3_t1 << endl;
                cout << "T1 -> Porte : " << t1_p << endl;
                cout << "Total : " << plus_court_chemin << endl;
                cout << endl;
            } else if(tresor2_entoure_eau) {
                cout << "Porte -> T3 : " << p_t3 << endl;
                cout << "T3 -> T1 : " << t3_t1 << endl;
                cout << "T1 -> Porte : " << t1_p << endl;
                cout << "Total : " << plus_court_chemin << endl;
                cout << endl;
            }
        }
        if (ordre_3_1_2 == plus_court_chemin) {
            if (tous_tresors_accesible) {
                cout << "Porte -> T3 : " << p_t3 << endl;
                cout << "T3 -> T1 : " << t3_t1 << endl;
                cout << "T1 -> T2 : " << t1_t2 << endl;
                cout << "T2 -> Porte : " << t2_p << endl;
                cout << "Total : " << plus_court_chemin << endl;
                cout << endl;
            } else if (tresor3_entoure_eau){
                cout << "Porte -> T1 : " << p_t1 << endl;
                cout << "T1 -> T2 : " << t1_t2 << endl;
                cout << "T2 -> Porte : " << t2_p << endl;
                cout << "Total : " << plus_court_chemin << endl;
                cout << endl;
            }
        }
        if (ordre_3_2_1 == plus_court_chemin) {
            if (tous_tresors_accesible) {
                cout << "Porte -> T3 : " << p_t3 << endl;
                cout << "T3 -> T2 : " << t3_t2 << endl;
                cout << "T2 -> T1 : " << t2_t1 << endl;
                cout << "T1 -> Porte : " << t1_p << endl;
                cout << "Total : " << plus_court_chemin << endl;
                cout << endl;
            } else if (tresor3_entoure_eau){
                cout << "Porte -> T2 : " << p_t3 << endl;
                cout << "T2 -> T1 : " << t2_t1 << endl;
                cout << "T1 -> Porte : " << t1_p << endl;
                cout << "Total : " << plus_court_chemin << endl;
                cout << endl;
            }
        }
    }
}

/**
 * Si ouvert, lit le fichier à corriger par l'utilisateur, sinon affiche un message d'erreur
 * Valide les coordonnées de la map
 * Valide le terrain de la map
 * Valide le nombre de portes
 * Valide si les trésor sont dans l'eau ainsi que leurs coordonnées
 * Calcule la distances entre les points
 * Affiche les résultats
 * Ferme le fichier
 * @param argv le pointeur de pointeurs aux arguments de la ligne de commande
 * @param argc le nombre d'argument
 */
void LectureFichier(int argc,char* argv []) {
    //aucun fichier à corriger
    if (argc ==2) {
        ifstream fichier ;
        string univers = argv [1];
        fichier.open(univers);
        if (fichier.is_open()) {
            int ligne;
            int longeur;
            int largeur;
            string mot;
            char buf[ 64 ];

            //analyse la ligne 1
            fichier.getline( buf, 32 );
            istringstream i (buf);

            //premier chiffre (largeur)
            i>>mot;
            if(EstUnNumero(mot)){
                largeur = stoi(mot);
            }else{
                cout << "entrez une largeur numérique" << endl;
                exit(-1);
            }

            //deuxième chiffre (longeur)
            i>>mot;
            if(EstUnNumero(mot)){
                longeur = stoi(mot);
            }else{
                cout << "entrez une longeur numérique" << endl;
                exit(-1);
            }

            //les trésors sont exactement à la ligne après la dernière ligne de l'univers
            int ligne_debut_tresor = 2 + longeur;

            Graphe <pair<int,int>, double> graphe;
            Univers<int,double> univers(largeur,longeur);

            for (ligne = 1; fichier; ligne++) {
                //permet de faire i >> mot
                istringstream i (buf);

                while (i >> mot ) {
                    //on doit ignorer la ligne 1 dans le else, puisqu'on l'a déjà analysé
                    if (ligne != 1) {

                        if(ligne < ligne_debut_tresor) {
                            //valide la longeur et les lettres de la string
                            if (!EstUnMotValide(mot, largeur)){
                                exit(-1);
                            }

                            VerifierNombrePorte(mot,ligne,univers);

                            //insère dans l'univers la string
                            for (int i = 0; i < largeur ; i++) {
                                univers.univers[i][ligne -2] = mot[i];

                            }
                            //début des lignes du trésors
                        }else {
                            VerifierCoordonneeTresor(mot, ligne, ligne_debut_tresor, i,univers);
                        }
                    } else {
                        //on a déjà analysé la première ligne
                        i.str(string());
                    }
                }
                // la prochaine ligne
                fichier.getline( buf, 128 );
            }
            if  (EstUnTresorDansEau(univers)){
                cout << "Le trésor est au fond de l'eau!!! Link ne sait pas nager encore!!!" << endl;
                exit (-1);
            }else if(univers.nbr_porte == 0){
                cout << "Il n'y a pas de porte pour que Link puisse entrer!!!" << endl;
                exit (-1);
            } else {
                FloydWarshall(univers, largeur, longeur, graphe);
                AfficherResultats(univers,graphe);
            }
        } else {
            cout << "Impossible d'ouvrir ce fichier." << endl;
            exit(-1);
        }
        fichier.close();
    }else {
        cout << "Entrez un univers sur la ligne de commande." << endl;
        exit(-1);
    }
}

int main(int argc , char* argv []) {
    LectureFichier(argc,argv);
    return 0;
}
