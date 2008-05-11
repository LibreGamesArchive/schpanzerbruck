# encoding=UTF-8

import glob, os
from mapbase import MapBase
import infos, constantes
from config import Config

config=Config.getInstance()

class ContexteClient:
    """La classe supervisant un combat (côté client)"""
    
    def __init__(self, moteurJeuWS, fichierMap, touches = config.touches):
        self.map = MapBase(fichierMap)
        self.MJ = moteurJeuWS
        LFI = self.map.listesFichiersImages()
        self.MJ.demarrerMoteurCombat(self.map.largeur, self.map.hauteur, self.map["tuiles"]+[self.map.bordure], self.map["elements"]+[0], LFI["tuiles"]+[LFI["bordure"]], LFI["elements"]+[""], self.map.bordure, 0.4)
        self.MC = self.MJ.getMoteurCombat()
        
        self.persos = []    # persos[0] est toujours le personnage dont c'est actuellement le tour
        # La liste est mise à jour depuis le serveur
    
    
    def persoActuel(self):
        return self.persos[0]
    
    
    def lancerBoucle(self):
        """Lancer la boucle principale du combat"""
        
        # Met le curseur au centre de l'écran (fixe le bug de scrolling)
        self.MC.centrerCurseur()
        
        running = True
        while running:
            # GESTION DES EVENEMENTS :
            running = self.MC.traiterEvenements()
            
            self.MC.afficher()
            
            print self.MC.getFPS()
