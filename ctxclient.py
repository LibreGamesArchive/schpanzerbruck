# encoding=UTF-8

from __future__ import with_statement
from PySFML import sf
import mapmng, gui, img, utils, constantes
from threading import Lock
import socket

lockCtxClient = Lock()


def chercherServeurs():
        """Cherche la liste des serveurs sur le réseau par un envoi en broadcast"""
        
        sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        sock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, True)
        sock.sendto("...", ("255.255.255.255", constantes.reseau.PORT_BROADCAST))
        sock.settimeout(3)
        serveursTrouves = []
        try:
            while True:
                msg, (IP_Serveur, PortServeur) = sock.recvfrom(1024)    # Le message reçu est le numéro de port du socket TCP attendant les connexions sur le serveur
                serveursTrouves.append((IP_Serveur, int(msg)))
        except socket.timeout:
            pass
        finally:
            sock.close()
        
        return serveursTrouves


class ContexteClient:
    """La classe supervisant un combat (côté client), c'est le GroupeClient qui opère dessus.
    Il s'occupe des phases de rendu à l'écran et de gestion des événements souris/clavier"""
    
    def __init__(self, app, fichierMap, perspective = constantes.defaut.PERSPECTIVE, touches = constantes.defaut.touches):
        self.app = app
        self.gestImages = img.GestionnaireImages()
        self.map = mapmng.Map(fichierMap, self.gestImages, perspective)
        self.interface = gui.InterfaceCombat()
        
        self.persos = []    # persos[0] est toujours le personnage dont c'est actuellement le tour
        # La liste est mise à jour depuis le serveur
        
        self.vitesseDefil = 700    # Vitesse de défilement de la map (en pixels/seconde)
        self.bordureDefil = 40    # Taille de la bordure pour le défilement (en pixels)
        
        self.touches = touches
    
    
    def persoActuel(self):
        return self.persos[0]
    
    
    def __scrolling(self, vueMap, curseurX, curseurY):
        """Scrolling de la map"""
        
        defil = self.vitesseDefil * self.app.GetFrameTime()
        W, H = self.app.GetWidth(), self.app.GetHeight()
        
        if curseurX <= self.bordureDefil and vueMap.Rect.Left > self.map.rect.Left:   # DEFILEMENT VERS LA GAUCHE
            vueMap.Rect.Left -= defil
            vueMap.Rect.Right -= defil
            if vueMap.Rect.Left < self.map.rect.Left:
                vueMap.Rect.Left = self.map.rect.Left
                vueMap.Rect.Right = self.map.rect.Left + W
        elif curseurX >= self.app.GetWidth() - self.bordureDefil and vueMap.Rect.Right < self.map.rect.Right:   # DEFILEMENT VERS LA DROITE
            vueMap.Rect.Left += defil
            vueMap.Rect.Right += defil
            if vueMap.Rect.Right > self.map.rect.Right:
                vueMap.Rect.Left = self.map.rect.Right - W
                vueMap.Rect.Right = self.map.rect.Right
        if curseurY <= self.bordureDefil and vueMap.Rect.Top > self.map.rect.Top:   # DEFILEMENT VERS LE HAUT
            vueMap.Rect.Top -= defil
            vueMap.Rect.Bottom -= defil
            if vueMap.Rect.Top < self.map.rect.Top:
                vueMap.Rect.Top = self.map.rect.Top
                vueMap.Rect.Bottom = self.map.rect.Top + H
        elif curseurY >= self.app.GetHeight() - self.bordureDefil and vueMap.Rect.Bottom < self.map.rect.Bottom:   # DEFILEMENT VERS LE BAS
            vueMap.Rect.Top += defil
            vueMap.Rect.Bottom += defil
            if vueMap.Rect.Bottom > self.map.rect.Bottom:
                vueMap.Rect.Top = self.map.rect.Bottom - H
                vueMap.Rect.Bottom = self.map.rect.Bottom
    
    
    def lancerBoucle(self):
        """Lancer la boucle principale du combat"""
        
        w, h = self.app.GetWidth(), self.app.GetHeight()
        vueMap = sf.View(sf.FloatRect(0, 0, w, h))
        vueInterface = sf.View(sf.FloatRect(0, 0, w, h))
        
        running = True
        evt = sf.Event()
        while running:
            # GESTION DES EVENEMENTS :
            while running and self.app.GetEvent(evt):
                if evt.Type == sf.Event.Closed:
                    running = False
                elif evt.Type == sf.Event.KeyPressed:
                    if evt.Key.Code == sf.Key.Escape:
                        self.map.noircir()
                        # A FAIRE :
                        # Interface : Afficher le menuEchap
                        running = False
                elif evt.Type == sf.Event.MouseButtonPressed:
                    res = self.interface.gererClic(evt, vueInterface)  # L'interface est bien entendu prioritaire sur la Map dans la gestion des clics
                    if res == None:
                        res = self.map.gererClic(evt, vueMap)
                    else:
                        running = not res.quitter
            
            # Gestion de la souris en temps réel :
            appInput = self.app.GetInput()
            curseurX, curseurY = appInput.GetMouseX(), appInput.GetMouseY()
            
            # ZOOM de la Map :
            if appInput.IsKeyDown(self.touches.ZOOM_AVANT) and vueMap.Zoom < 1:
                vueMap.Zoom += self.app.GetFrameTime()
                if vueMap.Zoom > 1:
                    vueMap.Zoom = 1
            elif appInput.IsKeyDown(self.touches.ZOOM_ARRIERE) and vueMap.Zoom > 0.3:
                vueMap.Zoom -= self.app.GetFrameTime()
                if vueMap.Zoom < 0.3:
                    vueMap.Zoom = 0.3
            
            self.__scrolling(vueMap, curseurX, curseurY)
            
            t = self.app.GetFrameTime()
            print "FPS: %.2f" % (1.0/(t if t>0 else 1))
            
            # DESSIN :
            self.app.SetView(vueMap)
            self.app.Draw(self.map)
            self.app.SetView(vueInterface)
            self.app.Draw(self.interface)
            self.app.Display()