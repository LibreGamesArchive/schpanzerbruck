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
    
    masqueCasesPossibles = new bool[hauteur*largeur];
    initMasqueCasesPossibles();
    
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
    
    picked[0] = -1; picked[1] = -1; picked[2] = -1;     // ObjetMap sélectionné par le picking.
    /* picked[0] indique la case sélectionnée
       picked[1] indique si c'est la tuile, l'élément ou le perso
       picked[2] indique, si c'est le perso, quel est son numéro (-1 sinon)
    */
    
    // Edition de infosDessinElements:
    for(unsigned int i=0; i<numsElements.size(); i++)
    {
        InfosSupDessin info;
        info.clrCorps.R = 255; info.clrCorps.V = 255; info.clrCorps.B = 255;
        info.alphaCorps = 255;
        info.mourant = false; info.retirer = false;
        infosDessinElements.push_back(info);
    }
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
    
    delete masqueCasesPossibles;
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

void MapGraphique::GL_DessinPerso(list<PersoGraphique>::iterator perso, list<InfosSupDessin>::iterator infosSupDessin, bool select)
{
    float haut=1.8;
    float larg=haut/2;
    glTranslatef(0.7, (1-larg)/2, 0);
    glRotated(inclinaisonElements-90, 0, 1, 0);
    int nvGris = 255/factAssomb;
    
    int R_corps = infosSupDessin->clrCorps.R/factAssomb; // Ne pas confondre avec perso->clr, qui indique la couleur de l'équipe !
    int V_corps = infosSupDessin->clrCorps.V/factAssomb;
    int B_corps = infosSupDessin->clrCorps.B/factAssomb;
    int A_corps = infosSupDessin->alphaCorps;
    int A_halo = A_corps > 20 ? A_corps-20 : 0;
    
    glColor4ub(perso->clr.R/factAssomb, perso->clr.V/factAssomb, perso->clr.B/factAssomb, A_halo);
    gestImages->obtenirImage("persos", HALO)->Bind();
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(0, 0, haut);
        glTexCoord2f(0, 1); glVertex3f(0, 0, 0);
        glTexCoord2f(1, 1); glVertex3f(0, larg, 0);
        glTexCoord2f(1, 0); glVertex3f(0, larg, haut);
    glEnd();
    
    if(select)
        glColor4ub(0, nvGris/2, nvGris, A_corps);
    else
        glColor4ub(R_corps, V_corps, B_corps, A_corps);
    gestImages->obtenirImage("persos", FANTOME)->Bind();
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(0.01, 0, haut);
        glTexCoord2f(0, 1); glVertex3f(0.01, 0, 0);
        glTexCoord2f(1, 1); glVertex3f(0.01, larg, 0);
        glTexCoord2f(1, 0); glVertex3f(0.01, larg, haut);
    glEnd();
    
    glColor4ub(perso->clr.R/factAssomb, perso->clr.V/factAssomb, perso->clr.B/factAssomb, A_halo);
    gestImages->obtenirImage("armes", (perso->arme)+1)->Bind();
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(0.02, 0, haut);
        glTexCoord2f(0, 1); glVertex3f(0.02, 0, 0);
        glTexCoord2f(1, 1); glVertex3f(0.02, larg, 0);
        glTexCoord2f(1, 0); glVertex3f(0.02, larg, haut);
    glEnd();
    
    if(select)
        glColor4ub(0, nvGris/2, nvGris, A_corps);
    else
        glColor4ub(R_corps, V_corps, B_corps, A_corps);
    gestImages->obtenirImage("armes", perso->arme)->Bind();
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
            if(statut == DEPLACEMENT && !masqueCasesPossibles[numCase])
                continue;
            
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
            if(!masqueCasesPossibles[numCase])
                continue;
            
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
            for(list<PersoGraphique>::iterator pAct=listePersos.begin(); pAct!=listePersos.end(); pAct++)
            {
                // Si la case est accessible est que le pAct n'est pas mort
                if(masqueCasesPossibles[pAct->pos] && pAct->pos >= 0 && pAct->pos < static_cast<int>(largeur*hauteur))
                {                
                    glPushMatrix();
                    
                    if(numPerso == numPersoCourant)
                        glTranslatef(persoDep_offsetX, persoDep_offsetY, 0);
                    
                    glTranslatef(coordsCases[pAct->pos][0], coordsCases[pAct->pos][1], coordsCases[pAct->pos][2]);
                    
                    glPushName(pAct->pos); glPushName(PERSO); glPushName(numPerso);
                    GL_DessinPersoPourSelection();
                    glPopName(); glPopName(); glPopName();
                    
                    glPopMatrix();
                }
                
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
        
        glPushMatrix();
        glTranslatef(coordsCases[numCase][0], coordsCases[numCase][1], coordsCases[numCase][2]);
        
        bool accessible = false;
        
        if (numTuileAct > 0)
        {
            accessible = masqueCasesPossibles[numCase];
            
            glColor3ub(nvGris, nvGris, nvGris);
            
            if(accessible)
            {
                if(statut == DEPLACEMENT)
                    glColor3ub(nvGris, nvGris, 0);
                else if(statut == CIBLAGE)
                    glColor3ub(nvGris, nvGris/3, 0);
            }
            if (picked[0] == static_cast<int>(numCase))
            {
                if (picked[1] == TUILE)
                    glColor3ub(0, nvGris/2, nvGris);
            }
            
            GL_DessinTuile(gestImages->obtenirImage("tuiles", numTuileAct));
        }
        else
        {
            glColor3ub(nvGris/4, nvGris/4, nvGris/3);
            glDisable(GL_TEXTURE_2D);
            GL_DessinTuile();
            glEnable(GL_TEXTURE_2D);
        }
        
        glPopMatrix();
    }
    
    // DESSIN DES ELEMENTS
    vector<unsigned int>::iterator elemAct = numsElements.begin();
    vector<InfosSupDessin>::iterator infosElemAct = infosDessinElements.begin();
    unsigned int numCase=0;
    while(elemAct != numsElements.end())
    {
        int numElemAct = *elemAct;
        if (numElemAct > 0)
        {
            glPushMatrix();
            glTranslatef(coordsCases[numCase][0], coordsCases[numCase][1], coordsCases[numCase][2]);
            
            if(elemsON)
                glColor4ub(infosElemAct->clrCorps.R/factAssomb, infosElemAct->clrCorps.V/factAssomb, infosElemAct->clrCorps.B/factAssomb, infosElemAct->alphaCorps);
            else
                glColor4ub(infosElemAct->clrCorps.R/factAssomb, infosElemAct->clrCorps.V/factAssomb, infosElemAct->clrCorps.B/factAssomb, infosElemAct->alphaCorps/5);
            
            if (picked[0] == static_cast<int>(numCase))
            {
                if (picked[1] == ELEMENT)
                    glColor4ub(0, nvGris/2, nvGris, infosElemAct->alphaCorps);
            }
            
            GL_DessinElement(gestImages->obtenirImage("elements", numElemAct));
            
            if(infosElemAct->mourant)
            {
                infosElemAct->alphaCorps -= static_cast<int>(250*frameTime);
                if(infosElemAct->alphaCorps <= 0)    // L'élément est mort
                {
                    *elemAct = 0;
                }
            }
            
            glPopMatrix();
        }
        
        numCase++;
        elemAct++;
        infosElemAct++;
    }
    
    // DESSIN DES PERSOS
    if(imagesPersosChargees)
    {   int numPerso=0;
        list<InfosSupDessin>::iterator infAct = infosDessinPersos.begin();
        list<PersoGraphique>::iterator pAct = listePersos.begin();
        while(pAct != listePersos.end())
        {
            bool increm = true;
            // Si le perso n'est pas mort
            if(pAct->pos >= 0 && pAct->pos < static_cast<int>(largeur*hauteur)) 
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
                
                GL_DessinPerso(pAct, infAct, selec);
                
                // Gestion de la mort du perso
                if(infAct->mourant)
                {
                    infAct->alphaCorps -= static_cast<int>(250*frameTime);
                    if(infAct->alphaCorps <= 0)    // Le perso est mort
                    {
                        if(infAct->retirer)
                        {
                            pAct = listePersos.erase(pAct);
                            infAct = infosDessinPersos.erase(infAct);
                            increm = false;
                            if(numPersoCourant > numPerso)     // On recalcule numPersoCourant pour qu'il soit toujours valide
                                numPersoCourant--;
                            else if(numPersoCourant == numPerso)    // Cas improbable : Le perso qui meurt est le perso courant
                                numPersoCourant = -1;
                        }
                        else
                            pAct->pos = -1;
                    }
                }
                
                // Calcul des coords du menu triangle
                if(statut == CHOIX_ACTION && numPerso == numPersoCourant)
                {
                    GLdouble modelview[16], proj[16];
                    GLint viewport[4];
                    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
                    glGetDoublev(GL_PROJECTION_MATRIX, proj);
                    glGetIntegerv(GL_VIEWPORT, viewport);
                    GLdouble z=0;
                    
                    gluProject(0, -0.4, 1.6, modelview, proj, viewport, &(menuTriangle->btnDeplX), &(menuTriangle->btnDeplY), &z);
                    gluProject(0, 1.4, 1.6, modelview, proj, viewport, &(menuTriangle->btnActionX), &(menuTriangle->btnActionY), &z);
                    gluProject(0, 0.5, -0.7, modelview, proj, viewport, &(menuTriangle->btnPasserX), &(menuTriangle->btnPasserY), &z);
                }
                
                glPopMatrix();
            }
            
            if(increm)
            {
                numPerso++;
                pAct++;
                infAct++;
            }
        }
    }
}

void MapGraphique::passerSelection(int* selection)
{
    picked[0] = selection[0];
    picked[1] = selection[1];
    picked[2] = selection[2];
    clic = false;
}

void MapGraphique::pasDeSelection()
{
    picked[0] = -1; picked[1] = -1;
    clic = false;
}

void MapGraphique::setListePersos(list<PersoGraphique> _listePersos)
{
    listePersos = _listePersos;
    infosDessinPersos.clear();
    for(unsigned int i=0; i<listePersos.size(); i++)
    {
        InfosSupDessin info;
        info.clrCorps.R = 255; info.clrCorps.V = 255; info.clrCorps.B = 255;
        info.alphaCorps = 230;
        info.mourant = false;
        info.retirer = false;
        infosDessinPersos.push_back(info);
    }
}

void MapGraphique::deplacerPersoCourant(list<int> _chemin)
{
    cheminDeplacement = _chemin;
}

void MapGraphique::initMasqueCasesPossibles()
{
    for(unsigned int i=0; i<hauteur*largeur; i++)
        masqueCasesPossibles[i] = false;
}

void MapGraphique::mortPerso(int numPerso, bool retirer)
{
    int num=0;
    for(list<InfosSupDessin>::iterator it=infosDessinPersos.begin(); it!=infosDessinPersos.end(); it++)
    {
        if(num == numPerso)
        {
            it->mourant = true;
            it->retirer = retirer;
            it->clrCorps.R = 255;
            it->clrCorps.V = 0;
            it->clrCorps.B = 0;
            it->alphaCorps = 255;
            break;
        }
        num++;
    }
}

void MapGraphique::mortElement(int numCase)
{
    infosDessinElements[numCase].mourant = true;
    infosDessinElements[numCase].clrCorps.R = 255;
    infosDessinElements[numCase].clrCorps.V = 0;
    infosDessinElements[numCase].clrCorps.B = 0;
    infosDessinElements[numCase].alphaCorps = 255;
}

void MapGraphique::lancerAnimationCombat(int numLanceur, bool cibleEstElement, int numCible, bool aDistance, float modifLanceurVie, float modifLanceurFatigue, float modifCibleVie, float modifCibleFatigue)
{
    
}

bool MapGraphique::deplacementEnCours()
{
    return (cheminDeplacement.size() != 0);
}

bool MapGraphique::actionEnCours()
{
    return false;
}

}
