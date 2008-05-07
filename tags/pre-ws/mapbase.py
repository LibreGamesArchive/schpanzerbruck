# encoding=UTF-8

from xml.dom import minidom


class MapBase:
    """La classe de base pour une Map"""
    
    def __init__(self, map):
        self.objets = { "tuiles":[], "elements":[] }
        # Chaque case des listes du dico contient un ObjetMap
    
    
    def parserMap(self, map):
        """Renvoie un dico contenant les listes de numéros"""
        
        map_node = minidom.parse(map).documentElement
        self.nom = map_node.attributes["nom"].value
        
        self.largeur = int(map_node.attributes["largeur"].value)
        self.hauteur = int(map_node.attributes["hauteur"].value)
        if map_node.hasAttribute("bordure"):
            self.bordure = int(map_node.attributes["bordure"].value, 16)
        else:
            self.bordure=0x00
        map_strs = {"tuiles":"", "elements":""}
        tabsNums = {"tuiles":[], "elements":[]}
        
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
            tabsNums[i] = [int(x.strip(), 16) for x in map_strs[i].split("|")]
            if len(tabsNums[i]) != self.hauteur * self.largeur:
                raise Exception, "Hauteur et/ou largeur ne correspondent pas à la liste de nums pour les %s dans la map %s" % (i, map)
        
        return tabsNums
    
    
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
