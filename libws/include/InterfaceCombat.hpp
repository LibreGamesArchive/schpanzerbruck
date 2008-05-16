#ifndef INTERFACE_COMBAT_HEADER
#define INTERFACE_COMBAT_HEADER

#include <GL/gl.h>
#include <GL/glu.h>


enum{   SLC_MENU_ECHAP, SLC_CONTINUER, SLC_QUITTER,
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
    
    int picked[2];
    
public:
    InterfaceCombat(unsigned int _appL, unsigned int _appH);
    
    void switchMenuEchap();
    
    void GL_DessinPourSelection(int curseurX, int curseurY, bool clic=false);
    void GL_Dessin();
    
    void traiterSelection(int* selection);
    void pasDeSelection();
};

}

#endif
