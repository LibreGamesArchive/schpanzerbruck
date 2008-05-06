#ifndef MOTEUR_COMBAT_HEADER
#define MOTEUR_COMBAT_HEADER

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <GL/gl.h>

#include <MoteurJeu.hpp>
#include <MapClient.hpp>
#include <GestionnaireImages.hpp>

namespace ws
{
struct DonneesMap
{
    unsigned int largeur;
    unsigned int hauteur;
    int* numsTuiles;
    int* numsElements;
    char** cheminsTuiles;
    char** cheminsElements;
    unsigned int numTexBordure;
    float hauteurBordure;
};

struct Touches
{
    sf::Key::Code zoomAvant;
    sf::Key::Code zoomArriere;
};

class MoteurJeu;

class MoteurCombat      // Synchronise la Map et l'Interface
{
private:
    sf::RenderWindow* app;
    unsigned int L, H, curseurX, curseurY;
    Touches touches;
    GestionnaireImages* gestImages;
    MapClient* map;
    Camera* camera;

    unsigned int vitesseDefil, bordureDefil;
    bool elemsON;

    void scrolling();

    friend class MoteurJeu;

    MoteurCombat(sf::RenderWindow* _app, DonneesMap _DM, Touches _touches);

public:
    ~MoteurCombat();

    void centrerCurseur();

    void afficher();

    bool gestionClavierSouris();

    float FPS();
};
}

#endif

