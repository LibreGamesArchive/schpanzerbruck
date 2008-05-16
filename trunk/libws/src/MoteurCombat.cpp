#include <MoteurCombat.hpp>

namespace ws
{
MoteurCombat::MoteurCombat(sf::RenderWindow* _app, GestionnaireImages* _gestImages, const DonneesMap& _DM, const Touches& _touches)
{
    app = _app;
    L = app->GetWidth();
    H = app->GetHeight();
    mapGraph = new MapGraphique(_gestImages, _DM, L, H);
    gui = new InterfaceCombat(L, H);

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

    float frameTime = app->GetFrameTime();
    mapGraph->GL_Dessin(frameTime, *camera, elemsON);
    gui->GL_Dessin();
    
    GLuint buffer[512];
    glSelectBuffer(512, buffer);
    
    glRenderMode(GL_SELECT);
    glInitNames();
    
    glPushName(0); // O pour la Map, 1 pour l'interface
    mapGraph->GL_DessinPourSelection(frameTime, *camera, curseurX, curseurY, elemsON);
    glPopName();
    
    glPushName(1);
    gui->GL_DessinPourSelection(curseurX, curseurY);
    glPopName();

    GLuint hits = glRenderMode(GL_RENDER);
    GLfloat plusPetitZ_min = 1.0;
    GLuint clicSur = 0;
    int selection[2];
    
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
        
        if (clicSur == 0)   // CLIC SUR LA MAP
        {
            mapGraph->traiterSelection(selection);
            gui->pasDeSelection();
        }
        else    // CLIC SUR L'INTERFACE
        {
            mapGraph->pasDeSelection();
            gui->traiterSelection(selection);
        }
    }
    else
    {    mapGraph->pasDeSelection(); gui->pasDeSelection();    }

    app->Display();
}

bool MoteurCombat::traiterEvenements()
{
    bool running = true;
    sf::Event evt;
    while (running && app->GetEvent(evt))
    {
        if (evt.Type == sf::Event::Closed)
            running = false;
        
        else
            if (evt.Type == sf::Event::KeyPressed)
            {   if (evt.Key.Code == sf::Key::Escape)
                {   mapGraph->noircir();
                    gui->switchMenuEchap();
                    running = false;
                }
            }
        
        else
            if (evt.Type == sf::Event::MouseWheelMoved)  // ZOOM de la Map avec la molette de la souris
            {
                camera->pos[2] -= 50*evt.MouseWheel.Delta*app->GetFrameTime();
                if (camera->pos[2] < 3)
                    camera->pos[2] = 3;
                if (camera->pos[2] > 12)
                    camera->pos[2] = 12;
            }
    }
    
    const sf::Input& input = app->GetInput();
    
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
    
    // Gestion de la souris en temps réel :
    curseurX = input.GetMouseX();
    curseurY = input.GetMouseY();

    scrolling();

    return running;
}
}
