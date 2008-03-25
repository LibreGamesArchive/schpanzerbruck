# encoding=UTF-8

from PySFML import sf
from xml.dom import minidom
from constantes import tailles
from utils import ConstsContainer
import copy


statut = ConstsContainer()
statut.PAS_DE_SELECTION = 0
statut.INFOS_SEULEMENT = 1
statut.DEPLACEMENT = 2
statut.CIBLAGE = 3
statut.NOIRCIR = 4


class ResultatClicMap:
    """Contient des infos sur le résultat d'un clic sur la Map"""
    
    def __init__(self):
        pass


class Map(sf.Drawable):
    
    def __init__(self, map, gestImages, perspective):
        """Parse un fichier XML de carte pour initialiser l'objet Map"""
        
        sf.Drawable.__init__(self)
        
        self.__donnees = { "tuiles":[], "gdsElements":[], "ptsElements":[], "persos":[], "infos":[] }
        # Chaque case des listes du dico contient un sf.Sprite
        # sauf la case "infos" qui contient une liste dont chaque case contient un conteneur de Constantes qui possède les attributs "tuile", "gdElement" et "ptElement"
        
        self.PERSPECTIVE = perspective
        self.DECALAGE_PTS_ELEMENTS_Y = int(tailles.HAUTEUR_TUILES*(3.0/4))
        self.DECALAGE_GDS_ELEMENTS_Y = tailles.HAUTEUR_TUILES/2
        self.DECALAGE_PERSOS_Y = int(tailles.HAUTEUR_TUILES*(5.0/8))
        if perspective >= 0:
            self.DECALAGE_PTS_ELEMENTS_X = int(perspective*(1.0/4))   # Th. de Thalès
            self.DECALAGE_GDS_ELEMENTS_X = perspective/2   # Th. de Thalès
            self.DECALAGE_PERSOS_X = int(perspective*(3.0/8))    # Th. de Thalès
        else:
            self.DECALAGE_PTS_ELEMENTS_X = -int(perspective*(3.0/4))
            self.DECALAGE_GDS_ELEMENTS_X = -perspective/2
            self.DECALAGE_PERSOS_X = -int(perspective*(5.0/8))
        
        tabsNums = self.__parserMap(map)
        
        gestImages.chargerImagesMap(perspective, **tabsNums) # On "dépaquète" le dictionnaire pour le changer en une liste d'arguments
        # C'est comme si on faisait gestImages.chargerImagesMap(perspective, tuiles=listeNumsTuiles, gdsElements=listeNumsGdsElements, ptsElements=listeNumsPtsElements)
        
        self.__creerSpritesEtInfosSurMap(tabsNums, gestImages) # On remplit le dico self
        
        self.__statut = statut.INFOS_SEULEMENT    # Si True, les cases de la map ne sont pas sélectionnables, et le fait de les survoler ne change rien à l'affichage
    
    
    def __getitem__(self, item):    # OPERATEUR []
        return self.__donnees[item]
    
    
    def __parserMap(self, map):
        """Renvoie un dico contenant les listes de numéros"""
        
        map_node = minidom.parse(map).documentElement
        self.nom = map_node.attributes["nom"].value
        
        self.largeur = int(map_node.attributes["largeur"].value)
        self.hauteur = int(map_node.attributes["hauteur"].value)
        
        largeurPix = self.hauteur * abs(self.PERSPECTIVE) + self.largeur * tailles.LARGEUR_TUILES
        hauteurPix = (self.hauteur+1) * tailles.HAUTEUR_TUILES
        self.rect = sf.FloatRect(0, 0, largeurPix, hauteurPix)
        
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
                raise Exception, "Hauteur et/ou largeur ne correspondent pas à la liste de nums pour les %s dans la map %s" % (i, map)
        
        return tabsNums
    
    
    def __creerSpritesEtInfosSurMap(self, tabsNums, gestImages):
        """Comme son nom l'indique...
        Ne s'occupe pas des personnages"""
        
        # Dictionnaire temporaire pour récupérer et traiter les infos nécessaires sur chaque image:
        infosImages = {"tuiles":[], "gdsElements":[], "ptsElements":[]}
        
        # Constructions des listes de sprites :
        for typeObj in ["tuiles", "gdsElements", "ptsElements"]:
            ligne, colonne = 0, 0
            recopier, remonterDe = 0, 1    # recopier est un int, qui indique si l'élément de la case précédente doit chevaucher encore d'autres cases et si oui, combien (ex. recopier == 2 si l'élément de la case précédent s'étale encore sur 2 cases)
            # remonterDe sert pour les infos : si un élément s'étend sur 3 cases, et que la case concernée est la dernière de ces 3 cases, alors remonterDe == 2
            for ind, num in enumerate(tabsNums[typeObj]):
                sprite, infos = None, None
                
                if recopier >= 1: # L'élément s'étend sur plusieurs cases, on recopie les infos de la case précédente
                    infos = remonterDe
                    recopier -= 1
                    remonterDe += 1
                
                elif num >= 0x01 and (typeObj=="tuiles" or (typeObj != "tuiles" and self["tuiles"][ind] != None)):   # Si il n'y a pas de tuile à cet endroit-là (0x00), on ne met pas non plus d'élément(s)
                    imgETinfos = gestImages[typeObj][num]
                    sprite = sf.Sprite(imgETinfos.image)
                    infos = imgETinfos.infos
                    etalement = infos["etalement"]
                    
                    # A FAIRE : VERIFICATION : VOIR SI L'ELEMENT NE DEBORDE PAS DE LA MAP
                    
                    # POSITIONNEMENT DE CHAQUE SPRITE : (Par rapport au point en haut à gauche de la map)
                    if self.PERSPECTIVE >= 0:
                        tuileX = colonne * tailles.LARGEUR_TUILES + (self.hauteur-1 - ligne) * self.PERSPECTIVE
                    else:
                        tuileX = colonne * tailles.LARGEUR_TUILES + ligne * abs(self.PERSPECTIVE)
                    tuileY = (ligne+1) * tailles.HAUTEUR_TUILES   # "ligne+1" pour décaler la map vers le bas, afin que les éléments sur les premières cases soient quand même visibles en entier
                    if typeObj == "tuiles":
                        sprite.SetPosition(tuileX, tuileY)
                    else:
                        spriteWidth, spriteHeight = sprite.GetSize()
                        if typeObj == "gdsElements":
                            elemX = tuileX + self.DECALAGE_GDS_ELEMENTS_X + (tailles.LARGEUR_TUILES * etalement)/2 - spriteWidth/2
                            elemY = tuileY + self.DECALAGE_GDS_ELEMENTS_Y - spriteHeight
                        else:
                            elemX = tuileX + self.DECALAGE_PTS_ELEMENTS_X + (tailles.LARGEUR_TUILES * etalement)/2 - spriteWidth/2
                            elemY = tuileY + self.DECALAGE_PTS_ELEMENTS_Y - spriteHeight
                        sprite.SetPosition(elemX, elemY)
                    
                    if etalement >= 2:
                        remonterDe = 1
                        recopier = etalement - 1
                
                self[typeObj].append(sprite)
                infosImages[typeObj].append(infos)
                
                colonne += 1
                if colonne == self.largeur:
                    colonne = 0
                    ligne += 1
        
        for i in range(0, self.hauteur * self.largeur): # Traitement du dictionnaire temporaire infoImages
            grpInfos = ConstsContainer()
            grpInfos.tuile = infosImages["tuiles"][i]
            grpInfos.gdElement = infosImages["gdsElements"][i]
            grpInfos.ptElement = infosImages["ptsElements"][i]
            self["infos"].append(grpInfos)
    
    
    def pathfinding(self, coord, mvt, origine="0", coordsPossibles=[], chemin=[], route=[], mvtRestant=[]):
        """Renvoie 3 listes : les coords accessibles, le chemin à parcourir pour chaque et le mouvement restant au personnage pour chaque
        Arguments:
        - coord: la case actuelle du perso
        - mvt: la capacité de mouvement du perso
        Les autres arguments servent pour la recursivité"""
        
        if not coord in coordsPossibles:
            coordsPossibles.append(coord)
            chemin.append(copy.copy(route))
            mvtRestant.append(mvt)
        else:
            index = coordsPossibles.index(coord)
            if mvtRestant[index] < mvt:
                chemin[index] = copy.copy(route)
                mvtRestant[index] = mvt
        
        terrain = 1   # Cout de la case actuelle (coord) en mouvement POUR SORTIR DE LA CASE
        
        if mvt-terrain >= 0:
            if (coord+1)%self.largeur != 0 and origine != "d" and True:
                route.append("d")
                self.pathfinding(coord+1,mvt-terrain,"g",coordsPossibles,chemin,route,mvtRestant)
                route.pop()
            
            if coord%self.largeur != 0 and origine != "g" and True:
                route.append("g")
                self.pathfinding(coord-1,mvt-terrain,"d",coordsPossibles,chemin,route,mvtRestant)
                route.pop()
            
            if coord-self.largeur >= 0 and origine != "h" and True:
                route.append("h")
                self.pathfinding(coord-self.largeur,mvt-terrain,"b",coordsPossibles,chemin,route,mvtRestant)
                route.pop()
            
            if coord+self.largeur < self.largeur*self.hauteur and origine != "b" and True:
                route.append("b")
                self.pathfinding(coord+self.largeur,mvt-terrain,"h",coordsPossibles,chemin,route,mvtRestant)
                route.pop()
        
        return (coordsPossibles, chemin, mvtRestant)
    
    
    def bloquer(self, autoriserInfos = True):
        if autoriserInfos:
            self.__statut = statut.INFOS_SEULEMENT
        else:
            self.__statut = statut.PAS_DE_SELECTION
    
    def noircir(self):
        self.__statut = statut.NOIRCIR
    
    def phaseDeplacement(self):
        self.__statut = statut.DEPLACEMENT
    
    def phaseCiblage(self):
        self.__statut = statut.CIBLAGE
    
    
    def Render(self, renderWindow):
        for tuile in self["tuiles"]:
            if tuile != None:
                renderWindow.Draw(tuile)
                pass
        
        for i in range(0, self.hauteur):
            for x in [0, 1]: # DESSIN DES ELEMENTS DES CASES PAIRES, PUIS IMPAIRES
                while x < self.largeur:
                    for typeObj in ["gdsElements", "ptsElements"]:
                        elemCourant = self[typeObj][i*self.largeur + x]
                        if elemCourant != None:
                            renderWindow.Draw(elemCourant)
                            pass
                    x += 2
    
    
    def gererClic(self, evt, vue):
        # A FAIRE
        
        res = ResultatClicMap()
        return res