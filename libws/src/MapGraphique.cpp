#include <MapGraphique.hpp>


namespace ws
{
MapGraphique::MapGraphique(GestionnaireImages* _gestImages, const DonneesMap& _DM)
{
    gestImages = _gestImages;
    largeur = _DM.largeur;
    hauteur = _DM.hauteur;
    numsTuiles = _DM.numsTuiles;
    numsElements = _DM.numsElements;
    numTexBordure = _DM.numTexBordure;
    hauteurBordure = _DM.hauteurBordure;
    
    gestImages->chargerImagesMap(largeur*hauteur, numsTuiles, numsElements, _DM.cheminsTuiles, _DM.cheminsElements);
    gestImages->chargerImage("tuiles", numTexBordure, _DM.fichierTexBordure);
    
    coordsCases = new int*[hauteur*largeur]; // Coordonnées du point en haut à gauche de chaque case dans le plan (0xy)
    for(unsigned int i=0; i<hauteur*largeur; i++)
        coordsCases[i] = new int[3];
    
    int i=0;
    for(unsigned int x=0; x<hauteur; x++)
        for(unsigned int y=0; y<largeur; y++) {
            coordsCases[i][0] = x;
            coordsCases[i][1] = y;
            coordsCases[i][2] = 0;
            i++;
        }
    
    statut = INFOS_SEULEMENT;
    
    inclinaisonElements = 1;
    
    DeploiementElements* monFX = new DeploiementElements();
    lancerFX(monFX);
    
    picked[0] = -1; picked[1] = -1;     // ObjetMap sélectionné par le picking.
    // picked == [-1, -1] : Pas d'objet sélectionné
    // picked = [numCase, typeObjet] : Objet sélectionné :
    //       typeObjet: ==0 : tuile; ==1 : élément; ==2 : perso
}

MapGraphique::~MapGraphique()
{
    for(unsigned int i=0; i<hauteur*largeur; i++)
        delete coordsCases[i];
    delete coordsCases;
}

void MapGraphique::GL_DessinTuile(sf::Image* texture)
{
    if (texture != NULL)
        texture->Bind();
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
    glTexCoord2f(0, 1); glVertex3f(1, 0, 0);
    glTexCoord2f(1, 1); glVertex3f(1, 1, 0);
    glTexCoord2f(1, 0); glVertex3f(0, 1, 0);
    glEnd();
}

void MapGraphique::GL_DessinElement(sf::Image* texture)
{
    glTranslatef(0.5, 0, 0);
    glRotated(inclinaisonElements-90, 0, 1, 0);
    if (texture != NULL)
        texture->Bind();
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(0, 0, 1.15);
    glTexCoord2f(0, 1); glVertex3f(0, 0, 0);
    glTexCoord2f(1, 1); glVertex3f(0, 1, 0);
    glTexCoord2f(1, 0); glVertex3f(0, 1, 1.15);
    glEnd();
}

int MapGraphique::getHauteur()
{
    return hauteur;
}

int MapGraphique::getLargeur()
{
    return largeur;
}

void MapGraphique::bloquer(bool autoriserInfos)
{
    if (autoriserInfos)
        statut = INFOS_SEULEMENT;
    else
        statut = PAS_DE_SELECTION;
}

void MapGraphique::noircir()
{
    statut = NOIRCIR;
}

void MapGraphique::phaseDeplacement()
{
    statut = DEPLACEMENT;
}

void MapGraphique::phaseCiblage()
{
    statut = CIBLAGE;
}

void MapGraphique::lancerFX(FX* nouvFX)
{
    FXActives.push_back(nouvFX);
}

void MapGraphique::GL_DessinPourPicking(float frameTime, int appL, int appH, const Camera& camera, int curseurX, int curseurY, bool elemsON)
{
    GLuint buffer[128];
    glSelectBuffer(128, buffer);

    glRenderMode(GL_SELECT);
    glInitNames();

    GLint viewport[4];
    glGetIntegerv (GL_VIEWPORT, viewport);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPickMatrix(curseurX, appH - curseurY, 1, 1, viewport);
    gluPerspective(70, static_cast<float>(appL)/appH, 1, 50);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(camera.pos[0], camera.pos[1], camera.pos[2], camera.cible[0], camera.cible[1], camera.cible[2], 0, 0, 1);

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glDisable(GL_ALPHA_TEST);

    if (statut != CIBLAGE)
    {
        // DESSIN DES TUILES
        for(unsigned int numCase=0; numCase<hauteur*largeur; numCase++)
        {
            int numTuileAct = numsTuiles[numCase];
            if (numTuileAct > 0)
            {
                glPushMatrix();
                glTranslated(coordsCases[numCase][0], coordsCases[numCase][1], coordsCases[numCase][2]);
                
                glPushName(numCase); glPushName(0);
                GL_DessinTuile(gestImages->obtenirImage("tuiles", numTuileAct));
                glPopName(); glPopName();
                
                glPopMatrix();
            }
        }
    }

    if (elemsON && statut == CIBLAGE)
    {
        // DESSIN DES ELEMENTS
        for(unsigned int numCase=0; numCase<hauteur*largeur; numCase++)
        {
            int numElemAct = numsElements[numCase];
            if (numElemAct > 0)
            {
                glPushMatrix();
                glTranslated(coordsCases[numCase][0], coordsCases[numCase][1], coordsCases[numCase][2]);
                
                glPushName(numCase); glPushName(1);
                GL_DessinElement(gestImages->obtenirImage("elements", numElemAct));
                glPopName(); glPopName();
                
                glPopMatrix();
            }
        }
    }

    GLuint hits = glRenderMode(GL_RENDER);
    GLfloat plusPetitZ_min = 1.0;
    if (hits > 0)
    {
        GLfloat z_min = 1.0;
        //GLfloat z_max = 0.0;
        GLuint* ptr = buffer;
        for(GLuint i=0; i < hits; i++)
        {
            GLuint nbrNames = *ptr; ptr++;
            z_min = (GLfloat)(*ptr)/0xffffffff; ptr++;
            /*z_max=(GLfloat)(*ptr)/0xffffffff;*/ ptr++;
            if (z_min < plusPetitZ_min)
            {
                for(GLuint j=0; j<nbrNames; j++)
                {
                    picked[j] = *ptr; ptr++;
                }
                plusPetitZ_min = z_min;
            }
            else
                for(GLuint j=0; j<nbrNames; j++)
                    ptr++;
        }
    }
    else
    {    picked[0] = -1; picked[1] = -1;    }
}

void MapGraphique::GL_Dessin(float frameTime, int appL, int appH, const Camera& camera, int curseurX, int curseurY, bool elemsON)
{
    // Dessine la Map dans le plan (0xy)

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70, static_cast<float>(appL)/appH, 1, 50);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(camera.pos[0], camera.pos[1], camera.pos[2], camera.cible[0], camera.cible[1], camera.cible[2], 0, 0, 1);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glEnable(GL_ALPHA_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glAlphaFunc(GL_GREATER, 0);
    
    int factAssomb = 1;
    if (statut == NOIRCIR)
        factAssomb = 5;
    else
    {   list<FX*>::iterator it = FXActives.begin();
        while(it != FXActives.end())
        {
            FX* FXActuel = *it;
            if (FXActuel->effet(this, frameTime))
            {
                delete FXActuel;
                it = FXActives.erase(it);
            }
            else
            {   it++;   }
        }
    }
    
    int nvGris = 255/factAssomb;
    glColor3ub(nvGris, nvGris, nvGris);

    bool selec = false;
    
    // DESSIN DES TUILES
    for(unsigned int numCase=0; numCase<hauteur*largeur; numCase++)
    {
        int numTuileAct = numsTuiles[numCase];
        if (numTuileAct > 0)
        {
            glPushMatrix();
            glTranslated(coordsCases[numCase][0], coordsCases[numCase][1], coordsCases[numCase][2]);
            
            selec = false;
            if (picked[0] == static_cast<int>(numCase))
                if (picked[1] == 0)
                    selec = true;
            
            if (selec)
                glColor3ub(0, nvGris/2, nvGris);
            GL_DessinTuile(gestImages->obtenirImage("tuiles", numTuileAct));
            if (selec)
                glColor3ub(nvGris, nvGris, nvGris);
            
            glPopMatrix();
        }
    }
    
    // DESSIN DES ELEMENTS
    for(unsigned int numCase=0; numCase<hauteur*largeur; numCase++)
    {
        int numElemAct = numsElements[numCase];
        if (numElemAct > 0)
        {
            glPushMatrix();
            glTranslated(coordsCases[numCase][0], coordsCases[numCase][1], coordsCases[numCase][2]);
            
            selec = false;
            if (picked[0] == static_cast<int>(numCase))
                if (picked[1] == 1)
                    selec = true;
            
            if (selec)
                glColor3ub(0, nvGris/2, nvGris);
            else
                if (!elemsON)
                    glColor4ub(nvGris, nvGris, nvGris, 80);
            GL_DessinElement(gestImages->obtenirImage("elements", numElemAct));
            if (selec || !elemsON)
                glColor3ub(nvGris, nvGris, nvGris);
            
            glPopMatrix();
        }
    }
    
    // Dessin du plateau:
    gestImages->obtenirImage("tuiles", numTexBordure)->Bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBegin(GL_QUADS);

    nvGris = 195/factAssomb;
    glColor3ub(nvGris, nvGris, nvGris);
    glTexCoord2d(0, 0); glVertex3f(0, 0, 0);
    glTexCoord2d(0, hauteurBordure); glVertex3f(0, 0, -hauteurBordure);
    glTexCoord2d(hauteur, hauteurBordure); glVertex3f(hauteur, 0, -hauteurBordure);
    glTexCoord2d(hauteur, 0); glVertex3f(hauteur, 0, 0);

    nvGris = 85/factAssomb;
    glColor3ub(nvGris, nvGris, nvGris);
    glTexCoord2d(0, 0); glVertex3f(0, 0, 0);
    glTexCoord2d(0, hauteurBordure); glVertex3f(0, 0, -hauteurBordure);
    glTexCoord2d(largeur, hauteurBordure); glVertex3f(0, largeur, -hauteurBordure);
    glTexCoord2d(largeur, 0); glVertex3f(0, largeur, 0);

    nvGris = 135/factAssomb;
    glColor3ub(nvGris, nvGris, nvGris);
    glTexCoord2d(0, 0); glVertex3f(hauteur, largeur, 0);
    glTexCoord2d(0, hauteurBordure); glVertex3f(hauteur, largeur, -hauteurBordure);
    glTexCoord2d(hauteur, hauteurBordure); glVertex3f(0, largeur, -hauteurBordure);
    glTexCoord2d(hauteur, 0); glVertex3f(0, largeur, 0);

    nvGris = 255/factAssomb;
    glColor3ub(nvGris, nvGris, nvGris);
    glTexCoord2d(0, 0); glVertex3f(hauteur, 0, 0);
    glTexCoord2d(0, hauteurBordure); glVertex3f(hauteur, 0, -hauteurBordure);
    glTexCoord2d(largeur, hauteurBordure); glVertex3f(hauteur, largeur, -hauteurBordure);
    glTexCoord2d(largeur, 0); glVertex3f(hauteur, largeur, 0);

    glEnd();

    if (statut != NOIRCIR && statut != PAS_DE_SELECTION)
        GL_DessinPourPicking(frameTime, appL, appH, camera, curseurX, curseurY, elemsON);
}
}

