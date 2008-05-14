#ifndef INTERFACE_COMBAT_HEADER
#define INTERFACE_COMBAT_HEADER

#include <GL/gl.h>


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
    
    void GL_DessinPourSelection();
    void GL_Dessin();
};

}

#endif
