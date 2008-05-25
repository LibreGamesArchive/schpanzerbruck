#ifndef MOTEUR_COMBAT_HEADER
#define MOTEUR_COMBAT_HEADER

#include "MoteurJeu.hpp"
#include "MapGraphique.hpp"
#include "InterfaceCombat.hpp"
#include "GestionnaireImages.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <GL/gl.h>

#define IC  InterfaceCombat


namespace ws
{

struct Touches
{
    sf::Key::Code zoomAvant;
    sf::Key::Code zoomArriere;
};

class MoteurJeu;

class EvenementCombat;

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
    
    void traiterSelectInterface(int* selec, bool clic, unsigned int& whatHappens);

public:
    enum {
        RAS, QUITTER, MAITRISES_CHOISIES, CASE_CHOISIE, CIBLE_CHOISIE, LISTE_MAITRISES_DEMANDEE, INFOS_DETAILLEES_DEMANDEES
    };
    
    ~MoteurCombat();
    
    void centrerCurseur();
    
    void afficher();
    
    unsigned int traiterEvenements();
    
    float getFPS();
    
    int* selectMapActuelle();
    int* maitrisesChoisies();
    
    void setInfosDsBarre(string tuile="", string element="", string perso="");
    void setChrono(string temps="");
};

}

#endif
