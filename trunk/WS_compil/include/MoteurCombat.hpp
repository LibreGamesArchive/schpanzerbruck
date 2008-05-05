#ifndef MOTEUR_COMBAT_HEADER
#define MOTEUR_COMBAT_HEADER

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <GL/gl.h>

#include "MoteurJeu.hpp"
#include "MapClient.hpp"
#include "GestionnaireImages.hpp"

using namespace std;

struct DonneesMap
{
    int largeur;
    int hauteur;
    int* numsTuiles;
    int* numsElements;
    char** cheminsTuiles;
    char** cheminsElements;
    int numTexBordure;
    int hauteurBordure;
};

struct Touches
{
    int zoomAvant;
    int zoomArriere;
};


class MoteurCombat      // Synchronise la Map et l'Interface
{
private:
    sf::RenderWindow* app;
    int L, H;
    Touches touches;
    GestionnaireImages* gestImages;
    MapClient* map;
    Camera* camera;

    int vitesseDefil, bordureDefil;
    bool elemsON;

    void scrolling(int curseurX, int curseurY);

    friend class MoteurJeu;

    MoteurCombat(sf::RenderWindow* _app, DonneesMap _DM, Touches _touches);

public:
    ~MoteurCombat();

    void centrerCurseur();

    void afficher();

    bool gestionClavierSouris();

    float FPS();
};

#endif
