#ifndef MAP_GRAPHIQUE_HEADER
#define MAP_GRAPHIQUE_HEADER

#include "GestionnaireImages.hpp"
#include "MapFX.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <GL/gl.h>
#include <GL/glu.h>
#include <list>
#include <string>
#include <iostream>

#define PAS_DE_SELECTION    0
#define NOIRCIR             1
#define INFOS_SEULEMENT     2
#define DEPLACEMENT         3
#define CIBLAGE             4

using namespace std;

namespace ws
{
struct DonneesMap
{
    unsigned int largeur;
    unsigned int hauteur;
    vector<unsigned int> numsTuiles;
    vector<unsigned int> numsElements;
    vector<string> cheminsTuiles;
    vector<string> cheminsElements;
    unsigned int numTexBordure;
    string fichierTexBordure;
    float hauteurBordure;
};

struct Camera
{
    float pos[3], cible[3];
};

class FX;

class MapGraphique
{
private:

    vector<unsigned int> numsTuiles, numsElements;
    GestionnaireImages* gestImages;

    unsigned int appL, appH;
    int **coordsCases;

    unsigned int hauteur, largeur, statut, numTexBordure;
    float hauteurBordure;
    
    sf::Image* texBordure;

    list<FX*> FXActives;   // Liste des effets spéciaux actuellement utilisés sur la Map

    void GL_DessinTuile(sf::Image* texture=NULL);
    void GL_DessinElement(sf::Image* texture=NULL);


public:
    int picked[2];
    float inclinaisonElements;
    
    MapGraphique(GestionnaireImages* _gestImages, const DonneesMap& _DM, unsigned int _appL, unsigned int _appH);
    ~MapGraphique();
    
    int getHauteur();
    int getLargeur();
    
    void bloquer(bool autoriserInfos=true);
    void noircir();
    void phaseDeplacement();
    void phaseCiblage();
    
    int getStatut();
    
    void lancerFX(FX* nouvFX);
    
    void GL_DessinPourSelection(float frameTime, const Camera& camera, unsigned int curseurX, unsigned int curseurY, bool elemsON);
    void GL_Dessin(float frameTime, const Camera& camera, bool elemsON=true);
    
    void traiterSelection(int* selection, bool clic=false);
    void pasDeSelection();
};
}

#endif
