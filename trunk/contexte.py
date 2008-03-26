# encoding=UTF-8

from PySFML import sf
import mapmng, gui, img, utils, constantes


class ContexteCombat:
    """La classe suprême supervisant un combat"""
    
    def __init__(self, app, fichierMap, perspective = constantes.defaut.PERSPECTIVE, touches = constantes.defaut.touches):
        self.app = app
        self.gestImages = img.GestionnaireImages()
        self.map = mapmng.Map(fichierMap, self.gestImages, perspective)
        self.interface = gui.InterfaceCombat()
        
        self.joueursLocaux = []  # A changer quand la classe JoueurClient sera finie
        
        self.persosAyantJoue = []
        self.persosRestants = []    # persosRestants[0] est toujours le personnage dont c'est actuellement le tour
        
        self.vitesseDefil = 700    # Vitesse de défilement de la map (en pixels/seconde)
        self.bordureDefil = 40    # Taille de la bordure pour le défilement (en pixels)
        
        self.touches = touches
    
    
    def persoActuel(self):
        return self.persosRestants[0]
    
    
    def auSuivant(self):
        self.persosAyantJoue.append(self.persosRestants.pop(0))
        # A FAIRE :
        # Trier les persosAyantJoue selon leur INITIATIVE
        if len(self.persosRestants) == 0:
            self.persosRestants = self.persosAyantJoue
            self.persosAyantJoue = []
    
    
    def lancerBoucle(self):
        """Lancer la boucle principale du combat"""
        
        w, h = self.app.GetWidth(), self.app.GetHeight()
        vueMap = sf.View(sf.FloatRect(0, 0, w, h))
        vueInterface = sf.View(sf.FloatRect(0, 0, w, h))
        
        run = True
        evt = sf.Event()
        while run:
            # GESTION DES EVENEMENTS :
            while run and self.app.GetEvent(evt):
                if evt.Type == sf.Event.Closed:
                    run = False
                elif evt.Type == sf.Event.KeyPressed:
                    if evt.Key.Code == sf.Key.Escape:
                        self.map.noircir()
                        # A FAIRE :
                        # Interface : Afficher le menuEchap
                        run = False
                elif evt.Type == sf.Event.MouseButtonPressed:
                    res = self.interface.gererClic(evt, vueInterface)  # L'interface est bien entendu prioritaire sur la Map dans la gestion des clics
                    if res == None:
                        res = self.map.gererClic(evt, vueMap)
                    else:
                        run = not res.quitter
            
            # Gestion de la souris en temps réel :
            input = self.app.GetInput()
            curseurX, curseurY = input.GetMouseX(), input.GetMouseY()
            
            # ZOOM de la Map :
            if input.IsKeyDown(self.touches.ZOOM_AVANT) and vueMap.Zoom < 1:
                vueMap.Zoom += self.app.GetFrameTime()
                if vueMap.Zoom > 1:
                    vueMap.Zoom = 1
            elif input.IsKeyDown(self.touches.ZOOM_ARRIERE) and vueMap.Zoom > 0.3:
                vueMap.Zoom -= self.app.GetFrameTime()
                if vueMap.Zoom < 0.3:
                    vueMap.Zoom = 0.3
            
            # SCROLLING de la Map :
            defil = self.vitesseDefil * self.app.GetFrameTime()
            W, H = self.app.GetWidth(), self.app.GetHeight()
            
            if curseurX <= self.bordureDefil and vueMap.Rect.Left > self.map.rect.Left:   # DEFILEMENT VERS LA GAUCHE
                vueMap.Rect.Left -= defil
                vueMap.Rect.Right -= defil
                if vueMap.Rect.Left < self.map.rect.Left:
                    vueMap.Rect.Left = self.map.rect.Left
                    vueMap.Rect.Right = self.map.rect.Left + W
            elif curseurX >= self.app.GetWidth() - self.bordureDefil and vueMap.Rect.Right < self.map.rect.Right:   # DEFILEMENT VERS LA DROITE
                vueMap.Rect.Left += defil
                vueMap.Rect.Right += defil
                if vueMap.Rect.Right > self.map.rect.Right:
                    vueMap.Rect.Left = self.map.rect.Right - W
                    vueMap.Rect.Right = self.map.rect.Right
            if curseurY <= self.bordureDefil and vueMap.Rect.Top > self.map.rect.Top:   # DEFILEMENT VERS LE HAUT
                vueMap.Rect.Top -= defil
                vueMap.Rect.Bottom -= defil
                if vueMap.Rect.Top < self.map.rect.Top:
                    vueMap.Rect.Top = self.map.rect.Top
                    vueMap.Rect.Bottom = self.map.rect.Top + H
            elif curseurY >= self.app.GetHeight() - self.bordureDefil and vueMap.Rect.Bottom < self.map.rect.Bottom:   # DEFILEMENT VERS LE BAS
                vueMap.Rect.Top += defil
                vueMap.Rect.Bottom += defil
                if vueMap.Rect.Bottom > self.map.rect.Bottom:
                    vueMap.Rect.Top = self.map.rect.Bottom - H
                    vueMap.Rect.Bottom = self.map.rect.Bottom
            
            t = self.app.GetFrameTime()
            print "FPS: %.2f" % (1/(t if t>0 else 1))
            
            # DESSIN :
            self.app.SetView(vueMap)
            self.app.Draw(self.map)
            self.app.SetView(vueInterface)
            self.app.Draw(self.interface)
            self.app.Display()