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
    
    bool clic;
    unsigned int appL, appH;
    int **coordsCases;

    unsigned int hauteur, largeur, numTexBordure;
    float hauteurBordure;
    
    sf::Image* texBordure;

    list<FX*> FXActives;   // Liste des effets spéciaux actuellement utilisés sur la Map

    void GL_DessinTuile(sf::Image* texture=NULL);
    void GL_DessinElement(sf::Image* texture=NULL);


public:
    enum {
        PAS_DE_SELECTION, INFOS_SEULEMENT, DEPLACEMENT, CIBLAGE
    };
    
    unsigned int statut;
    bool noircir;
    int picked[2];
    float inclinaisonElements;
    
    MapGraphique(GestionnaireImages* _gestImages, const DonneesMap& _DM, unsigned int _appL, unsigned int _appH);
    ~MapGraphique();
    
    int getHauteur();
    int getLargeur();
    
    void lancerFX(FX* nouvFX);
    
    void GL_DessinPourSelection(float frameTime, const Camera& camera, unsigned int curseurX, unsigned int curseurY, bool elemsON, bool _clic=false);
    void GL_Dessin(float frameTime, const Camera& camera, bool elemsON=true);
    
    void passerSelection(int* selection);
    void pasDeSelection();
};

}

#endif
