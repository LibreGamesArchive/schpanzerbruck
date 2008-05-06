#ifndef MOTEUR_JEU_HEADER
#define MOTEUR_JEU_HEADER

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <MoteurCombat.hpp>

namespace ws
{
struct DonneesMap;
struct Touches;
class MoteurCombat;

class MoteurJeu
{
private:
    sf::RenderWindow* app;

public:
    MoteurJeu(bool pleinEcran=true, bool modeAuto=true, bool synchroVert=true, int appL=800, int appH=600, int bpp=32);

    ~MoteurJeu();

    void limiterFPS(int fpsMax);

    MoteurCombat getMoteurCombat(DonneesMap DM, Touches touches);
};
}

#endif
