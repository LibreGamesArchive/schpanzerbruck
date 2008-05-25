#include <MoteurCombat.hpp>

namespace ws
{

MoteurCombat::MoteurCombat(sf::RenderWindow* _app, GestionnaireImages* _gestImages, const DonneesMap& _DM, const Touches& _touches)
{
    app = _app;
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

int* MoteurCombat::maitrisesChoisies()
{
    return gui->mtrChoisies;
}

void MoteurCombat::setInfosDsBarre(string tuile, string element, string perso)
{
    gui->setInfosDsBarre(tuile, element, perso);
}


void MoteurCombat::traiterSelectInterface(int* selec, bool clic, unsigned int& whatHappens)
{
    switch(selec[0])
    {
        case IC::SLC_MENU_ECHAP:
            switch(selec[1])
            {
                case IC::SLC_CONTINUER:
                    if(clic)
                    {   gui->switchMenuEchap();
                        mapGraph->noircir = false;
                    }
                    break;
                case IC::SLC_QUITTER:
                    if(clic)
                        whatHappens = QUITTER;
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
                if (mapGraph->noircir)
                    break;
                camera->pos[2] -= evt.MouseWheel.Delta;
                if (camera->pos[2] < 3)
                    camera->pos[2] = 3;
                if (camera->pos[2] > 12)
                    camera->pos[2] = 12;
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
        
        if (input.IsKeyDown(sf::Key::A))
            elemsON = false;
        else
            elemsON = true;
        
        scrolling();
    }
    
    GLuint buffer[512];
    glSelectBuffer(512, buffer);
    
    glRenderMode(GL_SELECT);
    glInitNames();
    
    glPushName(0); // O pour la Map, 1 pour l'interface
    mapGraph->GL_DessinPourSelection(app->GetFrameTime(), *camera, curseurX, curseurY, elemsON, clic);
    glPopName();
    
    glPushName(1);
    gui->GL_DessinPourSelection(curseurX, curseurY, clic);
    glPopName();

    GLuint hits = glRenderMode(GL_RENDER);
    GLfloat plusPetitZ_min = 1.0;
    GLuint clicSur = 0;
    int selection[2];
    selection[0] = -1;
    selection[1] = -1;
    
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
                clicSur = *ptr; ptr++;   // Le premier nom est 0 ou 1 (map ou interface)
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
        
        if(clicSur == 0)   // CLIC SUR LA MAP
        {
            mapGraph->passerSelection(selection);
            gui->pasDeSelection();
        }
        else    // CLIC SUR L'INTERFACE
        {
            mapGraph->pasDeSelection();
            gui->passerSelection(selection);
            traiterSelectInterface(selection, clic, whatHappens);
            gui->setInfosDsBarre(); // Vide la barre d'infos, vu que le curseur n'est pas sur la map
        }
    }
    else
    {    mapGraph->pasDeSelection(); gui->pasDeSelection(); gui->setInfosDsBarre();    }
    
    
    return whatHappens;
}

}
