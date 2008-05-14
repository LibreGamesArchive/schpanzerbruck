#ifndef INTERFACE_COMBAT_HEADER
#define INTERFACE_COMBAT_HEADER

#include <GL/gl.h>
#include <GL/glu.h>


enum{   SLC_MENU_ECHAP, SLC_CONTINUER, SLC_QUITTER,
        SLC_MENU_TRIANGLE, SLC_DEPLACEMENT, SLC_ACTION, SLC_PASSER
    };


namespace ws
{

class InterfaceCombat
{
private:
    bool menuEchapON;
    
    void GL_MenuEchapPourSelection();
    void GL_MenuEchap();
    
public:
    InterfaceCombat();
    
    void switchMenuEchap();
    
    void GL_DessinPourSelection(int appL, int appH, int curseurX, int curseurY);
    void GL_Dessin(int appL, int appH);
    
    void traiterSelection(int* selection);
    void pasDeSelection();
};

}

#endif
