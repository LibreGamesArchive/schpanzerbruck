# encoding=UTF-8

from PySFML import sf
from OpenGL.GL import *
from OpenGL.GLU import gluPerspective, gluLookAt
import mapmng, img, utils, constantes
#import gui


class ContexteClient:
    """La classe supervisant un combat (côté client), c'est le GroupeClient qui opère dessus.
    Il s'occupe des phases de rendu à l'écran et de gestion des événements souris/clavier"""
    
    def __init__(self, app, fichierMap, touches = constantes.defaut.touches):
        self.app = app
        self.L, self.H = self.app.GetWidth(), self.app.GetHeight()
        self.input = app.GetInput()
        self.gestImages = img.GestionnaireImages()
        self.map = mapmng.Map(fichierMap, self.gestImages)
        #self.interface = gui.InterfaceCombat(self.L, self.H)
        
        self.persos = []    # persos[0] est toujours le personnage dont c'est actuellement le tour
        # La liste est mise à jour depuis le serveur
        
        self.vitesseDefil = 8    # Vitesse de défilement de la map (en cases/seconde)
        self.bordureDefil = 40    # Taille de la bordure pour le défilement (en pixels)
        
        self.touches = touches
        
        self.camera = utils.Container()
        self.camera.pos = [self.map.hauteur/2 + 3, self.map.largeur/2, 4.5]
        self.camera.cible = [self.map.hauteur/2 - 2, self.map.largeur/2, 0]
        
        #self.vueInterface = sf.View(sf.FloatRect(0, 0, self.L, self.H))
    
    
    def persoActuel(self):
        return self.persos[0]
    
    
    def __scrolling(self, curseurX, curseurY):
        """Scrolling de la map"""
        
        defil = self.vitesseDefil * self.app.GetFrameTime()
        
        if curseurX <= self.bordureDefil:   # DEFILEMENT VERS LA GAUCHE
            self.camera.pos[1] -= defil
            self.camera.cible[1] -= defil
        elif curseurX >= self.app.GetWidth() - self.bordureDefil:   # DEFILEMENT VERS LA DROITE
            self.camera.pos[1] += defil
            self.camera.cible[1] += defil
        if curseurY <= self.bordureDefil:   # DEFILEMENT VERS LE HAUT
            self.camera.pos[0] -= defil
            self.camera.cible[0] -= defil
        elif curseurY >= self.app.GetHeight() - self.bordureDefil:   # DEFILEMENT VERS LE BAS
            self.camera.pos[0] += defil
            self.camera.cible[0] += defil
    
    
    def lancerBoucle(self):
        """Lancer la boucle principale du combat"""
        
        glMatrixMode(GL_PROJECTION)
        glLoadIdentity()
        gluPerspective(70, float(self.app.GetWidth())/self.app.GetHeight(), 1, 50)
        glEnable(GL_DEPTH_TEST)
        glEnable(GL_BLEND)
        glEnable(GL_ALPHA_TEST)
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)
        glAlphaFunc(GL_GREATER, 0)
        
        running = True
        evt = sf.Event()
        while running:
            # GESTION DES EVENEMENTS :
            while running and self.app.GetEvent(evt):
                if evt.Type == sf.Event.Closed:
                    running = False
                elif evt.Type == sf.Event.KeyPressed:
                    if evt.Key.Code == sf.Key.Escape:
                        self.map.noircir()
                        # A FAIRE :
                        # Interface : Afficher le menuEchap
                        running = False
                elif evt.Type == sf.Event.MouseButtonPressed:
                    res = None # self.interface.gererClic(evt, self.vueInterface)  # L'interface est bien entendu prioritaire sur la Map dans la gestion des clics
                    if res == None:
                        res = self.map.gererClic(evt)
                    else:
                        running = not res.quitter
            
            # Gestion de la souris en temps réel :
            curseurX, curseurY = self.input.GetMouseX(), self.input.GetMouseY()
            
            # ZOOM de la Map :
            if self.input.IsKeyDown(self.touches.ZOOM_AVANT) and self.camera.pos[2] > 3:
                self.camera.pos[2] -= 10*self.app.GetFrameTime()
            elif self.input.IsKeyDown(self.touches.ZOOM_ARRIERE) and self.camera.pos[2] < 20:
                self.camera.pos[2] += 10*self.app.GetFrameTime()
            
            self.__scrolling(curseurX, curseurY)
            
            # DESSIN :
            
            glMatrixMode(GL_MODELVIEW)
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
            glLoadIdentity()
            gluLookAt(self.camera.pos[0], self.camera.pos[1], self.camera.pos[2], self.camera.cible[0], self.camera.cible[1], self.camera.cible[2], 0, 0, 1)
            
            self.map.GL_Dessin()
            
            self.app.Display()