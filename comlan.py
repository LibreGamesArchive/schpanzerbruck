# encoding=UTF-8

from threading import Thread
import socket, sys
from vars import reseau


class Joueur:
    """Toutes les infos nécessaires sur un joueur"""
    
    def __init__(self, pseudo):
        self.pseudo = pseudo
        self.connecteAuJeu = True   # True si le Joueur est toujours connecté à la partie


class BaseCom:
    """Communicateur de base, que les joueurs soient locaux ou non"""
    
    def __init__(self):
        self.enFonctionnement = True   # True si le Communicateur est toujours en cours de fonctionnement
    
    
    def arreter(self):
        self.enFonctionnement = False


class BaseComReseau(BaseCom, Thread):
    """Communicateur: La classe de base pour la communication client/serveur
    C'est un Thread, de manière à pouvoir recevoir les infos sans bloquer"""
    
    def __init__(self, socket, contexte):
        BaseCom.__init__(self)
        Thread.__init__(self, name=self.__class__.__name__)
        self.socket = socket
        self.contexte = contexte



class ComClient(BaseComReseau, list):
    """BaseComReseau: Communique vers le ComServeur correspondant
    list: Un groupe de joueurs jouant sur le même PC
    (Peut bien sûr ne contenir qu'un seul joueur)"""
    
    def __init__(self, socket, joueurs, contexte):
        BaseComReseau.__init__(self, socket, contexte)
        list.__init__(self, joueurs)
        self.__numJoueurActuel = -1   # -1 si ce n'est au tour d'aucun des joueurs du groupe de jouer, sinon égal au num du joueur dans le groupe
    
    
    def joueurActuel(self):
        if self.__numJoueurActuel >= 0:
            return self[self.__numJoueurActuel]
        return None
    
    
    def run(self):  # RECEPTION
        """Recoit les infos du ComServeur et appelle les méthodes correspondantes"""
        while self.connecteAuJeu:   # Est exécuté tant que le ComClient est connecté
            if self.joueurActuel():    # Si le joueur actuel est dans le groupe
                socket.recv(1024)



class ComServeur(BaseComReseau):
    """Contient la socket pour communiquer côté serveur avec un ComClient"""
    
    def __init__(self, socket, contexte):
        BaseComReseau.__init__(self, socket, contexte)
        self.nombreJoueurs = 0    # Nombre de joueurs sur le même PC client
    
    
    def run(self):   # RECEPTION
        """Recoit les infos des clients, et appelle les méthodes correspondantes"""
        pass
    
    
    def deconnexion(self, num):
        """Signale la déconnexion d'un joueur du groupe"""
        self.nombreJoueurs -= 1
        if self.nombreJoueurs <= 0:
            self.connecteAuJeu = False



class ComLocal(BaseCom, list):
    """Permet de communiquer avec les joueurs se trouvant sur le même PC que le serveur
    Le ContexteServeur l'utilise de la même manière qu'il utilise le ComServeur"""
    
    def __init__(self, joueurs, ctxClient, ctxServeur):
        BaseCom.__init__(self)
        list.__init__(self, joueurs)
        self.ctxClient = ctxClient
        self.ctxServeur = ctxServeur



class ComConnexions(BaseComReseau):
    """Gère les connexions des nouveaux ComClients en leur attribuant un nouveau ComServeur"""
    
    def __init__(self, contexte):
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)    # La socket locale du serveur
        BaseComReseau.__init__(self, sock, contexte)
        
        try:
            self.socket.bind(reseau.IP_PORT_SERVEUR)
        except socket.error:
            print >> sys.stderr, "Impossible de lier la socket à l'adresse %s et au port %s " % reseau.IP_PORT_SERVEUR
        self.socket.listen(5)
    
    
    def run(self):
        try:
            while self.enFonctionnement:
                socketClient, IP_PortClient = self.socket.accept()
                print "Client %s connecté au port %s" % IP_PortClient
                
                nouveauCom = ComServeur(socketClient, self.contexte)
                self.contexte.coms.append(nouveauCom)
                nouveauCom.start()
        finally:
            self.socket.close()



class ComBroadcasts(BaseComReseau):
    """Le seul Communicateur à fonctionner en UDP.
    Gère les clients qui cherchent le serveur en leur revoyant l'IP/Port du socket du ComConnexions"""
    
    def __init__(self):
        sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)     # Socket en UDP (DATAGRAM)
        BaseComReseau.__init__(self, sock, None)
        self.socket.bind(("", reseau.PORT_BROADCAST))
    
    
    def run(self):
        try:
            while self.enFonctionnement:
                msg, IP_PortClient = self.socket.recvfrom(1024)
                self.socket.sendto(str(reseau.IP_PORT_SERVEUR[1]), IP_PortClient)
        finally:
            self.socket.close()


class ComRecherche(BaseComReseau):
    """Côté client, sert à chercher les serveurs sur le réseau"""
    
    def __init__(self):
        sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        BaseComReseau.__init__(self, sock, None)
        self.socket.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, True)
        self.socket.settimeout(3)
        self.serveursTrouves = []    # Contient les tuples (IP, port) des serveurs détectés
        self.__doitChercher = False
    
    def lancerRecherche(self):
        self.__doitChercher = True
    
    def __chercherServeurs(self):
        """Cherche la liste des serveurs sur le réseau par un envoi en broadcast"""
        
        self.serveursTrouves = []   # On vide la liste des serveurs
        self.socket.sendto("...", ("255.255.255.255", reseau.PORT_BROADCAST))
        try:
            while True:
                msg, (IP_Serveur, PortServeur) = self.socket.recvfrom(1024)    # Le message reçu est le numéro de port du socket TCP attendant les connexions sur le serveur
                nouvServeur = (IP_Serveur, int(msg))
                self.serveursTrouves.append(nouvServeur)
        except socket.timeout:
            pass
        finally:
            self.__doitChercher = False
    
    def run(self):
        try:
            while self.enFonctionnement:
                if self.__doitChercher:
                    self.__chercherServeurs()
        except socket.error:
            # A FAIRE : GERER LES ERREURS RESEAU
            raise
        finally:
            self.__doitChercher = False
            self.socket.close()
    
    def rechercheEnCours(self):
        return self.__doitChercher