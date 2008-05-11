#include <MoteurCombat.hpp>

namespace ws
{
MoteurCombat::MoteurCombat(sf::RenderWindow* _app, GestionnaireImages* _gestImages, const DonneesMap& _DM, const Touches& _touches)
{
    app = _app;
    L = app->GetWidth();
    H = app->GetHeight();
    mapGraph = new MapGraphique(_gestImages, _DM);

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

    mapGraph->GL_Dessin(app->GetFrameTime(), L, H, *camera, curseurX, curseurY, elemsON);

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
                    // A FAIRE :
                    // Interface : Afficher le menuEchap
                    running = false;
                }
            }
        
        else
            if (evt.Type == sf::Event::MouseWheelMoved)  // ZOOM de la Map avec la molette de la souris
            {
                camera->pos[2] -= (1800.0/(mapGraph->getHauteur()*mapGraph->getLargeur()))*evt.MouseWheel.Delta*app->GetFrameTime();
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

