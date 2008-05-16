#include <InterfaceCombat.hpp>


namespace ws
{

InterfaceCombat::InterfaceCombat(unsigned int _appL, unsigned int _appH)
{
    appL = _appL;
    appH = _appH;
    picked[0] = -1; picked[1] = -1;
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

void InterfaceCombat::GL_DessinPourSelection(int curseurX, int curseurY, bool clic)
{
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPickMatrix(curseurX, appH - curseurY, 1, 1, viewport);
    gluPerspective(70, static_cast<float>(appL)/appH, 1, 50);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glDisable(GL_ALPHA_TEST);
    
    if(menuEchapON)
        glPushName(SLC_MENU_ECHAP);
        GL_MenuEchapPourSelection();
        glPopName();
}

void InterfaceCombat::GL_Dessin()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, appL, 0, appH);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glDisable(GL_ALPHA_TEST);
    
    if(menuEchapON)
        GL_MenuEchap();
}

void InterfaceCombat::traiterSelection(int* selection)
{
    picked[0] = selection[0];
    picked[1] = selection[1];
}

void InterfaceCombat::pasDeSelection()
{
    picked[0] = -1; picked[1] = -1;
}

}
