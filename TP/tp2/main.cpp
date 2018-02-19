/*  INF3105 | Structures de données et algorithmes
 *  UQAM | Département d'informatique | Automne 2016
 *  main pour le  TP2
 *  Julien Guité-Vinet GUIJ 09058407
 *  Bruno Gillard-Ouellette GILB 06058305
 */
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>
#include "ArbreMap.h"

using namespace std;

/**
 * Enregistre les mots réservés dans un vector si le fichier du dictionnaire est ouvert
 * sinon, affiche un message d'erreur
 * @return deque<string>* dictionnaire
 */
void SauverKeywords(vector<string>& mots_reserves) {
    ifstream fichier ;
    fichier.open("reserved_keywords.txt");

    if (!(fichier.is_open())) {
        cout << "Impossible d'ouvrir le fichier. Le fichier n' existe pas.(erreur keywords)" << endl;
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
    int j = dictionnaire.size()-1;
    int i = 0;

    while (i <= j) {
        if (dictionnaire[(i+j) / 2] == mot) {
            string test = dictionnaire[(i+j) / 2];
            i=dictionnaire.size()+1;
            reponse = true;
        } else {
            if (mot < dictionnaire[(i+j) / 2]) {
                string test = dictionnaire[(i+j) / 2];
                j = ((i+j) / 2) - 1;
            } else {
                string test = dictionnaire[(i+j) / 2];
                i = ((i+j) / 2) + 1;
            }
        }
    }
    return reponse;
}

/**
 * Vérifie si le mot contient le string "/*"
 * Initialise  un booléen a true si oui
 * Verifie si le mot contient le string pour fin de commentaire
 * Initialise un booléan a false si oui
 * Retourne les index du caractère étoile par référence
 * @param mot le mot à analyser
 * @param index_etoile l'index du caractère étoile dans le mot
 * @param index_fin_etoile l'index du caractère étoile dans le mot pour fin de caractère
 * @param fin_du_commentaire vrai si le mot contient "etoile slash", faux sinon
 * @return vrai si le mot contient "/*".
 */
bool EstUnCommentaireEtoile (const string& mot, int& index_etoile, int& index_fin_etoile, bool& fin_du_commentaire){

    bool est_un_commentaire = false;
    size_t fin_commentaire = mot.find("*/");
    size_t debut_commentaire = mot.find("/**");
    if (debut_commentaire != string::npos){
        est_un_commentaire = true;
        fin_du_commentaire = false;
        index_etoile = debut_commentaire;
    }
    if ( fin_commentaire != string::npos){
        index_fin_etoile = fin_commentaire;
        fin_du_commentaire = true;
    }
    return est_un_commentaire;
}

/**
 * Vérifie si une lettre est alphanumérique
 * @param c une lettre
 * @return vrai si la lettre est alphanumérique
 */
static inline bool EstAlphanumerique(char c) { return !isalnum(c); }

bool EstUnMotAlphanumerique(const string &str) {
    return find_if(str.begin(), str.end(), EstAlphanumerique) == str.end(); }

/**
 * Analyse si le mot contient le char '/' au caractère 0 et 1
 * @param mot le mot à analyser
 * @return vrai si le mot contient '/' au caractère 0 et 1, faux sinon
 */
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
bool EstUnCaractereValide (const char& lettre){ return ((lettre <= 'z' && lettre >= 'a') || (lettre <= 'Z' && lettre >= 'A') || lettre == '_' ); }

/**
 * Vérifie si le mot contient soit des lettres, soit des underscores, soit des chiffres
 * @param mot le string mot à valider
 * @return vrai si valide, faux sinon
 */
bool EstUnMotValide (string& mot){

    bool est_valide = false;
    if (EstUnCaractereValide(mot[0])) {
        for (int i = 0; i < mot.size(); i++) {
            est_valide = (mot[i] <= 'z' && mot[i] >= 'a') || (mot[i] <= 'Z' && mot[i] >= 'A') || (mot[i] == '_') || (mot[i] >= 0 && mot[i] <= 9 );
            if (!est_valide) i = mot.size(); //fin de boucle
        }
    }
    return est_valide;
}

/**
 * Affiche les resultats selon la métrique secondaire
 * si activée, exporte les résultats dans un fichier externe, si non affiche à la console
 * @param metrique_second booléen activé à vrai avec argc suppérieur à 1
 * @param arbre la structure où l'on sauvegarde les données
 * @param nom_fichier_export le nom du fichier où l'on doit exporter les résultats
 */
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
                    //un virgule entre chaque chiffre    sauf pour le dernier chiffre
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

/**
 * Enlève le dernier caractère d'un string
 * @param mot le mot à transformer
 * @return le mot sans le derniere caractère
 */
string SubstringDernierCaract (string& mot){

    int position_dernier_caract = mot.size()-1;
    char dernier_caract = mot[position_dernier_caract];

    if (!(EstUnCaractereValide(dernier_caract))){
        //si le dernier caractère est une parathèse fermante, c'est peut-être une fonction.
        if (dernier_caract != ')'){
            mot = mot.substr(0,position_dernier_caract);
         //on analyse alors l'avant dernier caractère
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

/**
 * Analise si le mot est valide et s'il n est pas dans le dictionnaire.
 * Si oui, ajouter le mot à l'arbreAVL.
 * @param mot le mot à analyser.
 * @param arbre l'arbre où l'on garde les mots enregistrés.
 * @param ligne le numéro de ligne où se trouve le mot.
 * @param mots_reserves le vector où sont enregistrés les mots réservés.
 */
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
    //aucun fichier à corriger
    if (argc > 1) {
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
            size_t found=0;
            vector<string> mots_reserves;
            ArbreMap<string,vector<int>> arbre;
            int ligne;
            bool est_un_commentaire_slash = false;
            bool est_un_commentaire_etoile = false;
            bool est_un_commentaire_guillemet = false;
            bool hashtag_line = false;

            SauverKeywords(mots_reserves);

            for (ligne = 1;fichier; ligne++) {
                char buf[ 511 ];
                fichier.getline( buf, 256 );
                istringstream i (buf);

                while (i >> mot ){

                    if (!hashtag_line) {
                        found = mot.find("#line");
                        if (found != string::npos){
                            hashtag_line = true;
                        }
                    }else{
                        ligne = stoi(mot);
                        hashtag_line = false;
                    }
                    //inutile de rechercher les commentaires si le mot est alphanumérique ou est un hashtag_line
                    if (!(EstUnMotAlphanumerique(mot)) && !hashtag_line) {
                        found = mot.find("//");
                        if (found != string::npos){est_un_commentaire_slash = true;}
                        //inutile de rechercher le debut du commentaire_etoile si un commentaire_slash ou un commentaire_etoile est courrant
                        if (!est_un_commentaire_slash && !est_un_commentaire_etoile){
                            found = mot.find("/*");
                            if (found != string::npos){est_un_commentaire_etoile = true;}
                        }
                        //recherche la fin du commentaire_etoile seulement s'il y a eu un debut de commentaire_etoile
                        if (est_un_commentaire_etoile){
                            found = mot.find("*/");
                            if (found != string::npos){est_un_commentaire_etoile = false;}
                        }
                        //inutile de rechercher le commentaire_guillemet si un commentaire_slah ou un commentaire_etoile est courrant
                        if (!est_un_commentaire_etoile && !est_un_commentaire_slash){
                            found = mot.find("\"");
                            if(found != string::npos){ est_un_commentaire_guillemet = !est_un_commentaire_guillemet; }
                            //possible que la fin du commentaire_guillemet soit dans le même mot
                            if (est_un_commentaire_guillemet) {
                                found = mot.find("\"", found+1);
                                if(found != string::npos){ est_un_commentaire_guillemet = !est_un_commentaire_guillemet; }
                            }
                        }
                    }
                    //analyse seulement si il n'y a pas de commentaires
                    if (!est_un_commentaire_slash && !est_un_commentaire_etoile  && !est_un_commentaire_guillemet && !hashtag_line) {
                        AnalyseMot(mot,arbre,ligne,mots_reserves);
                    }else {
                        //le commentaire_slash annule toute une ligne
                        if (est_un_commentaire_slash) {
                            i.str(string());
                            est_un_commentaire_slash = false;
                        }
                    }
                }
            }

            AffichageResultat(metrique_second,arbre, nom_fichier_export);

        } else cout << "Impossible d'ouvrir le fichier. Le fichier n' existe pas." << endl;\

        fichier.close();
    }else {
        cout << "Entrez un fichier à corriger sur la ligne de commande." << endl;\
    }
}

int main(int argc , char* argv []) {

    LectureFichier(argc,argv);
    return 0;
}

