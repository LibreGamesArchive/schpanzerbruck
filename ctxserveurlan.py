# encoding=UTF-8

from __future__ import with_statement
import comlan
from threading import Thread, Lock

lockCtxServeur = Lock()


class ContexteServeur(Thread):
    """La classe suprême supervisant un combat (côté serveur)
    Fait tous les calculs nécessaires au déroulement du jeu, et envoie via les ComsServeurs les résultats à tous les ComsClients,
    il ne contient donc AUCUNE instruction réseau autre que la création de sockets"""
    
    def __init__(self, localSeulement=False):
        Thread.__init__(self)
        self.localSeulement = localSeulement
        
        self.coms = []  # Contient des ComsServeurs avec éventuellement un ComLocal
        self.comBroadcasts = (comlan.ComBroadcasts() if not localSeulement else None)
        self.comConnexions = (comlan.ComConnexions(self) if not localSeulement else None)
        
        self.persosAyantJoue = []
        self.persosRestants = []    # persosRestants[0] est toujours le personnage dont c'est actuellement le tour
    
    
    def persoActuel(self):
        """Un bête raccourci"""
        return self.persosRestants[0]
    
    
    def comActuel(self):
        """Ca aussi, c'est un bête raccourci"""
        return self.coms[self.persoActuel().identif[0]]
    
    
    def tourPersoSuivant(self):
        """Lorsque le le ComServeur actuel a intercepté un signal de fin de tour,
        le ContexteServeur actualise la liste des persos, renvoie l'info,
        et signale au joueur suivant que c'est son tour de jouer"""
        
        self.persosAyantJoue.append(self.persosRestants.pop(0))
        # A FAIRE :
        # Trier les persosAyantJoue selon leur INITIATIVE
        if len(self.persosRestants) == 0:
            self.persosRestants = self.persosAyantJoue
            self.persosAyantJoue = []
    
    
    def envoiGlobal(self, msg):
        for com in self.coms:
            com.emission(msg)
    
    
    def lancerJeu(self):
        pass
    
    
    def run(self):
        if not self.localSeulement:
            self.comBroadcasts.start()
            self.comConnexions.start()