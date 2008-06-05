# encoding=UTF-8

import glob, os

from PyWS import ws
from mapbase import MapBase
import infos
from constantes import chemins, armes
from config import Config

config = Config.getInstance()

class CoreClient:
    """La classe supervisant un combat (côté client)"""
    
    def __init__(self, moteurJeuWS, fichierMap, touches = config.touches):
        self.map = MapBase(fichierMap)
        self.map.demarrerMoteurCombat(moteurJeuWS)
        self.MC = moteurJeuWS.getMoteurCombat()
        
        chemPersoF = os.path.join(chemins.IMGS_PERSOS_ET_ARMES, "persoFantome.png")
        chemPersoH = os.path.join(chemins.IMGS_PERSOS_ET_ARMES, "persoHalo.png")
        chemEpeeF = os.path.join(chemins.IMGS_PERSOS_ET_ARMES, "epeeFantome.png")
        chemEpeeH = os.path.join(chemins.IMGS_PERSOS_ET_ARMES, "epeeHalo.png")
        chemHacheF = os.path.join(chemins.IMGS_PERSOS_ET_ARMES, "hacheFantome.png")
        chemHacheH = os.path.join(chemins.IMGS_PERSOS_ET_ARMES, "hacheHalo.png")
        self.MC.chargerImagesPersos(chemPersoF, chemPersoH, {armes.EPEE: chemEpeeF, armes.EPEE+1: chemEpeeH, armes.HACHE: chemHacheF, armes.HACHE+1: chemHacheH})    # Passe au MoteurCombat les images liées aux personnages (fantôme, halo, armes)
        
        
        self.persos = []    # persos[0] est toujours le personnage dont c'est actuellement le tour
        # La liste est mise à jour depuis le serveur
        
        self.MC.setListePersos([(0, armes.EPEE, (255, 0, 255)), (10, armes.EPEE, (0, 255, 0)), (31, armes.HACHE, (255, 0, 0))])
        self.MC.setPersoCourant(True, 0, "Kadoc", 20.76, 40)
        self.MC.deplacerPersoCourant([ws.DROITE, ws.BAS, ws.BAS, ws.BAS, ws.BAS, ws.BAS, ws.DROITE, ws.DROITE, ws.BAS]);
        
        self.MC.setCasesPossibles([0, 1, 2, 15])    # Spécifie la liste des cases accessibles pour un déplacement/une attaque
    
    def persoActuel(self):
        return self.persos[0]
    
    
    def lancerBoucle(self):
        """Lancer la boucle principale du combat"""
        
        # Met le curseur au centre de l'écran (fixe le bug de scrolling)
        self.MC.centrerCurseur()
        
        running = True
        while running:
            # GESTION DES EVENEMENTS :
            whatHappens = self.MC.evenementsEtAffichage()
            
            numCase, numObj = self.MC.selectMapActuelle()   # numObj == ws.TUILE, ws.ELEMENT ou ws.PERSO
            
            if numCase != -1:
                self.MC.setInfosDsBarre(self.map.infosSur("tuiles", numCase, "nom"), self.map.infosSur("elements", numCase, "nom"), "")
            
            self.MC.setChrono(self.MC.getFPS())
            
            #self.MC.setMaitrisesAffichees([("Poulette",ws.GRADE_D), ("Gneuh",ws.GRADE_E), ("Pouik",ws.GRADE_B), ("Pouik",ws.GRADE_A), ("Gneuh",ws.GRADE_C)])
            
            #self.MC.maitrisesChoisies()    #Pour récupérer les 3 tuples (num_maitrise, grade) choisis
            
            if whatHappens == ws.QUITTER:
                running = False
