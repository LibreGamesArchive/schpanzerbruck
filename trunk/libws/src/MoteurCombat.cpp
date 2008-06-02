#include <MoteurCombat.hpp>

namespace ws
{

MoteurCombat::MoteurCombat(sf::Window* _app, GestionnaireImages* _gestImages, const DonneesMap& _DM, const Touches& _touches)
{
    app = _app;
    gestImages = _gestImages;
    L = app->GetWidth();
    H = app->GetHeight();
    mapGraph = new MapGraphique(_gestImages, _DM, L, H);
    gui = new InterfaceCombat(_gestImages, L, H);
    
    touches = _touches;
    
    vitesseDefil = 8;
    bordureDefil = 40;
    
    curseurX = 0;
    curseurY = 0;
    
    camera = new Camera;
    camera->pos[0] = mapGraph->getHauteur()/2 + 4;
    camera->pos[1] = mapGraph->getLargeur()/2;
    camera->pos[2] = 4;
    camera->cible[0] = mapGraph->getHauteur()/2;
    camera->cible[1] = mapGraph->getLargeur()/2;
    camera->cible[2] = 0;
    
    elemsON = true;
    
    glCullFace( GL_BACK );
    glEnable( GL_CULL_FACE );
}

MoteurCombat::~MoteurCombat()
{
    delete mapGraph;
    delete gui;
    delete camera;
}

void MoteurCombat::scrolling()
{    
    float defil = vitesseDefil * app->GetFrameTime();

    if (curseurX <= bordureDefil && camera->cible[1] > -2)   // DEFILEMENT VERS LA GAUCHE
    {   camera->pos[1] -= defil;
        camera->cible[1] -= defil;
    }
    else
        if (curseurX >= L - bordureDefil and camera->cible[1] < mapGraph->getLargeur()+2)   // DEFILEMENT VERS LA DROITE
        {   camera->pos[1] += defil;
            camera->cible[1] += defil;
        }
    if (curseurY <= bordureDefil and camera->cible[0] > 0)   // DEFILEMENT VERS LE HAUT
    {   camera->pos[0] -= defil;
        camera->cible[0] -= defil;
    }
    else
        if (curseurY >= H - bordureDefil and camera->cible[0] < mapGraph->getHauteur())   // DEFILEMENT VERS LE BAS
        {   camera->pos[0] += defil;
            camera->cible[0] += defil;
        }
}

void MoteurCombat::zoom(float delta)
{
    if (delta != 0 && !mapGraph->noircir)
    {
        camera->pos[2] -= delta;
        if (camera->pos[2] < 3)
            camera->pos[2] = 3;
        if (camera->pos[2] > 12)
            camera->pos[2] = 12;
    }
}

void MoteurCombat::centrerCurseur()
{
    // Met le curseur au centre de l'écran (fixe le bug de scrolling)
    app->SetCursorPosition(L/2, H/2);
}

float MoteurCombat::getFPS()
{
    float frameTime = app->GetFrameTime();
    if (frameTime != 0)
        return 1.0/(frameTime);
    return 0;
}

void MoteurCombat::afficher()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    mapGraph->GL_Dessin(app->GetFrameTime(), *camera, elemsON);
    gui->GL_Dessin();
    
    app->Display();
}

int* MoteurCombat::selectMapActuelle()
{
    return mapGraph->picked;
}

void MoteurCombat::maitrisesChoisies(int* mtrChoisies, int* gradesChoisis)
{
    for(unsigned int i=0; i<3; i++)
    {
        mtrChoisies[i] = gui->mtrChoisies[i];
        gradesChoisis[i] = gui->gradesChoisis[i];
    }
}

void MoteurCombat::setInfosDsBarre(string tuile, string element, string perso)
{
    gui->setInfosDsBarre(tuile, element, perso);
}

void MoteurCombat::setChrono(float temps)
{
    gui->valChrono = temps;
}

void MoteurCombat::setInfosPersoActuel(string nom, float VIE, float FTG)
{
    gui->infosPersoActuel.nom = nom;
    gui->infosPersoActuel.VIE = VIE;
    gui->infosPersoActuel.FTG = FTG;
}

void MoteurCombat::setMaitrisesAffichees(vector<string> listeMtr, vector<int> listeGrades)
{
    gui->mtrAffichees = listeMtr;
    gui->gradesMtrAffichees = listeGrades;
    for(unsigned int i=0; i<3; i++)
    {
        gui->mtrChoisies[i] = -1;
        gui->gradesChoisis[i] = -1;
    }
    gui->numPremMtrAffichee = 0;
}

void MoteurCombat::setListePersos(vector<PersoGraphique> listePersos)
{
    mapGraph->listePersos = listePersos;
}

void MoteurCombat::chargerImagesPersos(string cheminFantome, string cheminHalo, map<int, string> cheminsArmes)
{
    gestImages->chargerImage("persos", FANTOME, cheminFantome);
    gestImages->chargerImage("persos", HALO, cheminHalo);
    
    for(map<int, string>::iterator it=cheminsArmes.begin(); it!=cheminsArmes.end(); it++)
        gestImages->chargerImage("armes", it->first, it->second);
    
    mapGraph->imagesPersosChargees = true;
}


void MoteurCombat::traiterSelectInterface(int* selec, bool clic, float delta, unsigned int& whatHappens)
{
    switch(selec[0])
    {
        case SLC_MENU_ECHAP:
            switch(selec[1])
            {
                case SLC_CONTINUER:
                    if(clic)
                    {   gui->switchMenuEchap();
                        mapGraph->noircir = false;
                    }
                    break;
                
                case SLC_QUITTER:
                    if(clic)
                        whatHappens = QUITTER;
                    break;
                
                default: break;
            }
            break;
        
        case SLC_FENETRE_MAITRISES:
            switch(selec[1])
            {
                case SLC_LISTE_MAITRISES:
                    if(clic)
                    {
                        int numMtrDsChoix = gui->numMtrDsChoix(selec[2]);
                        if(numMtrDsChoix != -1)     // Si la maitrise a déjà été choisie
                        {
                            if(selec[3] == SLC_MAITRISE)    // Si on a cliqué sur le nom de la maitrise, on la dé-choisit
                            {   gui->mtrChoisies[numMtrDsChoix] = -1;
                                gui->gradesChoisis[numMtrDsChoix] = -1;
                            }
                            else    // Si on a cliqué sur un grade
                                if(selec[3] == gui->gradesChoisis[numMtrDsChoix])    // Si le grade cliqué est le grade déjà choisi, on dé-choisit la maitrise
                                {   gui->mtrChoisies[numMtrDsChoix] = -1;
                                    gui->gradesChoisis[numMtrDsChoix] = -1;
                                }
                                else    // Sinon, on change le grade choisi
                                    gui->gradesChoisis[numMtrDsChoix] = selec[3];
                        }
                        else
                        {
                            for(unsigned int i=0; i<3; i++)     // On trouve si possible une place vide pour ajouter la maitrise choisie
                            {   if(gui->mtrChoisies[i] == -1)
                                {   gui->mtrChoisies[i] = selec[2];
                                    if(selec[3] == SLC_MAITRISE)    // Si l'on a pas spécifié de grade, on met le grade le plus fort
                                        gui->gradesChoisis[i] = gui->gradesMtrAffichees[selec[2]];
                                    else
                                        gui->gradesChoisis[i] = selec[3];
                                    break;
                                }
                            }
                        }
                    }
                    if(delta > 0 && gui->numPremMtrAffichee > 0)
                        gui->numPremMtrAffichee--;
                    else if(delta < 0 && gui->numPremMtrAffichee + 16 <= gui->mtrAffichees.size())
                        gui->numPremMtrAffichee++;
                    break;
                
                case SLC_VALIDER_MAITRISES:
                    if(clic)
                        whatHappens = MAITRISES_CHOISIES;
                
                case SLC_FERMER_MAITRISES:
                    if(clic)
                        gui->switchFenetreMaitrises();
                    break;
                
                default: break;
            }
            break;
        
        default: break;
    }
}

unsigned int MoteurCombat::traiterEvenements()
{
    bool clic = false;
    sf::Event evt;
    float delta = 0;    // Pour la rotation de la molette
    
    unsigned int whatHappens = RAS;
    
    while(app->GetEvent(evt))
    {
        switch(evt.Type)
        {
            case sf::Event::Closed:
               whatHappens = QUITTER;
               break;
            
            case sf::Event::KeyPressed:
                switch(evt.Key.Code)
                {
                    case sf::Key::Escape:
                        mapGraph->noircir = !mapGraph->noircir;
                        gui->switchMenuEchap();
                        break;
                    
                    default: break;
                }
                break;
            
            case sf::Event::MouseButtonPressed:
                switch(evt.MouseButton.Button)
                {
                    case sf::Mouse::Left:
                        clic = true;
                        break;
                    
                    default: break;
                }
                break;
            
            case sf::Event::MouseWheelMoved:  // ZOOM de la Map avec la molette de la souris
                delta = evt.MouseWheel.Delta;
                break;
            
            default: break;
        }
    }
    
    const sf::Input& input = app->GetInput();
    // Gestion de la souris en temps réel :
    curseurX = input.GetMouseX();
    curseurY = input.GetMouseY();
    
    if (!mapGraph->noircir)
    {
        // ZOOM de la Map avec le clavier :
        if (input.IsKeyDown(touches.zoomAvant) and camera->pos[2] > 3)
            camera->pos[2] -= 10*app->GetFrameTime();
        else
            if (input.IsKeyDown(touches.zoomArriere) and camera->pos[2] < 12)
                camera->pos[2] += 10*app->GetFrameTime();
        
        elemsON = !input.IsKeyDown(sf::Key::A);
        
        scrolling();
    }
    
    GLuint buffer[512];
    glSelectBuffer(512, buffer);
    
    glRenderMode(GL_SELECT);
    glInitNames();
    
    glPushName(MAP);
    mapGraph->GL_DessinPourSelection(app->GetFrameTime(), *camera, curseurX, curseurY, elemsON, clic);
    glPopName();
    
    glPushName(INTERFACE);
    gui->GL_DessinPourSelection(curseurX, curseurY, clic);
    glPopName();
    
    GLuint hits = glRenderMode(GL_RENDER);
    GLfloat plusPetitZ_min = 1.0;
    GLuint curseurSur = 0;
    int selection[4];
    selection[0] = -1;
    selection[1] = -1;
    selection[2] = -1;
    selection[3] = -1;
    
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
            if (z_min <= plusPetitZ_min)
            {
                curseurSur = *ptr; ptr++;   // Le premier nom est 0 ou 1 (map ou interface)
                for(GLuint j=0; j<nbrNames-1; j++)
                {
                    selection[j] = *ptr; ptr++;
                }
                plusPetitZ_min = z_min;
            }
            else
                for(GLuint j=0; j<nbrNames; j++)
                    ptr++;
        }
        
        if(curseurSur == MAP)   // CURSEUR SUR LA MAP
        {
            mapGraph->passerSelection(selection);
            gui->pasDeSelection();
            
            zoom(delta);
        }
        else    // CLIC SUR L'INTERFACE
        {
            mapGraph->pasDeSelection();
            gui->passerSelection(selection);
            traiterSelectInterface(selection, clic, delta, whatHappens);
            gui->setInfosDsBarre(); // Vide la barre d'infos, vu que le curseur n'est pas sur la map
        }
    }
    else
    {   mapGraph->pasDeSelection(); gui->pasDeSelection(); gui->setInfosDsBarre();
        
        zoom(delta);
    }
    
    return whatHappens;
}

}
