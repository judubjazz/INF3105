#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <sstream>
#include "ArbreMap.h"

using namespace std;

/**
 * Enregistre le dictionnaire dans un vector si le fichier du dictionnaire est ouvert
 * sinon, affiche un message d'erreur
 * @return deque<string>* dictionnaire
 */
void SauverKeywords(vector<string>& mots_reserves) {
    ifstream fichier ;
    fichier.open("reserved_keywords");

    if (!(fichier.is_open())) {
        cout << "Impossible d'ouvrir le fichier. Le fichier n' existe pas." << endl;
    } else {
        string mot;
        while (fichier >> mot ){
            mots_reserves.push_back(mot);
        }
        sort(mots_reserves.begin(),mots_reserves.end());
        fichier.close();
    }
}

/**
 * Cherche si le mot est présent dans le dictionnaire
 * Utilise une recherche binaire
 * @param mot  le mot recherché
 * @param dictionnaire  le deque du dictionnaire
 * @return vrai si le mot est présent, faux sinon
 */
bool EstDansLeDic (const string& mot, const vector<string>& dictionnaire){
    bool reponse = false;
    int j = dictionnaire.size();
    int i = 0;


    while (i < j) {
        if (dictionnaire[(i+j) / 2] == mot) {
            string test= dictionnaire[(i+j) / 2];
            i=dictionnaire.size()+1;
            reponse = true;
        } else {
            if (mot < dictionnaire[(i+j) / 2]) {
                string test= dictionnaire[(i+j) / 2];
                j = ((i+j) / 2) - 1;
                int k = (i+j)/2;
                int test4 = 0;
            } else {
                string test= dictionnaire[(i+j) / 2];
                i = ((i+j) / 2) + 1;
            }
        }
    }
    return reponse;
}
bool EstUnCommentaireEtoile (const string& mot, int& index_etoile, int& index_fin_etoile, bool& fin_du_commentaire){
    bool est_un_commentaire = false;
    size_t fin_commentaire = mot.find("\\*");
    size_t debut_commentaire = mot.find("/*");
    if (debut_commentaire != string::npos){
        est_un_commentaire = true;
        index_etoile = debut_commentaire;
    }
    if ( fin_commentaire != string::npos){
        index_fin_etoile = fin_commentaire;
        fin_du_commentaire = true;
    }
    return est_un_commentaire;
}

bool EstUnCommentaire (const string& mot){
    bool est_un_commentaire = false;
    char premier_caract = mot[0];
    if ( premier_caract == '/'){
        char deuxieme_caract = mot[1];
        if(deuxieme_caract == '/'){
            est_un_commentaire = true;
        }
    }
    return est_un_commentaire;
}

/**
 * Vérifie si le mot contient soit des lettres, soit des underscores
 * @param mot le string mot à valider
 * @return vrai si valide, faux sinon
 */
bool EstUnCaractereValide (const char& lettre){
    return ((lettre <= 'z' && lettre >= 'a') || (lettre <= 'Z' && lettre >= 'A') || lettre == '_' );
}

/**
 * Vérifie si le mot contient soit des lettres, soit des underscores, soit des chiffres
 * @param mot le string mot à valider
 * @return vrai si valide, faux sinon
 */
bool EstUnMotValide (string& mot){
    bool est_valide = false;
    bool test = EstUnCaractereValide(mot[0]);
    if (test) {
        for (int i = 0; i < mot.size(); i++) {
            est_valide = (mot[i] <= 'z' && mot[i] >= 'a') || (mot[i] <= 'Z' && mot[i] >= 'A') || (mot[i] == '_') || (mot[i] >= 0 && mot[i] <= 9 );
            if (!est_valide) i = mot.size(); //fin de boucle
        }
    }
    return est_valide;
}

void AffichageResultat (bool& metrique_second, ArbreMap<string,vector<int>>& arbre, string& nom_fichier_export){

    ArbreMap<string,vector<int>>::Iterateur iter = arbre.debut();

    if (metrique_second) {
        ofstream fichier_export;
        fichier_export.open(nom_fichier_export);
        if (fichier_export.is_open()) {
            while (iter) {
                string s = iter.cle();
                vector<int> v = iter.valeur();
                fichier_export << iter.cle() << " : ";
                for (int i = 0; i < v.size(); i++) {
                    //un virgule entre chaque chiffre    sauf pour le dernier
                    i < v.size() - 1 ? fichier_export << v[i] << ", " : fichier_export << v[i];
                }
                fichier_export << endl;

                ++iter;
            }
        }else {
            cout << "Impossible d'écrire sur ce fichier.";
        }
    } else {
        while (iter) {
            string s = iter.cle();
            vector<int> v = iter.valeur();
            cout << iter.cle() << " : ";
            for (int i = 0; i < v.size(); i++) {
                //un virgule entre chaque chiffre    sauf pour le dernier
                i < v.size() - 1 ? cout << v[i] << ", " : cout << v[i];
            }
            cout << endl;

            ++iter;
        }
    }
}

string SubstringDernierCaract (string& mot){
    int position_dernier_caract = mot.size()-1;
    char dernier_caract = mot[position_dernier_caract];

    if (!(EstUnCaractereValide(dernier_caract))){
        if (dernier_caract != ')'){
            mot = mot.substr(0,position_dernier_caract);
        }else {
            char avant_dernier_caract = mot[position_dernier_caract -1];
            if (avant_dernier_caract == '('){
                mot = mot.substr(0,position_dernier_caract -1);
            }else {
                mot = mot.substr(0,position_dernier_caract);
            }
        }

    }
    return mot;
}

void AnalyseMot (string& mot, ArbreMap<string,vector<int>>& arbre, int& ligne, const vector<string>& mots_reserves){
    mot = SubstringDernierCaract(mot);

    //si le mot contient seulement des lettres et des underscores ou n'est pas un mot reservé
    bool est_un_mot_valide = EstUnMotValide(mot);
    bool est_un_mot_reserve = EstDansLeDic(mot,mots_reserves);

    if (est_un_mot_valide && !est_un_mot_reserve) {
        arbre[mot].push_back(ligne);
    }
}
/**
 * si ouvert, lit le fichier à corriger par l'utilisateur, sinon affiche un message d'erreur
 * si le mot contient des lettres et des apostrophes, analyse du mot
 * sinon, sépare le mot en sous-string et analyse les sous-string
 * @param argv le pointeur de pointeurs aux arguments de la ligne de commande
 */
void LectureFichier(int argc,char* argv []) {
    ifstream fichier ;
    string fichier_a_corriger = argv [1];
    string nom_fichier_export;
    bool metrique_second=false;
    if (argc >2) {
        metrique_second = true;
        nom_fichier_export = argv [2];
    }

    fichier.open(fichier_a_corriger);
    if (fichier.is_open()) {
        string mot;
        vector<string> mots_reserves;
        ArbreMap<string,vector<int>> arbre;
        int ligne;
        bool est_un_commentaire;
        bool est_un_commentaire_etoile;
        bool fin_du_commentaire = false;
        int index_etoile;
        int index_fin_etoile = -1;

        SauverKeywords(mots_reserves);

        for (ligne = 1;fichier; ligne++) {
            char buf[ 255 ];
            fichier.getline( buf, 128 );
            istringstream i (buf);

            while (i >> mot ){
                est_un_commentaire = EstUnCommentaire(mot);
                est_un_commentaire_etoile = EstUnCommentaireEtoile(mot, index_etoile, index_fin_etoile, fin_du_commentaire );

                if (est_un_commentaire_etoile && fin_du_commentaire){
                    if (index_etoile == 0){
                        est_un_commentaire = true;
                    }else {
                        string mot_backup = mot;
                        mot = mot.substr(0,index_etoile);
                        AnalyseMot(mot,arbre,ligne,mots_reserves);
                        int test = mot_backup.size();
                        if (index_fin_etoile <= mot_backup.size()){
                            mot = mot_backup.substr(index_fin_etoile + 2,mot_backup.size());
                            AnalyseMot(mot,arbre,ligne,mots_reserves);
                        }
                    }
                }

                if (!est_un_commentaire && !est_un_commentaire_etoile && fin_du_commentaire) {
                    AnalyseMot(mot,arbre,ligne,mots_reserves);
                }else {
                    i.str(string());
                    est_un_commentaire = false;
                }
            }
        }

        AffichageResultat(metrique_second,arbre, nom_fichier_export);

    } else cout << "Impossible d'ouvrir le fichier. Le fichier n' existe pas." << endl;\

    fichier.close();
}

int main(int argc , char* argv []) {

    LectureFichier(argc,argv);
    return 0;
}

