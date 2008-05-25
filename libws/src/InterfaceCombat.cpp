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
    barreInfosON = false;
    clic = false;
    mtrChoisies[0] = -1; mtrChoisies[1] = -1; mtrChoisies[2] = -1;
    factAssomb = 1;
    setInfosDsBarre();
    txtChrono = "";
}


void InterfaceCombat::switchMenuEchap()
{
    menuEchapON = !menuEchapON;
    factAssomb = (factAssomb == 1 ? 5 : 1);
    barreInfosON = !menuEchapON;
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

void InterfaceCombat::GL_LigneTexteLargeurMax(string texte, float largeurTxtMax, float hauteurTxt, bool centrer, unsigned int numPoliceBmp)
{
    if(texte.length() == 0)
        return;
    
    float largeurTxt = (hauteurTxt/2)*texte.length();
    if(largeurTxt > largeurTxtMax)
        largeurTxt = largeurTxtMax;
    
    if(centrer)
    {   glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glTranslatef(largeurTxtMax/2 - largeurTxt/2, 0, 0);
    }
    
    GL_LigneTexte(texte, largeurTxt, hauteurTxt, numPoliceBmp);
    
    if(centrer)
        glPopMatrix();
}

void InterfaceCombat::GL_LigneTexteHauteurMax(string texte, float largeurTxt, float hauteurTxtMax, bool centrer, unsigned int numPoliceBmp)
{
    if(texte.length() == 0)
        return;
    
    float hauteurTxt = (largeurTxt/texte.length())*2;
    if(hauteurTxt > hauteurTxtMax)
        hauteurTxt = hauteurTxtMax;
    
    if(centrer)
    {   glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glTranslatef(0, hauteurTxtMax/2 - hauteurTxt/2, 0);
    }
    
    GL_LigneTexte(texte, largeurTxt, hauteurTxt, numPoliceBmp);
    
    if(centrer)
        glPopMatrix();
}

void InterfaceCombat::GL_Cadre(float L, float H, float rayon)
{
    float min = (L < H ? L : H)/2;
    if(rayon < 0)
        rayon = 0;
    else if(rayon > min)
        rayon = min;
    
    float conv = M_PI/180;
    
    glBegin(GL_POLYGON);
        for(unsigned int i=0; i<=90; i+=10)
            glVertex2f(rayon*(1-cos(i*conv)), rayon*(1-sin(i*conv)));
        for(unsigned int i=0; i<=90; i+=10)
            glVertex2f(L - rayon*(1-sin(i*conv)), rayon*(1-cos(i*conv)));
        for(unsigned int i=0; i<=90; i+=10)
            glVertex2f(L - rayon*(1-cos(i*conv)), H-rayon*(1-sin(i*conv)));
        for(unsigned int i=0; i<=90; i+=10)
            glVertex2f(rayon*(1-sin(i*conv)), H-rayon*(1-cos(i*conv)));
    glEnd();
}

void InterfaceCombat::GL_Bouton(string texte, int R_txt, int V_txt, int B_txt, float L, float H, float rayon)
{    
    float L_txt = (4.0/5)*L;
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
        GL_Cadre(L, H, rayon);
        glColor3ub(R_txt, V_txt, B_txt);
        glTranslatef((1.0/10)*L, 0, 0);
        GL_LigneTexteLargeurMax(texte, L_txt, H);
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
    float menuL = appL/3.0;
    float menuH = (3.0/4)*menuL;
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    
    glTranslatef(appL/2 - menuL/2, appH/2 - menuH/2, 0);    // Origine au point en bas à gauche du MenuEchap
    glColor4ub(20, 20, 20, 200);
    GL_Cadre(menuL, menuH, appH/16.0);
    
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


void InterfaceCombat::GL_BarreInfos()
{
    float barreL = appL/2.0;
    float barreH = barreL/9;
    unsigned int rayonBarre = 10;
    float zoneEcritureL = barreL - rayonBarre*2;
    float zoneEcritureH = barreH - rayonBarre*2;
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    
    glTranslatef(5, 5, 0);
    glColor4ub(225/factAssomb, 162/factAssomb, 0, 220);
    GL_Cadre(barreL, barreH, rayonBarre);
    
    glTranslatef(rayonBarre, rayonBarre, 0);
    glColor3ub(0, 0, 0);
    
    float hautTxt = zoneEcritureH*(2.0/5);
    float largTxtMax = zoneEcritureL*(4.0/10);
    
    glPushMatrix();
        // Infos TUILE
        GL_LigneTexteLargeurMax(infosActDsBarre[0], largTxtMax, hautTxt, false);
        
        glTranslatef(largTxtMax + zoneEcritureL/10, 0, 0);       // Infos ELEMENT
        GL_LigneTexteLargeurMax(infosActDsBarre[1], largTxtMax, hautTxt, false);
    glPopMatrix();
    
    glTranslatef(0, hautTxt + zoneEcritureH/5, 0);    // Infos PERSO
    GL_LigneTexteLargeurMax(infosActDsBarre[2], zoneEcritureL, hautTxt, false);
    
    glPopMatrix();
}


void InterfaceCombat::GL_Chrono()
{
    float tailleChrono = appL/12;
    float rayonChrono = 10;
    float tailleZoneEcriture = tailleChrono - rayonChrono*2;
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    
    glTranslatef(appL*(11.0/12) - 5, 5, 0);
    glColor4ub(0, 0, 0, 220);
    GL_Cadre(tailleChrono, tailleChrono, rayonChrono);
    
    glTranslatef(rayonChrono, rayonChrono, 0);
    glColor3ub(255, 255, 255);
    GL_LigneTexteLargeurMax(txtChrono, tailleZoneEcriture, tailleZoneEcriture);
    
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
    if(barreInfosON)
        GL_BarreInfos();
    
    GL_Chrono();
    
    if(menuEchapON)
        GL_MenuEchap();
}


void InterfaceCombat::passerSelection(int* selection)
{
    picked[0] = selection[0];
    picked[1] = selection[1];
    clic = false;
}


void InterfaceCombat::pasDeSelection()
{
    picked[0] = -1; picked[1] = -1;
    clic = false;
}

void InterfaceCombat::setInfosDsBarre(string tuile, string element, string perso)
{
    if(tuile == "" && element == "" && perso == "")
        barreInfosON = false;
    else
        barreInfosON = true;
    
    infosActDsBarre[0] = tuile;
    infosActDsBarre[1] = element;
    infosActDsBarre[2] = perso;
}

}
