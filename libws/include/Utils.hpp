#ifndef PERSO_GRAPH_HEADER
#define PERSO_GRAPH_HEADER

namespace ws
{

struct CouleurEquipe
{
    int R, V, B;
};

struct InfosSuccintesSurPerso
{
    string nom;
    float VIE, FTG;
};

struct PersoGraphique   // Ne contient que des donnees visuelles sur le perso (i.e. uniquement des trucs qui servent pour le dessiner)
{
    int pos;    // Numéro de la case sur laquelle se trouve le perso
    CouleurEquipe clr;
};

enum {
    TUILE, ELEMENT, PERSO
};

enum {
    GRADE_E, GRADE_D, GRADE_C, GRADE_B, GRADE_A,
    RAS, QUITTER, MAITRISES_CHOISIES, CASE_CHOISIE, CIBLE_CHOISIE, LISTE_MAITRISES_DEMANDEE,
    INFOS_DETAILLEES_DEMANDEES,     // Ces constantes sont les seules utiles pour un utilisateur de WS
    
    // Interface
    SLC_MENU_ECHAP, SLC_CONTINUER, SLC_QUITTER,
    SLC_MENU_TRIANGLE, SLC_DEPLACEMENT, SLC_ACTION, SLC_FIN_DU_TOUR,
    SLC_FENETRE_MAITRISES, SLC_FERMER_MAITRISES, SLC_VALIDER_MAITRISES, SLC_LISTE_MAITRISES, SLC_MAITRISE,
    
    // Map
    PAS_DE_SELECTION, INFOS_SEULEMENT, DEPLACEMENT, CIBLAGE
};

}

#endif
