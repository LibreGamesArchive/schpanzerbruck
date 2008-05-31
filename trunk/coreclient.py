# encoding=UTF-8

import glob, os

from PyWS import ws
from mapbase import MapBase
import infos, constantes
from config import Config

config = Config.getInstance()

class CoreClient:
    """La classe supervisant un combat (côté client)"""
    
    def __init__(self, moteurJeuWS, fichierMap, touches = config.touches):
        self.map = MapBase(fichierMap)
        self.MC = self.map.demarrerMoteurCombat(moteurJeuWS)
        
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
            
            numCase, numObj = self.MC.selectMapActuelle()   # numObj == ws.TUILE, ws.ELEMENT ou ws.PERSO
            
            if numCase != -1:
                self.MC.setInfosDsBarre(self.map.infosSur("tuiles", numCase, "nom"), self.map.infosSur("elements", numCase, "nom"), "")
            
            self.MC.setChrono(self.MC.getFPS())
            
            self.MC.setInfosPersoActuel("Kadok", 20.76, 40)
            
            #self.MC.setMaitrisesAffichees([("Poulette",ws.GRADE_D), ("Gneuh",ws.GRADE_E), ("Pouik",ws.GRADE_B), ("Pouik",ws.GRADE_A), ("Gneuh",ws.GRADE_C)])
            
            #self.MC.maitrisesChoisies()    #Pour récupérer les 3 tuples (num_maitrise, grade) choisis
            
            if whatHappens == ws.QUITTER:
                running = False
