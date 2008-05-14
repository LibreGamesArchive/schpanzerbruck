#include <InterfaceCombat.hpp>


namespace ws
{

InterfaceCombat::InterfaceCombat()
{
    menuEchapON = false;
}

void InterfaceCombat::switchMenuEchap()
{
    menuEchapON = !menuEchapON;
}

void InterfaceCombat::GL_MenuEchapPourSelection()
{
    
}

void InterfaceCombat::GL_MenuEchap()
{
    
}

void InterfaceCombat::GL_DessinPourSelection(int appL, int appH, int curseurX, int curseurY)
{
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPickMatrix(curseurX, appH - curseurY, 1, 1, viewport);
    gluPerspective(70, static_cast<float>(appL)/appH, 1, 50);
    
    if(menuEchapON)
        GL_MenuEchapPourSelection();
}

void InterfaceCombat::GL_Dessin(int appL, int appH)
{
    if(menuEchapON)
        GL_MenuEchap();
}

void InterfaceCombat::traiterSelection(int* selection)
{
    
}

void InterfaceCombat::pasDeSelection()
{
    
}

}
