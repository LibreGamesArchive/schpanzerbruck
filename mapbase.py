# encoding=UTF-8

import os.path
from xml.dom import minidom
from constantes import chemins
import infos


class MapBase(dict):
    """Utilisée conjointement avec la MapGraphique côté client, elle sert de classe de base pour la MapSrv.
    Dans tous les cas, elle est faite pour marcher avec un GestionnaireInfos"""
    
    def __init__(self, fichierMap):
        dict.__init__(self, {"tuiles":[], "elements":[]})
        self.gestInfos = infos.GestionnaireInfos()
        self.__parserMap(fichierMap)
    
    
    def estValide(self, numCase):
        if numCase >= 0 and numCase < self.largeur*self.hauteur:
            return True
        return False
    
    def estSurBordHaut(self, numCase):
        if numCase >= 0 and numCase < self.largeur:
            return True
        return False
    
    def estSurBordGauche(self, numCase):
        if estValide(numCase) and numCase % self.largeur == 0:
            return True
        return False
    
    def estSurBordBas(self, numCase):
        if numCase >= self.largeur*(self.hauteur-1) and numCase < self.largeur*self.hauteur:
            return True
        return False
    
    def estSurBordDroit(self, numCase):
        if estValide(numCase) and (numCase+1) % self.largeur == 0:
            return True
        return False
    
    
    def __parserMap(self, fichierMap):
        map_node = minidom.parse(fichierMap).documentElement
        self.nom = map_node.attributes["nom"].value
        
        self.largeur = int(map_node.attributes["largeur"].value)
        self.hauteur = int(map_node.attributes["hauteur"].value)
        if map_node.hasAttribute("bordure"):
            self.bordure = int(map_node.attributes["bordure"].value, 16)
        else:
            self.bordure=0x00
        map_strs = {"tuiles":"", "elements":""}
        
        for node in map_node.childNodes:
            if isinstance(node, minidom.Element): # On saute les Text Nodes dus au sauts de lignes
                for i in ["tuiles", "elements"]:
                    if node.tagName == i:
                        map_strs[i] = node.firstChild.data.strip()
                        break
        
        # Supprime le dernier "|" des chaines:
        for i in ["tuiles", "elements"]:
            if map_strs[i][-1] == "|":
                map_strs[i] = map_strs[i][0:-1]
            self[i] = [int(x.strip(), 16) for x in map_strs[i].split("|")]
            if len(self[i]) != self.hauteur * self.largeur:
                raise Exception, "Hauteur et/ou largeur ne correspondent pas à la liste de nums pour les %s dans la map %s" % (i, map)
    
    
    def listesFichiersImages(self):
        """Renvoie la liste des fichiers images à charger"""
        
        LFI = {"tuiles":[], "elements":[], "bordure":""}
        for typeObj in ["tuiles", "elements"]:
            for numObj in self[typeObj]:
                if numObj == 0:
                    cheminCourant = ""
                else:
                    cheminCourant = os.path.join( chemins.__dict__["IMGS_%s" % typeObj.upper()], self.gestInfos[typeObj][numObj]["fichier"] )
                LFI[typeObj].append(cheminCourant)
        if self.bordure > 0:
            LFI["bordure"] = self.gestInfos["tuiles"][self.bordure]["fichier"]
        
        return LFI
