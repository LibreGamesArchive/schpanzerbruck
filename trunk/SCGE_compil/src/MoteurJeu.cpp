#include "../include/MoteurJeu.hpp"


MoteurJeu::MoteurJeu(int appL, int appH, int bpp, bool fullscreen, bool synchroVert, int fpsMax)
{
    int style = sf::Style::Close;
    if(fullscreen)
        style |= sf::Style::Fullscreen;
    app = new sf::RenderWindow(sf::VideoMode(appL, appH, bpp), "SCHPANZERBRUCK", style);
    if (fpsMax > 0)
        app->SetFramerateLimit(fpsMax);
    app->UseVerticalSync(synchroVert);
}

MoteurJeu::~MoteurJeu()
{
    delete app;
}

MoteurCombat MoteurJeu::obtenirMoteurCombat(DonneesMap DM, Touches touches)
{
    MoteurCombat MC(app, DM, touches);
    return MC;
}
