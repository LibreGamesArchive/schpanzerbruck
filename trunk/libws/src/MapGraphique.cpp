#include <MapGraphique.hpp>


namespace ws
{

MapGraphique::MapGraphique(GestionnaireImages* _gestImages, const DonneesMap& _DM, unsigned int _appL, unsigned int _appH, MenuTriangle* _menuTriangle)
{
    appL = _appL;
    appH = _appH;
    gestImages = _gestImages;
    largeur = _DM.largeur;
    hauteur = _DM.hauteur;
    numsTuiles = _DM.numsTuiles;
    numsElements = _DM.numsElements;
    numTexBordure = _DM.numTexBordure;
    hauteurBordure = _DM.hauteurBordure;
    texBordure = NULL;
    menuTriangle = _menuTriangle;
    
    clic = false;
    imagesPersosChargees = false;
    
    gestImages->chargerImagesMap(largeur*hauteur, numsTuiles, numsElements, _DM.cheminsTuiles, _DM.cheminsElements);
    if (numTexBordure!=0)
    {
        gestImages->chargerImage("tuiles", numTexBordure, _DM.fichierTexBordure);
        texBordure = gestImages->obtenirImage("tuiles", numTexBordure);
    }
    else
        texBordure = new sf::Image(1, 1, sf::Color(128, 128, 128));
    
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
    noircir = false;
    
    inclinaisonElements = 1;
    factAssomb = 1;
    
    DeploiementElements* monFX = new DeploiementElements();
    lancerFX(monFX);
    
    numPersoCourant = -1;
    
    persoDep_offsetX=0;
    persoDep_offsetY=0;
    
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
    
    for(list<FX*>::iterator it = FXActives.begin(); it != FXActives.end(); it++)
        delete (*it);
    
    if (numTexBordure == 0)
        delete texBordure;
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
    glRotatef(inclinaisonElements-90, 0, 1, 0);
    if (texture != NULL)
        texture->Bind();
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(0, 0, 1.15);
        glTexCoord2f(0, 1); glVertex3f(0, 0, 0);
        glTexCoord2f(1, 1); glVertex3f(0, 1, 0);
        glTexCoord2f(1, 0); glVertex3f(0, 1, 1.15);
    glEnd();
}

void MapGraphique::GL_DessinPersoPourSelection()
{
    float haut=1.8;
    float larg=haut/2;
    glTranslatef(0.7, (1-larg)/2, 0);
    glRotatef(inclinaisonElements-90, 0, 1, 0);
    glBegin(GL_QUADS);
        glVertex3f(0, 0, haut);
        glVertex3f(0, 0, 0);
        glVertex3f(0, larg, 0);
        glVertex3f(0, larg, haut);
    glEnd();
}

void MapGraphique::GL_DessinPerso(sf::Image* texFantome, sf::Image* texHalo, sf::Image* texArmeFantome, sf::Image* texArmeHalo, int R, int V, int B, bool select)
{
    float haut=1.8;
    float larg=haut/2;
    glTranslatef(0.7, (1-larg)/2, 0);
    glRotated(inclinaisonElements-90, 0, 1, 0);
    int nvGris = 255/factAssomb;
    
    glColor4ub(R/factAssomb, V/factAssomb, B/factAssomb, 210);
    texHalo->Bind();
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(0, 0, haut);
        glTexCoord2f(0, 1); glVertex3f(0, 0, 0);
        glTexCoord2f(1, 1); glVertex3f(0, larg, 0);
        glTexCoord2f(1, 0); glVertex3f(0, larg, haut);
    glEnd();
    
    if(select)
        glColor4ub(0, nvGris/2, nvGris, 230);
    else
        glColor4ub(nvGris, nvGris, nvGris, 230);
    texFantome->Bind();
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(0.01, 0, haut);
        glTexCoord2f(0, 1); glVertex3f(0.01, 0, 0);
        glTexCoord2f(1, 1); glVertex3f(0.01, larg, 0);
        glTexCoord2f(1, 0); glVertex3f(0.01, larg, haut);
    glEnd();
    
    glColor4ub(R/factAssomb, V/factAssomb, B/factAssomb, 210);
    texArmeHalo->Bind();
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(0.02, 0, haut);
        glTexCoord2f(0, 1); glVertex3f(0.02, 0, 0);
        glTexCoord2f(1, 1); glVertex3f(0.02, larg, 0);
        glTexCoord2f(1, 0); glVertex3f(0.02, larg, haut);
    glEnd();
    
    if(select)
        glColor4ub(0, nvGris/2, nvGris, 230);
    else
        glColor4ub(nvGris, nvGris, nvGris, 230);
    texArmeFantome->Bind();
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(0.03, 0, haut);
        glTexCoord2f(0, 1); glVertex3f(0.03, 0, 0);
        glTexCoord2f(1, 1); glVertex3f(0.03, larg, 0);
        glTexCoord2f(1, 0); glVertex3f(0.03, larg, haut);
    glEnd();
    if(select)
        glColor3ub(nvGris, nvGris, nvGris);
}

int MapGraphique::getHauteur()
{
    return hauteur;
}

int MapGraphique::getLargeur()
{
    return largeur;
}

void MapGraphique::lancerFX(FX* nouvFX)
{
    FXActives.push_back(nouvFX);
}

void MapGraphique::GL_DessinPourSelection(float frameTime, const Camera& camera, unsigned int curseurX, unsigned int curseurY, bool elemsON, bool _clic)
{
    clic = _clic;
    
    if (noircir || statut == PAS_DE_SELECTION)
        return;
    
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    
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
    
    if(statut != CIBLAGE)
    {
        // DESSIN DES TUILES
        for(unsigned int numCase=0; numCase<hauteur*largeur; numCase++)
        {
            int numTuileAct = numsTuiles[numCase];
            if (numTuileAct > 0)
            {
                glPushMatrix();
                glTranslated(coordsCases[numCase][0], coordsCases[numCase][1], coordsCases[numCase][2]);
                
                glPushName(numCase); glPushName(TUILE);
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
                
                glPushName(numCase); glPushName(ELEMENT);
                GL_DessinElement(gestImages->obtenirImage("elements", numElemAct));
                glPopName(); glPopName();
                
                glPopMatrix();
            }
        }
    }
    
    if(statut == CIBLAGE)
    {
        // DESSIN DES PERSOS
        if(imagesPersosChargees)
        {   int numPerso=0;
            for(vector<PersoGraphique>::iterator pAct=listePersos.begin(); pAct!=listePersos.end(); pAct++)
            {
                glPushMatrix();
                
                if(numPerso == numPersoCourant)
                    glTranslatef(persoDep_offsetX, persoDep_offsetY, 0);
                
                glTranslatef(coordsCases[pAct->pos][0], coordsCases[pAct->pos][1], coordsCases[pAct->pos][2]);
                
                glPushName(pAct->pos); glPushName(PERSO);
                GL_DessinPersoPourSelection();
                glPopName(); glPopName();
                
                glPopMatrix();
                
                numPerso++;
            }
        }
    }
}

void MapGraphique::GL_Dessin(float frameTime, const Camera& camera, bool elemsON)
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
    
    if (noircir)
        factAssomb = 5;
    else
    {
        factAssomb = 1;
        list<FX*>::iterator it = FXActives.begin();
        while(it != FXActives.end())
        {
            FX* FXActuel = *it;
            if (FXActuel->effet(this, frameTime))
            {
                delete FXActuel;
                it = FXActives.erase(it);
            }
            else
                it++;
        }
    }
    
    unsigned int nvGris = 255/factAssomb;
    glColor3ub(nvGris, nvGris, nvGris);
    
    bool selec = false;
    
    // DESSIN DU PLATEAU
    texBordure->Bind();
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
    
    // DESSIN DES TUILES
    for(unsigned int numCase=0; numCase<hauteur*largeur; numCase++)
    {
        int numTuileAct = numsTuiles[numCase];
        if (numTuileAct > 0)
        {
            glPushMatrix();
            glTranslatef(coordsCases[numCase][0], coordsCases[numCase][1], coordsCases[numCase][2]);
            
            selec = false;
            if (picked[0] == static_cast<int>(numCase))
                if (picked[1] == TUILE)
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
            glTranslatef(coordsCases[numCase][0], coordsCases[numCase][1], coordsCases[numCase][2]);
            
            selec = false;
            if (picked[0] == static_cast<int>(numCase))
                if (picked[1] == ELEMENT)
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
    
    // DESSIN DES PERSOS
    if(imagesPersosChargees)
    {   int numPerso=0;
        for(vector<PersoGraphique>::iterator pAct=listePersos.begin(); pAct!=listePersos.end(); pAct++)
        {
            glPushMatrix();
            
            if(numPerso == numPersoCourant && cheminDeplacement.size() != 0)
            {
                switch(cheminDeplacement.front())
                {
                    case GAUCHE: persoDep_offsetY -= VITESSE_DEP*frameTime;
                        if(persoDep_offsetY <= 1)
                        {   persoDep_offsetY = 0;
                            pAct->pos--;
                            cheminDeplacement.pop_front();
                        }
                        break;
                    case DROITE: persoDep_offsetY += VITESSE_DEP*frameTime;
                        if(persoDep_offsetY >= 1)
                        {   persoDep_offsetY = 0;
                            pAct->pos++;
                            cheminDeplacement.pop_front();
                        }
                        break;
                    case HAUT: persoDep_offsetX -= VITESSE_DEP*frameTime;
                        if(persoDep_offsetX <= 1)
                        {   persoDep_offsetX = 0;
                            pAct->pos -= largeur;
                            cheminDeplacement.pop_front();
                        }
                        break;
                    case BAS: persoDep_offsetX += VITESSE_DEP*frameTime;
                        if(persoDep_offsetX >= 1)
                        {   persoDep_offsetX = 0;
                            pAct->pos += largeur;
                            cheminDeplacement.pop_front();
                        }
                        break;
                    default: break;
                }
                glTranslatef(persoDep_offsetX, persoDep_offsetY, 0);
            }
            
            glTranslatef(coordsCases[pAct->pos][0], coordsCases[pAct->pos][1], coordsCases[pAct->pos][2]);
            
            selec = false;
            if (picked[0] == static_cast<int>(pAct->pos))
                if (picked[1] == PERSO)
                    selec = true;
            
            GL_DessinPerso(gestImages->obtenirImage("persos", FANTOME), gestImages->obtenirImage("persos", HALO), gestImages->obtenirImage("armes", pAct->arme), gestImages->obtenirImage("armes", (pAct->arme)+1), pAct->clr.R, pAct->clr.V, pAct->clr.B, selec);
            
            glPopMatrix();
            
            numPerso++;
        }
    }
    
    //Calcul des coords du menu triangle
    if(statut == CHOIX_ACTION)
    {
        
    }
}

void MapGraphique::passerSelection(int* selection)
{
    picked[0] = selection[0];
    picked[1] = selection[1];
    clic = false;
}

void MapGraphique::pasDeSelection()
{
    picked[0] = -1; picked[1] = -1;
    clic = false;
}

void MapGraphique::deplacerPersoCourant(list<int> _chemin)
{
    cheminDeplacement = _chemin;
}

}
