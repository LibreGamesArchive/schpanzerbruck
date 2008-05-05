#ifndef MOTEUR_JEU_HEADER
#define MOTEUR_JEU_HEADER

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "MoteurCombat.hpp"


class MoteurJeu
{
private:
    sf::RenderWindow* app;

public:
    MoteurJeu(int appL, int appH, int bpp, bool fullscreen, bool synchroVert, int fpsMax=-1);
    
    ~MoteurJeu();
    
    MoteurCombat obtenirMoteurCombat(DonneesMap DM, Touches touches);
};

#endif
