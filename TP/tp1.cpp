#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>


using namespace std;

/**
 * Enregistre le dictionnaire dans un vector si le fichier du dictionnaire est ouvert
 * sinon, affiche un message d'erreur
 * @return deque<string>* dictionnaire
 */
void SauverDictionnaire(vector<string>& dictionnaire) {
    ifstream fichier ;
    fichier.open("words.txt");
    if (!(fichier.is_open())) {
        cout << "Impossible d'ouvrir le fichier. Le fichier n' existe pas." << endl;
    } else {
        string mot;
        while (fichier >> mot ){
            dictionnaire.push_back(mot);
        }
        fichier.close();
    }
}

/**
 * Verifie si le mot original est seulement en majuscule
 * si oui, modifie le mot corrigé seulement en majuscule
 * si non, vérifie si la première lettre du mot est en majuscule
 * si oui, modifie la première lettre du mot corrigé
 * si non, aucun changement
 * @param mot le mot original
 * @param mot_corrige la correction proposée
 * @return mot_corrige la correction avec les majuscules ou non
 */
string VerifierMajuscules(string& mot, string& mot_corrige){
    bool tous_majuscules = true;
    for(int i = 0; i < mot.size(); i++){
        if(!(isupper(mot[i]))){
            tous_majuscules= false;
            i=mot.size();
        }
    }
    if(tous_majuscules){
        for (int i = 0; i < mot_corrige.size(); i++){
            mot_corrige[i] = toupper(mot_corrige[i]);
        }
    }else if(isupper(mot[0])){mot_corrige[0] = (char) toupper(mot_corrige[0]);}

    return mot_corrige;
}

/**
 * Trouve l'indice du mot contenant seulement la lettre recherchée dans le dictionnaire
 * Utilise une recherche binaire
 * @param premiere_lettre  la lettre recherchée
 * @param dictionnaire  le deque du dictionnaire
 * @return indice_debut_lettre_dic = l'indice du mot
 */
int TrouverIndiceLettreDic(char premiere_lettre, const vector<string>& dictionnaire){
    unsigned int j = dictionnaire.size();
    unsigned int i = 0;
    int indice_debut_lettre_dic = 0;

    while (i <= j) {
        if ((dictionnaire[(i+j) / 2])[0] == premiere_lettre) {
            indice_debut_lettre_dic=(i+j)/2;
            string mot_a_l_indice= dictionnaire[indice_debut_lettre_dic];
            if(mot_a_l_indice.length()!=1 || mot_a_l_indice.find('\'') != string::npos){
                j = (i+j) / 2 - 1;
            }else{
                i=dictionnaire.size()+1;
            }
        } else {
            if (premiere_lettre < dictionnaire[(i+j) / 2][0]) {
                j = (i+j) / 2 - 1;
            } else {
                i = (i+j) / 2 + 1;
            }
        }
    }
    return indice_debut_lettre_dic;
}

/**
 * Supprime les lettres doublons dans une string
 * Empêche de parcourir le string mot au complet
 * @param mot le mot à analyser
 * @return str la string sans doublon
 */
string SupprimerLettreDoublon(string& mot){
    string str = mot;
    string::size_type size = str.size();
    string chars;
    unsigned short int index = 0;
    int nbr_caract_enleve = 0;

    for (char c : str) {
        if (chars.find(c) != string::npos) {
            str.erase(str.begin() + index);
            str.resize(size);
            nbr_caract_enleve++;
        }else {
            chars.push_back(c);
        }
        ++index;
    }
    str.resize(size-nbr_caract_enleve);
    return str;
}

/**
 * Supprimer la lettre qui a déjà été itérée dans le dictionnaire
 * Empêche de parcourir deux fois la même lettre
 * @param alphabet les string de lettres qu'on doit itérer dans le dictionnaire
 * @param lettre la lettre qui a été itérée
 * @return str l'alphabet sans la lettre déjà itérée
 */
string SupprimerLettreDeLalphabet(string& alphabet, char& lettre){
    string str = alphabet;

    for (int i = 0; i < alphabet.size(); ++i) {
        str.erase (remove(str.begin(), str.end(), lettre), str.end());
    }
    return str;
}

/**
 * Vérifie si le mot contient soit des lettres, soit des apostrophes
 * @param mot le string mot à valider
 * @return vrai si valide, faux sinon
 */
bool EstUnCaractereValide (const char& lettre){
    return ((lettre <= 'z' && lettre >= 'a') || (lettre <= 'Z' && lettre >= 'A') || lettre == '\'' );
}

/**
 * Vérifie si le mot contient soit des lettres, soit des apostrophes
 * @param mot le string mot à valider
 * @return vrai si valide, faux sinon
 */
bool EstUnMotValide (string& mot){
    bool est_valide = false;
    for (int i = 0 ; i < mot.size(); i++){
        est_valide = (mot[i] <= 'z' && mot[i] >= 'a') || (mot[i] <= 'Z' && mot[i] >= 'A') || (mot[i] == '\'' );
        if(!est_valide) i = mot.size();
    }
    return est_valide;
}

/**
 * Cherche si le mot est présent dans le dictionnaire
 * Utilise une recherche binaire
 * @param mot  le mot recherché
 * @param dictionnaire  le deque du dictionnaire
 * @return vrai si le mot est présent, faux sinon
 */
bool EstDansLeDic (string& mot, const vector<string>& dictionnaire){
    bool reponse = false;
    unsigned int j = dictionnaire.size();
    unsigned int i = 0;

    //transforme le mot en minuscule pour coincider avec les mots du dictionnaire
    transform(mot.begin(), mot.end(), mot.begin(), ::tolower);

    while (i <= j) {
        if (dictionnaire[(i+j) / 2] == mot) {
            i=dictionnaire.size()+1;
            reponse = true;
        } else {
            if (mot < dictionnaire[(i+j) / 2]) {
                j = ((i+j) / 2) - 1;
            } else {
                i = ((i+j) / 2) + 1;
            }
        }
    }
    return reponse;
}

/**
 * Compare deux mots et calcule le nombre de différences minimales à faire pour chaques sous-strings des mots
 * Crée une matrice de x = longeur du premier mot et y = longeur du deuxième mot
 * Analyse si un changement peut être fait par insertion, substitution ou par supression
 * Place dans la matrice la valeur minimum des trois cas
 * @param s1 un string mot
 * @param s2 un string mot à comparer
 * @return la derniere case de la matrice [longeur de s1] [longeur de s2]
 */
int LevenshteinDistance(const string& s1, const string& s2) {
    const size_t len1 = s1.size(), len2 = s2.size();
    vector<vector<int>> matrice(len1 + 1, vector<int>(len2 + 1));

    //insère la longeur des mots sur les axes x et y
    matrice[0][0] = 0;
    for(int i = 1; i <= len1; ++i) matrice[i][0] = i;
    for(int i = 1; i <= len2; ++i) matrice[0][i] = i;

    //insère la distance minimale entre les sous-strings dans chacunes des autres cases
    for(int i = 1; i <= len1; ++i)
        for(int j = 1; j <= len2; ++j)
            matrice[i][j] = min({ matrice[i - 1][j] + 1, matrice[i][j - 1] + 1, matrice[i - 1][j - 1] + (s1[i - 1] == s2[j - 1] ? 0 : 1) });
    return matrice[len1][len2];
}

/**
 * Compare deux mots et calcule le nombre de différences minimales à faire pour chaques sous-strings des mots
 * Crée une matrice de x = longeur du premier mot et y = longeur du deuxième mot
 * Analyse si un changement peut être fait par insertion, substitution, supression ou transposition
 * Place dans la matrice la valeur minimum des trois cas
 * @param string1 un string mot
 * @param string2 un string mot à comparer
 * @return la derniere case de la matrice [longeur de string1] [longeur de string2]
 */
int DamerauLevenshteinDistance(const string& string1,string& string2) {
    const size_t len1 = string1.size(), len2 = string2.size();
    int matrice[len1+1][len2+1];

    //insère la longeur des mots sur les axes x et y
    matrice[0][0] = 0;
    for(int i = 1; i <= len1; ++i) matrice[i][0] = i;
    for(int i = 1; i <= len2; ++i) matrice[0][i] = i;

    //insère la distance minimale entre les sous-strings dans chacunes des autres cases
    for(int i = 1; i <= len1; ++i){
        for(int j = 1; j <= len2; ++j){
                                //supression            //insertion             //substitution
            matrice[i][j] = min({ matrice[i - 1][j] + 1, matrice[i][j - 1] + 1, matrice[i - 1][j - 1] + (string1[i - 1] == string2[j - 1] ? 0 : 1) });

            if( (i > 1) && (j > 1) && (string1[i] == string2[j-1]) && (string1[i-1] == string2[j])) {
                matrice[i][j] = min(matrice[i][j], matrice[i-2][j-2] + (string1[i - 1] == string2[j - 1] ? 0 : 1));   // transposition
            }
        }
    }
    return matrice[len1][len2];
}

/**
 * Itère sur les mot du dictionnaire commençant par une lettre déterminée en comparant le mot à corriger
 * Calcule la distance entre les mots du dictionnaire et le mot à corriger  (si metrique secondaire = vrai: distance = Damerau-Levenshtein, sinon: distance =  Levenshtein)
 * Place dans un vector les solutions possible
 * @param mot   le mot à corriger
 * @param dictionnaire le vector contenant le dictionnaire
 * @param correction_possible le vector contenant les corrections possibles
 * @param indice_debut_lettre_dic  l'indice où commence l'itération du dictionnaire
 * @param indice_fin_lettre_dic   l'indice où doit s'arrêter l'itération du dictionnaire
 * @param plus_petite_difference  la plus petite distance entre deux mots
 * @param difference la distance entre deux mots
 * @param metrique_second si vrai, calcule la distance DamerauLevenshtein, sinon, calcule la distance Levensthein
 */
void ItererSurUneLettreDuDic (string& mot,vector<string>& dictionnaire, vector<string>& correction_possible, int& indice_debut_lettre_dic, int& indice_fin_lettre_dic,int& plus_petite_difference,int& difference,bool metrique_second ){
    //on itère sur les mots du dictinnaire commençant par la première lettre du mot recherché
    for(int i = indice_debut_lettre_dic; i <= indice_fin_lettre_dic; ++i){
        string mot_a_comparer_backup = dictionnaire [i];
        string mot_a_comparer = SupprimerLettreDoublon(mot_a_comparer_backup);

        //certains mots du dictionnaire contiennent des caractères invalides
        bool est_un_mot_valide = EstUnMotValide(mot_a_comparer);

        if (est_un_mot_valide) {
            //le mot doit contenir une lettre du mot à comparer mise à part la première
            for(int j = 1; j < mot_a_comparer.size(); j++){
                char une_lettre_du_mot = mot_a_comparer[j];
                bool doit_etre_compare = false;
                int difference_taille = mot.size() - mot_a_comparer.size();

                if( mot.find(une_lettre_du_mot) != string::npos){
                    doit_etre_compare = true;
                    //inutile d'itérer sur les autres lettres
                    j = mot_a_comparer.size();
                    // le mot doit contenir maximum 2 lettres de plus et maximum 2 lettres de moins
                    if(!(difference_taille < 3 && difference_taille > -3)){
                        doit_etre_compare=false;
                    }
                }
                if (doit_etre_compare) {

                    if (metrique_second) {
                        difference = DamerauLevenshteinDistance(mot, mot_a_comparer_backup);
                    }else{
                        difference = LevenshteinDistance(mot, mot_a_comparer_backup);
                    }
                    if (!(metrique_second)) {
                        //on efface la liste de correction si la différence est plus petite
                        if (difference < plus_petite_difference) {
                            correction_possible.clear();
                            correction_possible.push_back(mot_a_comparer_backup);
                            plus_petite_difference = difference;

                        // on ajoute le mot à la liste si la différence est égale
                        }else if (difference == plus_petite_difference) {
                            correction_possible.push_back(mot_a_comparer_backup);
                        }
                    }else {
                        //on efface la liste de correction si la différence est plus petite
                        if (difference < plus_petite_difference) {
                            plus_petite_difference = difference;
                        }
                        // on ajoute le mot à la liste si la différence est égale
                        if (difference == 1) {
                            correction_possible.push_back(mot_a_comparer_backup);
                        }
                    }
                }
            }
        }
    }
}

/**
 * Tri customisé selon l'ordre des lettres du mot à corrigé
 * ex: si mot à corriger = bleach alors beach < bar
 * @param mot le mot du dictionnaire à corriger
 * @param dictionnaire le vector contenant le dictionnaire
 * @param correction_possible le vector contenant les corrections possibles
 * @return mot_corrige la meilleure correction possible
 */
string CorrectionAcceptable (string& mot,vector<string>& dictionnaire, vector<string>& correction_possible){
    string mot_corrige;

    struct compare {
        bool operator()(const string& mot_du_vector, const string& mot) {
            return mot_du_vector.size() == mot.size();
        }
    };
    compare c;

    sort(correction_possible.begin(),correction_possible.end(),c);
    mot_corrige = correction_possible[0];

    return mot_corrige;
}

/**
 *
 * @param mot le mot à corriger
 * @param dictionnaire le vector contenant le dictionnaire
 * @param correction_possible le vector contenant les corrections possibles
 * @param metrique_second la metrique secondaire activée ou non
 * @return mot_corrige la meilleure correction possible
 */
string CorrigerMot (string& mot,vector<string>& dictionnaire, vector<string>& correction_possible,bool metrique_second){
    string mot_corrige;
    string mot_original = mot;

    bool est_dans_le_dic = EstDansLeDic(mot,dictionnaire);
    if(! est_dans_le_dic){
        char premiere_lettre = mot[0];

        //on trouve les indices de début et de fin dans le dictionnaire de la première lettre du mot recherché
        int indice_debut_lettre_dic = TrouverIndiceLettreDic(premiere_lettre, dictionnaire);
        //on doit comparer le mot suivant la lettre du dictionnaire
        indice_debut_lettre_dic++;

        int indice_fin_lettre_dic;
        if (premiere_lettre == 'z') {
            indice_fin_lettre_dic = dictionnaire.size()-1;
        } else {
            indice_fin_lettre_dic = TrouverIndiceLettreDic((char) (premiere_lettre + 1), dictionnaire);
            indice_fin_lettre_dic--;
        }

        //le plus grand mot anglais possède 45 lettres pour un maximum de différence de 45
        int plus_petite_difference = 45;
        int difference;

        ItererSurUneLettreDuDic(mot,dictionnaire,correction_possible,indice_debut_lettre_dic,indice_fin_lettre_dic,plus_petite_difference,difference,metrique_second);

        //le cas où il n'y aurait pas de difference de 1 avec la première lettre
        if (plus_petite_difference > 1) {
            //on itere sur l'alphabet placé dans l'ordre des premières lettres des mots les plus utilisés moins la lettre déjà comparée
            string alphabet =  "abcdefghijklmnopqrstuvwy";
            alphabet = SupprimerLettreDeLalphabet(alphabet,premiere_lettre);

            for (char c : alphabet) {
                indice_debut_lettre_dic = TrouverIndiceLettreDic(c, dictionnaire);
                //on doit comparer le mot suivant la lettre du dictionnaire
                indice_debut_lettre_dic++;
                indice_fin_lettre_dic = TrouverIndiceLettreDic((char) (c + 1), dictionnaire);
                //on doit comparer le mot avant la prochaine lettre du dictionnaire
                indice_fin_lettre_dic--;
                ItererSurUneLettreDuDic(mot, dictionnaire, correction_possible, indice_debut_lettre_dic,
                                        indice_fin_lettre_dic, plus_petite_difference, difference,metrique_second);
            }
        }
        //dans le cas où il n'y aurait pas de différence de 1, le mot original est affiché
        //sinon, on analyse le tableau de correction
        if(correction_possible.size()==0){
            mot_corrige = mot_original;
        }else {
            if (metrique_second) {
                //on analyse le tableau de corrections s'il propose plusieurs corrections
                if (correction_possible.size() > 1) {
                    mot_corrige = CorrectionAcceptable(mot, dictionnaire, correction_possible);
                }else {
                    mot_corrige = correction_possible[0];
                }
                mot_corrige = VerifierMajuscules(mot_original, mot_corrige);
            }else {
                //sans la metrique secondaire, on classe le tableau alphabétiquement et on remet les majuscules si nécéssaire
                sort(correction_possible.begin(), correction_possible.end());
                mot_corrige = correction_possible[0];
                mot_corrige = VerifierMajuscules(mot_original, mot_corrige);
            }
        }
    }else{
        mot_corrige = mot_original;
    }
    return mot_corrige;
}

/**
 * Appel récursif pour corriger un mot ayant des caractères indésirables
 * Crée une sous-chaîne du mot jusqu'au caractère fautif
 * Affiche le caractère fautif entre deux espaces
 * Analyse la sous-chaîne du mot à partir du caractère suivant le caractère fautif
 * @param mot le string mot à corriger
 * @param dictionnaire le vector contenant le dictionnaire
 * @param correction_possible le vector contenant les corrections possibles
 * @param metrique_second la metrique secondaire activée ou non
 */
void CorrigerMotAvecAutreCaract (string& mot,vector<string>& dictionnaire, vector<string>& correction_possible,bool metrique_second){
    string nouveau_mot;
    string mot_corrige;
    
    //condition de fin de recursion
    if (mot != "" && mot.size()>1) {
        //si le premier caractère du mot est un caractère invalide, on affiche
        if (!(EstUnCaractereValide(mot[0]))) {
            cout << mot [0];
            //puis on crée une sous-chaîne du mot à partir du caractère valide
            nouveau_mot = mot.substr(1,mot.size());
            CorrigerMotAvecAutreCaract(nouveau_mot, dictionnaire, correction_possible,metrique_second);
        }else {
            for(int i = 0 ; i < mot.size() ; i++){
                //on trouve s'il y a un autre caractère invalide
                if(!(EstUnCaractereValide(mot[i]))){
                    string sous_mot = mot.substr(0,i);
                    nouveau_mot = mot.substr(i+1,mot.size());
                    mot_corrige = CorrigerMot(sous_mot, dictionnaire, correction_possible,metrique_second);
                    if (EstUnCaractereValide(mot[i+1])) {
                        cout << mot_corrige << "" << mot[i] << " ";
                    }else{
                        //si le prochain caractère apres la sous-chaîne est un autre caractère invalide, il n'y a pas d'espace
                        cout << mot_corrige << "" << mot[i];
                    }
                    i = mot.size();

                }
            }
            //si le mot après le caractère invalide peut-être corrigé
            if (EstUnMotValide(nouveau_mot)){
                mot_corrige = CorrigerMot(nouveau_mot, dictionnaire, correction_possible,metrique_second);
                cout << mot_corrige << " " ;
            //sinon recursion    
            }else{
                CorrigerMotAvecAutreCaract(nouveau_mot, dictionnaire, correction_possible,metrique_second);
            }
        }
    }else{
        //le mot contient seulement une lettre
        cout << mot <<" ";
    }
}

/**
 * Trouve quel argument contient le flas -s
 * Si trouvé, active la métrique secondaire
 * @param argc le nombre d'argument
 * @param argv le pointeur du tableau de caractères des arguments
 * @param fichier_a_corriger le fichier qui doit être corrigé en string
 * @param flag_s la métrique secondaire en string
 * @return vrai si la metrique secondaire contient "-s", faux sinon
 */
bool TrouverNomFichier (int argc,char* argv [], string& fichier_a_corriger, string& flag_s){
    bool metrique_second = true;
    if (fichier_a_corriger.find("-s") != string::npos){
        fichier_a_corriger = argv [2];
        flag_s = argv[1];
    }
    return metrique_second;
}

/**
 * si ouvert, lit le fichier à corriger par l'utilisateur, sinon affiche un message d'erreur
 * si le mot contient des lettres et des apostrophes, analyse du mot
 * sinon, sépare le mot en sous-string et analyse les sous-string
 * @param argv le pointeur de pointeurs aux arguments de la ligne de commande
 * @param argc le nombre d'arguments
 */
void LectureFichier(int argc,char* argv []) {
    ifstream fichier ;
    string fichier_a_corriger = argv [1];
    bool metrique_second=false;
    if (argc >2) {
        if (argv[1]== string("-s") || argv[2] == string("-s") ) {
            string flag_s = argv [2];
            metrique_second = TrouverNomFichier(argc,argv, fichier_a_corriger, flag_s);
        }else{
            cout << "mauvais flag" <<endl;
        }
    }
    fichier.open(fichier_a_corriger);
    if (fichier.is_open()) {
        string mot;
        string mot_corrige;
        vector<string> dictionnaire ;
        vector<string> correction_possible;
        SauverDictionnaire(dictionnaire);

        while (fichier >> mot ){
            string mot_original = mot;

            //si le mot contient seulement des lettres et des apostrophes
            bool est_un_mot_valide = EstUnMotValide(mot);
            if (est_un_mot_valide) {
                mot_corrige = CorrigerMot(mot, dictionnaire, correction_possible,metrique_second);
                correction_possible.clear();
                cout <<  mot_corrige << " ";

            //si le mot contient des caractères indésirables
            }else {
                CorrigerMotAvecAutreCaract(mot, dictionnaire, correction_possible,metrique_second);
            }
        }
    } else cout << "Impossible d'ouvrir le fichier. Le fichier n' existe pas." << endl;
    fichier.close();
}

int main(int argc , char* argv []) {
	
    LectureFichier(argc,argv);
    return 0;
}

