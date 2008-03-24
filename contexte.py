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
            
            # Gestion de la souris en temps réel pour le scrolling de la Map :
            input = self.app.GetInput()
            curseurX, curseurY = input.GetMouseX(), input.GetMouseY()
            # A TERMINER
            
            # DESSIN :
            # A REGLER : Les vues qui provoquent des Core Dumped
            #self.app.SetView(vueMap)
            self.app.Draw(self.map)
            #self.app.SetView(vueInterface)
            #self.app.Draw(self.interface)
            self.app.Display()