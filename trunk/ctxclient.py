# encoding=UTF-8

import glob, os
import mapclient, img, infos, utils, constantes


class ContexteClient:
    """La classe supervisant un combat (côté client), c'est le GroupeClient qui opère dessus.
    Il s'occupe des phases de rendu à l'écran et de gestion des événements souris/clavier"""
    
    def __init__(self, moteurJeuWS, fichierMap, touches = constantes.defaut.touches):
        # # #
        self.MC = moteurJeuWS.getMoteurCombat()
        
        self.persos = []    # persos[0] est toujours le personnage dont c'est actuellement le tour
        # La liste est mise à jour depuis le serveur
    
    
    def persoActuel(self):
        return self.persos[0]
    
    
    def lancerBoucle(self):
        """Lancer la boucle principale du combat"""
        
        # Met le curseur au centre de l'écran (fixe le bug de scrolling)
        self.MC.centrerCurseur()
        
        running = True
        evt = sf.Event()
        while running:
            # GESTION DES EVENEMENTS :
            self.MC.gestionClavierSouris()
            
            self.MC.afficher()
            
            self.MC.getFPS()
