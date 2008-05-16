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
    unsigned int menuL = appL/3;
    unsigned int menuH = static_cast<unsigned int>((3.0/4)*menuL);
    glPushMatrix();     // MODELVIEW à ce stade
    glTranslatef(appL/2 - menuL/2, appH/2 - menuH/2, 0);    // Origine au point en bas à gauche du MenuEchap
    
    unsigned int obActL = static_cast<unsigned int>((2.0/3)*menuL);
    unsigned int obActH = static_cast<unsigned int>((1.5/10)*menuH);
    
    glPushName(SLC_QUITTER);
    glPushMatrix();     // BOUTON "QUITTER"
    glTranslatef(menuL/2 - obActL/2, (1.5/10)*menuH, 0);
    glBegin(GL_QUADS);
        glVertex2i(0, obActH);
        glVertex2i(0, 0);
        glVertex2i(obActL, 0);
        glVertex2i(obActL, obActH);
    glEnd();
    glPopMatrix();
    glPopName();
    
    glPopMatrix();
}

void InterfaceCombat::GL_MenuEchap()
{
    glColor4ub(40, 40, 40, 200);
    unsigned int menuL = appL/3;
    unsigned int menuH = static_cast<unsigned int>((3.0/4)*menuL);
    glPushMatrix();     // MODELVIEW à ce stade
    glTranslatef(appL/2 - menuL/2, appH/2 - menuH/2, 0);    // Origine au point en bas à gauche du MenuEchap
    glBegin(GL_POLYGON);
        glVertex2i(0, menuH-15);
        glVertex2i(0, 15);
        glVertex2i(15, 0);
        glVertex2i(menuL-15, 0);
        glVertex2i(menuL, 15);
        glVertex2i(menuL, menuH-15);
        glVertex2i(menuL-15, menuH);
        glVertex2i(15, menuH);
    glEnd();
    
    unsigned int obActL = static_cast<unsigned int>((2.0/3)*menuL);
    unsigned int obActH = static_cast<unsigned int>((1.5/10)*menuH);
    glColor3ub(255, 25, 25);
    glPushMatrix();     // BOUTON "QUITTER"
    glTranslatef(menuL/2 - obActL/2, (1.5/10)*menuH, 0);
    glBegin(GL_QUADS);
        glVertex2i(0, obActH);
        glVertex2i(0, 0);
        glVertex2i(obActL, 0);
        glVertex2i(obActL, obActH);
    glEnd();
    glPopMatrix();
    
    glPopMatrix();
}

void InterfaceCombat::GL_DessinPourSelection(unsigned int curseurX, unsigned int curseurY)
{
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPickMatrix(curseurX, appH - curseurY, 1, 1, viewport);
    gluOrtho2D(0, appL, 0, appH);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glDisable(GL_ALPHA_TEST);
    
    if(menuEchapON)
    {
        glPushName(SLC_MENU_ECHAP);
        GL_MenuEchapPourSelection();
        glPopName();
    }
    else
    {
        // DESSIN POUR SELECTION DE TOUT LE RESTE DE L'INTERFACE
    }
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
    
    glEnable(GL_BLEND);
    glEnable(GL_ALPHA_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glAlphaFunc(GL_GREATER, 0);
    
    // On est ici en 2D, donc les menus doivent être affichés dans un ordre précis
    
    if(menuEchapON)
        GL_MenuEchap();
}

bool InterfaceCombat::traiterSelection(int* selection, bool clic)
{
    switch(picked[0])
    {
        case SLC_MENU_ECHAP:
            switch(picked[1])
            {
                case SLC_QUITTER:
                    if(clic)
                        return false;
                    break;
                default: break;
            }
            break;
        
        default: break;
    }
    
    picked[0] = selection[0];
    picked[1] = selection[1];
    
    return true;
}

void InterfaceCombat::pasDeSelection()
{
    picked[0] = -1; picked[1] = -1;
}

}
