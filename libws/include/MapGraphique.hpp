#ifndef MAP_GRAPHIQUE_HEADER
#define MAP_GRAPHIQUE_HEADER

#include "GestionnaireImages.hpp"
#include "MapFX.hpp"
#include "Utils.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <GL/gl.h>
#include <GL/glu.h>
#include <list>
#include <string>
#include <iostream>

#define VITESSE_DEP     4
//Distance parcourue par les persos chaque seconde


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
    
    unsigned int factAssomb;
    
    sf::Image* texBordure;
    
    list<FX*> FXActives;   // Liste des effets spéciaux actuellement utilisés sur la Map
    
    float persoDep_offsetX, persoDep_offsetY;
    list<int> cheminDeplacement;
    
    MenuTriangle* menuTriangle;
    
    void GL_DessinTuile(sf::Image* texture=NULL);
    void GL_DessinElement(sf::Image* texture=NULL);
    void GL_DessinPersoPourSelection();
    void GL_DessinPerso(sf::Image* texFantome, sf::Image* texHalo, sf::Image* texArmeFantome, sf::Image* texArmeHalo, int R, int V, int B, bool select);

public:
    unsigned int statut;
    bool noircir;
    int picked[2];
    float inclinaisonElements;
    vector<PersoGraphique> listePersos;
    int numPersoCourant;
    bool imagesPersosChargees;      // Le MoteurCombat met ce booléen à TRUE si les images des persos (fantôme, halo, armes, etc.) ont bien été chargées dans le GestionnaireImages
    bool* masqueCasesPossibles;
    
    MapGraphique(GestionnaireImages* _gestImages, const DonneesMap& _DM, unsigned int _appL, unsigned int _appH, MenuTriangle* _menuTriangle);
    ~MapGraphique();
    
    int getHauteur();
    int getLargeur();
    
    void lancerFX(FX* nouvFX);
    
    void GL_DessinPourSelection(float frameTime, const Camera& camera, unsigned int curseurX, unsigned int curseurY, bool elemsON, bool _clic=false);
    void GL_Dessin(float frameTime, const Camera& camera, bool elemsON=true);
    
    void passerSelection(int* selection);
    void pasDeSelection();
    
    void deplacerPersoCourant(list<int> _chemin);
    
    void initMasqueCasesPossibles();
    
    bool deplacementEnCours();
    bool actionEnCours();
};

}

#endif
