# encoding=UTF-8

from PySFML import sf
from OpenGL.GL import *


class ResultatClicInterface:
    """Contient des infos sur le résultat d'un clic sur l'interface"""
    
    def __init__(self):
        self.quitter = False


class InterfaceCombat(sf.Drawable):
    """Définit l'IHM, i.e. l'ensemble des cadres/fenêtres affichés par dessus la Map"""
    
    def __init__(self):
        sf.Drawable.__init__(self)
        self.cadreInfos = None
        self.barreInfos = None
        self.infosPersoActuel = None
        self.menuEchap = None
        self.menuTriangle = None
        self.cadreMaitrises = None
        self.cadreTempsRestant = None
        
        self.cadre = Cadre()
    
    
    def Render(self, renderWindow):
        renderWindow.Draw(self.cadre)
        pass
    
    
    def gererClic(self, evt, vue):
        # A FAIRE
        
        return None # On renvoie None si l'interface n'a pas été cliquée
        
        res = ResultatClicInterface()
        return res


class Cadre(sf.Drawable):
    """Classe de base pour les cadres (fenêtres internes)"""
    
    def __init__(self, texture=None):
        sf.Drawable.__init__(self)
        self.texture = texture
    
    def Render(self, renderWindow):
        glBegin(GL_QUADS)
        glVertex2f(0, 0)
        glVertex2f(0, 50)
        glVertex2f(50, 50)
        glVertex2f(50, 0)
        glEnd()


