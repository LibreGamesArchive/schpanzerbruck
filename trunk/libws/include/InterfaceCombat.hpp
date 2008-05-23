#ifndef INTERFACE_COMBAT_HEADER
#define INTERFACE_COMBAT_HEADER

#include "GestionnaireImages.hpp"

#include <SFML/Graphics.hpp>
#include <GL/gl.h>
#include <GL/glu.h>

#include <string>

using namespace std;

enum{   SLC_CONTINUER, SLC_QUITTER, SLC_MENU_ECHAP,
        SLC_MENU_TRIANGLE, SLC_DEPLACEMENT, SLC_ACTION, SLC_FIN_DU_TOUR
    };


namespace ws
{

class InterfaceCombat
{
private:
    bool menuEchapON;
    
    void GL_MenuEchapPourSelection();
    void GL_MenuEchap();
    
    unsigned int appL, appH;
    GestionnaireImages* gestImages;
    
    int picked[2];
    
    void GL_LigneTexte(string texte, float largeurTxt, float hauteurTxt, unsigned int numPoliceBmp=1);
    
public:
    InterfaceCombat(GestionnaireImages* _gestImages, unsigned int _appL, unsigned int _appH);
    
    void switchMenuEchap();
    
    void GL_DessinPourSelection(unsigned int curseurX, unsigned int curseurY);
    void GL_Dessin();
    
    bool traiterSelection(int* selection, bool clic=false);
    void pasDeSelection();
};

}

#endif
