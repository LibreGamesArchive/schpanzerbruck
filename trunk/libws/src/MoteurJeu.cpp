#include <MoteurJeu.hpp>

namespace ws
{
MoteurJeu::MoteurJeu(bool pleinEcran, bool modeAuto, bool synchroVert, int appL, int appH, int bpp)
{
    MC = NULL;
    gestImages = new GestionnaireImages();
    int style = sf::Style::Close;
    if(pleinEcran)
        style |= sf::Style::Fullscreen;
    if(modeAuto)
        app = new sf::Window(sf::VideoMode::GetDesktopMode(), "SCHPANZERBRUCK", style);
    else
        app = new sf::Window(sf::VideoMode(appL, appH, bpp), "SCHPANZERBRUCK", style);
    app->UseVerticalSync(synchroVert);
    // app->PreserveOpenGLStates(true)
}

MoteurJeu::~MoteurJeu()
{
    arreterMoteurCombat();
    delete app;
    delete gestImages;
}

void MoteurJeu::limiterFPS(int fpsMax)
{
    app->SetFramerateLimit(fpsMax);
}

void MoteurJeu::demarrerMoteurCombat(const DonneesMap& DM, string policeBmp, Touches* touches)
{
    // CHARGEMENT DES POLICES:
    gestImages->chargerImage("polices_bmp", 1, policeBmp);
    
    if(MC == NULL)
    {
        if (touches != NULL)
        {
            MC = new MoteurCombat(app, gestImages, DM, *touches);
        }
        Touches defTouches;
        defTouches.zoomAvant = sf::Key::PageUp;
        defTouches.zoomArriere = sf::Key::PageDown;
        MC = new MoteurCombat(app, gestImages, DM, defTouches);
    }
}

MoteurCombat* MoteurJeu::getMoteurCombat()
{
    return MC;
}

void MoteurJeu::arreterMoteurCombat()
{
    if(MC != NULL)
    {
        delete MC;
        MC = NULL;
    }
}
}
