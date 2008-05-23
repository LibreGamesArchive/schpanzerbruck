#include <InterfaceCombat.hpp>


namespace ws
{

InterfaceCombat::InterfaceCombat(GestionnaireImages* _gestImages, unsigned int _appL, unsigned int _appH)
{
    appL = _appL;
    appH = _appH;
    gestImages = _gestImages;
    picked[0] = -1; picked[1] = -1;
    menuEchapON = false;
    clic = false;
}


void InterfaceCombat::switchMenuEchap()
{
    menuEchapON = !menuEchapON;
}


void InterfaceCombat::GL_LigneTexte(string texte, float largeurTxt, float hauteurTxt, unsigned int numPoliceBmp)
{
    // Le texte sera affiché dans le plan (0xy), en (0,0,0)
    
    glEnable(GL_TEXTURE_2D);
    gestImages->obtenirImage("polices_bmp", numPoliceBmp)->Bind();
    
    float unit = 1.0/16;
    float largeurLettres = largeurTxt / texte.length();
    float hauteurLettres = hauteurTxt;
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    for(unsigned int i=0; i<texte.length(); i++)
    {
        int numAscii = texte[i];
        float coordTexX, coordTexY;
        coordTexX = (numAscii % 16) * unit;
        coordTexY = (numAscii / 16) * unit;
        
        glBegin(GL_QUADS);
            glTexCoord2f(coordTexX, coordTexY);             glVertex3f(0, hauteurLettres, 0);
            glTexCoord2f(coordTexX, coordTexY+unit);        glVertex3f(0, 0, 0);
            glTexCoord2f(coordTexX+unit, coordTexY+unit);   glVertex3f(largeurLettres, 0, 0);
            glTexCoord2f(coordTexX+unit, coordTexY);        glVertex3f(largeurLettres, hauteurLettres, 0);
        glEnd();
        
        glTranslatef(largeurLettres, 0, 0);
    }
    glPopMatrix();
    
    glDisable(GL_TEXTURE_2D);
}

void InterfaceCombat::GL_Cadre(float L, float H, float offset)
{
    glBegin(GL_POLYGON);
        glVertex2f(0, H-offset);
        glVertex2f(0, offset);
        glVertex2f(offset, 0);
        glVertex2f(L-offset, 0);
        glVertex2f(L, offset);
        glVertex2f(L, H-offset);
        glVertex2f(L-offset, H);
        glVertex2f(offset, H);
    glEnd();
}

void InterfaceCombat::GL_Bouton(string texte, int R_txt, int V_txt, int B_txt, float L, float H, float offset)
{    
    float L_txt = (4.0/5)*L;
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
        GL_Cadre(L, H, offset);
        glColor3ub(R_txt, V_txt, B_txt);
        glTranslatef((1.0/10)*L, 0, 0);
        GL_LigneTexte(texte, L_txt, H);
    glPopMatrix();
}


void InterfaceCombat::GL_MenuEchapPourSelection()
{
    float menuL = appL/3;
    float menuH = (3.0/4)*menuL;
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslatef(appL/2 - menuL/2, appH/2 - menuH/2, 0);    // Origine au point en bas à gauche du MenuEchap
    
    float obActL = (2.0/3)*menuL;
    float obActH = (1.5/10)*menuH;
    
    glPushMatrix();
        glPushName(SLC_QUITTER);      // BOUTON "QUITTER"
            glTranslatef(menuL/2 - obActL/2, (1.5/10)*menuH, 0);
            GL_Cadre(obActL, obActH, 10);
        glPopName();
        
        glPushName(SLC_CONTINUER);     // BOUTON "CONTINUER"
            glTranslatef(0, (3.0/10)*menuH, 0);
            GL_Cadre(obActL, obActH, 10);
        glPopName();
    glPopMatrix(); 
    
    glPopMatrix();
}


void InterfaceCombat::GL_MenuEchap()
{
    float menuL = appL/3;
    float menuH = (3.0/4)*menuL;
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    
    glColor4ub(40, 40, 40, 200);
    glTranslatef(appL/2 - menuL/2, appH/2 - menuH/2, 0);    // Origine au point en bas à gauche du MenuEchap
    GL_Cadre(menuL, menuH, 15);
    
    float obActL = (2.0/3)*menuL;
    float obActH = (1.5/10)*menuH;
    
    glPushMatrix();
        // BOUTON "QUITTER"
        glTranslatef(menuL/2 - obActL/2, (1.5/10)*menuH, 0);
        glColor3ub(255, 25, 25);
        if (picked[1] == SLC_QUITTER)
        {   if (clic)
                glColor3ub(255, 204, 0);
            GL_Bouton("QUITTER", 255, 150, 0, obActL, obActH, 10);
        }
        else
            GL_Bouton("QUITTER", 255, 255, 255, obActL, obActH, 10);
        
        // BOUTON "CONTINUER"
        glTranslatef(0, (3.0/10)*menuH, 0);
        glColor3ub(255, 25, 25);
        if (picked[1] == SLC_CONTINUER)
        {   if (clic)
                glColor3ub(255, 204, 0);
            GL_Bouton("CONTINUER", 255, 150, 0, obActL, obActH, 10);
        }
        else
            GL_Bouton("CONTINUER", 255, 255, 255, obActL, obActH, 10);
        
        // TITRE
        glTranslatef(0, (3.0/10)*menuH, 0);
        glColor3ub(255, 255, 255);
        GL_LigneTexte("MENU ECHAP", obActL, obActH);
    glPopMatrix();
    
    glPopMatrix();
}


void InterfaceCombat::GL_DessinPourSelection(unsigned int curseurX, unsigned int curseurY, bool _clic)
{
    clic = _clic;
    
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


bool InterfaceCombat::traiterSelection(int* selection)
{
    picked[0] = selection[0];
    picked[1] = selection[1];
    
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
    
    return true;
}


void InterfaceCombat::pasDeSelection()
{
    picked[0] = -1; picked[1] = -1;
    clic = false;
}

}
