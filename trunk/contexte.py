# encoding=UTF-8

from PySFML import sf
import mapmng, gui, img, utils, constantes


class ContexteCombat:
    """La classe suprême supervisant un combat"""
    
    def __init__(self, app, fichierMap, perspective = constantes.defaut.PERSPECTIVE):
        self.app = app
        self.gestImages = img.GestionnaireImages()
        self.map = mapmng.Map(fichierMap, self.gestImages, perspective)
        self.interface = gui.InterfaceCombat()
        
        self.joueurs = {}
        
        self.persosAyantJoue = []
        self.persosRestants = []    # persosRestants[0] est toujours le personnage dont c'est actuellement le tour
        
        self.vitesseDefil = 700    # Vitesse de défilement de la map (en pixels/seconde)
        self.bordureDefil = 40    # Taille de la bordure pour le défilement (en pixels)
    
    
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
        rectMap = sf.FloatRect(0, 0, w, h)
        rectInterface = sf.FloatRect(0, 0, w, h)
        
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
                    res = self.interface.gererClic(evt, rectInterface)  # L'interface est bien entendu prioritaire sur la Map dans la gestion des clics
                    if res == None:
                        res = self.map.gererClic(evt, rectMap)
                    else:
                        run = not res.quitter
            
            # Gestion de la souris en temps réel :
            input = self.app.GetInput()
            curseurX, curseurY = input.GetMouseX(), input.GetMouseY()
            
            # SCROLLING de la Map :
            defil = self.vitesseDefil * self.app.GetFrameTime()
            W, H = self.app.GetWidth(), self.app.GetHeight()
            
            if curseurX <= self.bordureDefil and rectMap.Left > self.map.rect.Left:   # DEFILEMENT VERS LA GAUCHE
                rectMap.Left -= defil
                rectMap.Right -= defil
                if rectMap.Left < self.map.rect.Left:
                    rectMap.Left = self.map.rect.Left
                    rectMap.Right = self.map.rect.Left + W
            elif curseurX >= self.app.GetWidth() - self.bordureDefil and rectMap.Right < self.map.rect.Right:   # DEFILEMENT VERS LA DROITE
                rectMap.Left += defil
                rectMap.Right += defil
                if rectMap.Right > self.map.rect.Right:
                    rectMap.Left = self.map.rect.Right - W
                    rectMap.Right = self.map.rect.Right
            if curseurY <= self.bordureDefil and rectMap.Top > self.map.rect.Top:   # DEFILEMENT VERS LE HAUT
                rectMap.Top -= defil
                rectMap.Bottom -= defil
                if rectMap.Top < self.map.rect.Top:
                    rectMap.Top = self.map.rect.Top
                    rectMap.Bottom = self.map.rect.Top + H
            elif curseurY >= self.app.GetHeight() - self.bordureDefil and rectMap.Bottom < self.map.rect.Bottom:   # DEFILEMENT VERS LE BAS
                rectMap.Top += defil
                rectMap.Bottom += defil
                if rectMap.Bottom > self.map.rect.Bottom:
                    rectMap.Top = self.map.rect.Bottom - H
                    rectMap.Bottom = self.map.rect.Bottom
            
            t = self.app.GetFrameTime()
            print 1/(t if t>0 else 1)
            
            # DESSIN :
            self.app.SetView(sf.View(rectMap))
            self.app.Draw(self.map)
            self.app.SetView(sf.View(rectInterface))
            self.app.Draw(self.interface)
            self.app.Display()