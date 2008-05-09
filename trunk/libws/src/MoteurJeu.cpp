#include <MoteurJeu.hpp>

namespace ws
{
MoteurJeu::MoteurJeu(bool pleinEcran, bool modeAuto, bool synchroVert, int appL, int appH, int bpp)
{
    int style = sf::Style::Close;
    if(pleinEcran)
        style |= sf::Style::Fullscreen;
    app = new sf::RenderWindow(sf::VideoMode(appL, appH, bpp), "SCHPANZERBRUCK", style);
    app->UseVerticalSync(synchroVert);
    // app->PreserveOpenGLStates(true)
}

MoteurJeu::~MoteurJeu()
{
    delete app;
    delete MC;
}

void MoteurJeu::limiterFPS(int fpsMax)
{
    app->SetFramerateLimit(fpsMax);
}

MoteurCombat* MoteurJeu::getMoteurCombat(const DonneesMap& DM, Touches* touches)
{
    if (touches != NULL)
    {
        MC = new MoteurCombat(app, DM, *touches);
        return MC;
    }
    Touches defTouches;
    defTouches.zoomAvant = sf::Key::PageUp;
    defTouches.zoomArriere = sf::Key::PageDown;
    MC = new MoteurCombat(app, DM, defTouches);
    return MC;
}
}

