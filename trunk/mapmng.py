# encoding=UTF-8

from PySFML import sf
from xml.dom import minidom
from constantes import tailles
from utils import ConstsContainer
import copy


class Map(dict):
    
    def __init__(self, map, gestImages):
        """Parse un fichier XML de carte pour initialiser l'objet Map"""
        
        dict.__init__(self, { "tuiles":[], "gdsElements":[], "ptsElements":[], "persos":[], "infos":[] })
        # Chaque case des listes du dico contient un sf.Sprite
        # sauf la case "infos" qui contient une liste dont chaque case contient un conteneur de Constantes qui possède les attributs "tuile", "gdElement" et "ptElement"
        
        self.gestImages = gestImages # SECURITE : Tant qu'une référence vers le gestionnaire existe quelque part, les images restent en mémoire
        
        tabsNums = self.__parserMap(map)
        
        gestImages.chargerImagesMap(**tabsNums) # On "dépaquète" le dictionnaire pour le changer en une liste d'arguments
        # C'est comme si on faisait gestImages.chargerImagesMap(tuiles=listeNumsTuiles, gdsElements=listeNumsGdsElements, ptsElements=listeNumsPtsElements)
        
        self.__creerSpritesEtInfosSurMap(tabsNums, gestImages) # On remplit le dico self
    
    
    def __parserMap(self, map):
        """Renvoie un dico contenant les listes de numéros"""
        
        map_node = minidom.parse(map).documentElement
        self.nom = map_node.attributes["nom"].value
        self.largeur = int(map_node.attributes["largeur"].value)
        self.hauteur = int(map_node.attributes["hauteur"].value)
        map_strs = {"tuiles":"", "gdsElements":"", "ptsElements":""}
        tabsNums = {"tuiles":[], "gdsElements":[], "ptsElements":[]}
        
        for node in map_node.childNodes:
            if isinstance(node, minidom.Element): # On saute les Text Nodes dus au sauts de lignes
                for i in ["tuiles", "gdsElements", "ptsElements"]:
                    if node.tagName == i:
                        map_strs[i] = node.firstChild.data.strip()
                        break
        
        # Supprime le dernier "|" des chaines:
        for i in ["tuiles", "gdsElements", "ptsElements"]:
            if map_strs[i][-1] == "|":
                map_strs[i] = map_strs[i][0:-1]
            
            tabsNums[i] = [int(x.strip(), 16) for x in map_strs[i].split("|")]
            if len(tabsNums[i]) != self.hauteur * self.largeur:
                raise Exception, "Hauteur/Largeur ne correspondent pas à la liste de nums pour les %s dans %s" % (i, map)
        
        return tabsNums
    
    
    def __creerSpritesEtInfosSurMap(self, tabsNums, gestImages):
        """Comme son nom l'indique...
        Ne s'occupe pas des personnages"""
        
        # Dictionnaire temporaire pour récupérer et traiter les infos nécéssaires sur chaque image:
        infosImages = {"tuiles":[], "gdsElements":[], "ptsElements":[]}
        
        # Constructions des listes de sprites :
        for typeObj in ["tuiles", "gdsElements", "ptsElements"]:
            ligne, colonne = 0, 0
            for num in tabsNums[typeObj]:
                
                if num >= 0x01:
                    imgETinfos = gestImages[typeObj][num]
                    sprite = sf.Sprite(imgETinfos.image)
                    infos = imgETinfos.infos
                    # POSITIONNEMENT DE CHAQUE SPRITE : (Par rapport au point en haut à gauche de la map)
                    tuileX = colonne*tailles.LARGEUR_TUILES
                    tuileY = ligne*tailles.HAUTEUR_TUILES
                    if typeObj == "tuiles":
                        sprite.SetPosition(tuileX, tuileY)
                    else:
                        if typeObj == "gdsElements":
                            elemY = tuileY - sprite.GetHeight() + tailles.HAUTEUR_TUILES/2
                        else:
                            elemY = tuileY - sprite.GetHeight() + int(tailles.HAUTEUR_TUILES*(3.0/4))
                        elemX = (tailles.LARGEUR_TUILES*infos["etalement"])/2 + tuileX - sprite.GetWidth()/2
                        sprite.SetPosition(elemX, elemY)
                
                elif num == 0x00: # Pas de ce type d'objet pour cette case
                    sprite = None
                    infos = None
                
                elif num == -0x01: # L'élément s'étend sur plusieurs cases, on recopie les infos de la case précédente
                    sprite = None
                    infos = "=precedent"
                
                self[typeObj].append(sprite)
                infosImages[typeObj].append(infos)
                
                colonne = colonne + 1
                if colonne == self.largeur:
                    colonne = 0
                    ligne = ligne + 1
        
        for i in range(0, self.hauteur*self.largeur): # Traitement du dictionnaire temporaire infoImages
            grpInfos = ConstsContainer()
            grpInfos.tuile = infosImages["tuiles"][i]
            grpInfos.gdElement = infosImages["gdsElements"][i]
            grpInfos.ptElement = infosImages["ptsElements"][i]
            self["infos"].append(grpInfos)
    
    
    def pathfinding(coord, mvt, origine="0", coordsPossibles=[], chemin=[], route=[]):
        """Renvoie un tuple de 2 listes : les positions accessibles et les chemins correspondants a parcourir
        Arguments: coord: la case actuelle du perso
        mvt: le nombre de cases dont peut se deplacer le perso
        Les autres arguments servent pour la recursivité"""
        
        if not (coord in coordsPossibles):
            coordsPossibles.append(coord)
            chemin.append(copy.copy(route))
        
        terrain = 1 # Cout de la case actuelle (coord) en mouvement POUR SORTIR DE LA CASE
        
        if mvt-terrain >= 0:
            if True and origine != "D":
                route.append("D")
                self.pathfinding(coord+1,mvt-terrain,"G",coordsPossibles,chemin,route)
                route.pop()
            
            if True and origine != "G":
                route.append("G")
                self.pathfinding(coord-1,mvt-terrain,"D",coordsPossibles,chemin,route)
                route.pop()
            
            if True and origine != "H":
                route.append("H")
                self.pathfinding(coord-self.largeur,mvt-terrain,"B",coordsPossibles,chemin,route)
                route.pop()
            
            if True and origine != "B":
                route.append("B")
                self.pathfinding(coord+self.largeur,mvt-terrain,"H",coordsPossibles,chemin,route)
                route.pop()
        
        return coordsPossibles, chemin
    
    
    def dessinerSur(self, renderWindow):
        """On dessine la Map en (0,0) sur la fenêtre"""
        
        for tuile in self["tuiles"]:
            if tuile != None:
                renderWindow.Draw(tuile)
        
        for x in [0, 1]: # DESSIN DES ELEMENTS DES CASES PAIRES, PUIS IMPAIRES
            while x < self.hauteur * self.largeur:
                for typeObj in ["gdsElements", "ptsElements"]:
                    if self[typeObj][x] != None:
                        renderWindow.Draw(self[typeObj][x])
                x += 2