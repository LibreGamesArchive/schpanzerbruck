#ifndef MOTEUR_COMBAT_HEADER
#define MOTEUR_COMBAT_HEADER

#include "MoteurJeu.hpp"
#include "MapGraphique.hpp"
#include "InterfaceCombat.hpp"
#include "GestionnaireImages.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <GL/gl.h>


namespace ws
{
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
    MapGraphique* mapGraph;
    InterfaceCombat* gui;
    Camera* camera;

    unsigned int vitesseDefil, bordureDefil;
    bool elemsON;

    void scrolling();

    friend class MoteurJeu;

    MoteurCombat(sf::RenderWindow* _app, GestionnaireImages* _gestImages, const DonneesMap& _DM, const Touches& _touches);

    void traiterSelecMap(int* selec, bool clic, bool& running);
    void traiterSelecInterface(int* selec, bool clic, bool& running);

public:
    ~MoteurCombat();

    void centrerCurseur();

    void afficher();

    bool traiterEvenements();

    float getFPS();
};
}

#endif
