#ifndef MAP_CLIENT_HEADER
#define MAP_CLIENT_HEADER

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>

#include "GestionnaireImages.hpp"
#include "MapFX.hpp"

using namespace std;

#define PAS_DE_SELECTION    0
#define NOIRCIR             1
#define INFOS_SEULEMENT     2
#define DEPLACEMENT         3
#define CIBLAGE             4


struct Camera
{
    float pos[3], cible[3];
};


class MapClient
{
private:
    int *numsTuiles, *numsElements;
    GestionnaireImages *gestImages;

    int **coordsCases;

    int hauteur, largeur, statut, inclinaisonElements, hauteurBordure, numTexBordure;

    vector<FX> FXActives;   // Liste des effets spéciaux actuellement utilisés sur la Map

    int picked[2];

    friend class FX;


    void GL_DessinTuile(sf::Image* texture=NULL);

    void GL_DessinElement(sf::Image* texture=NULL, int inclinaisonElements=0);


    void GL_DessinPourPicking(float frameTime, int appL, int appH, Camera camera, int curseurX, int curseurY, bool elemsON);


public:
    MapClient(GestionnaireImages* _gestImages, int _largeur, int _hauteur, int* _numsTuiles, int* _numsElements, char** _cheminsTuiles, char** _cheminsElements, int _numTexBordure, float _hauteurBordure);

    ~MapClient();


    void bloquer(bool autoriserInfos=true);

    void noircir();

    void phaseDeplacement();

    void phaseCiblage();


    void lancerFX(FX &nouvFX);


    void GL_Dessin(float frameTime, int appL, int appH, Camera camera, int curseurX, int curseurY, bool elemsON=true);
};

#endif
