# encoding=UTF-8

from PySFML import sf


class ResultatClicInterface:
    """Contient des infos sur le résultat d'un clic sur l'interface"""
    
    def __init__(self):
        self.quitter = False


class InterfaceCombat(sf.Drawable):
    """Définit l'IHM, i.e. l'ensemble des cadres/fenêtres affichés par dessus la Map"""
    
    def __init__(self):
        sf.Drawable.__init__(self)
        self.fenetreInfos = None
        self.barreInfos = None
        self.infosPersoActuel = None
        self.menuEchap = None
        self.menuTriangle = None
        self.fenetreMaitrises = None
        self.cadreTempsRestant = None
    
    
    def Render(self, renderWindow):
        # A FAIRE
        pass
    
    
    def gererClic(self, evt, rect):
        # A FAIRE
        
        return None # On renvoie None si l'interface n'a pas été cliquée
        
        res = ResultatClicInterface()
        return res