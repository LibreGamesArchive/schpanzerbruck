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
        self.map.demarrerMoteurCombat(self.MJ)
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
            whatHappens = self.MC.traiterEvenements()
            
            self.MC.afficher()
            
            print self.MC.getFPS()
            
            if whatHappens == self.MC.QUITTER:
                running = False
