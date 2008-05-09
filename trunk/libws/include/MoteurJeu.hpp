#ifndef MOTEUR_JEU_HEADER
#define MOTEUR_JEU_HEADER

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <MoteurCombat.hpp>
#include <MapClient.hpp>

namespace ws
{
struct Touches;
class MoteurCombat;

class MoteurJeu
{
private:
    sf::RenderWindow* app;
    MoteurCombat* MC;

public:
    MoteurJeu(bool pleinEcran=true, bool modeAuto=true, bool synchroVert=true, int appL=800, int appH=600, int bpp=32);

    ~MoteurJeu();

    void limiterFPS(int fpsMax);

    MoteurCombat* getMoteurCombat(const DonneesMap& DM, Touches* touches=NULL);
};
}

#endif
