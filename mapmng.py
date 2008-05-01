# encoding=UTF-8

from PySFML import sf
from OpenGL.GL import *
from xml.dom import minidom
from constantes import tailles, defaut
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


class ObjetMap(object):
    """Classe de base pour les objets de la map"""
    
    def __init__(self, infos):
        object.__init__(self)
        self.__infos = infos    # La référence vers les infos de base de l'ObjetMap. READ ONLY !
    
    def __getattr__(self, attr):
        if attr in self.__dict__.keys():
            return self.__dict__[attr]
        return self.__infos[attr]

class Tuile(ObjetMap):
    """Représente une tuile"""
    
    def __init__(self, infos, texture=None):
        ObjetMap.__init__(self, infos)
        self.texture = texture      # Référence vers la sfImage faisant office de texture
        # Est égal à None dans un contexte non graphique (côté serveur)
    
    def GL_Dessin(self):
        self.texture.Bind()
        glBegin(GL_QUADS)
        glTexCoord2f(0, 0); glVertex3f(0, 0, 0)
        glTexCoord2f(0, 1); glVertex3f(1, 0, 0)
        glTexCoord2f(1, 1); glVertex3f(1, 1, 0)
        glTexCoord2f(1, 0); glVertex3f(0, 1, 0)
        glEnd()

class Element(ObjetMap):
    """Représente un élément (Arbre, maison, etc.)"""
    
    def __init__(self, infos, textures=[]):
        ObjetMap.__init__(self, infos)
        self.textures = textures    # Liste des références vers les textures nécessaires
        # Vide dans le cas d'un contexte non graphique (côté serveur)
        self.vie = infos["vie"]     # La vie d'un élément est modifiable, elle doit donc être extraite
    
    def GL_Dessin(self):
        self.textures[0].Bind()
        glBegin(GL_QUADS)
        glTexCoord2f(0, 0); glVertex3f(0.5, 0, 1.15)
        glTexCoord2f(0, 1); glVertex3f(0.5, 0, 0)
        glTexCoord2f(1, 1); glVertex3f(0.5, 1, 0)
        glTexCoord2f(1, 0); glVertex3f(0.5, 1, 1.15)
        glEnd()


class BaseMap:
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
            int(map_node.attributes["hauteur"].value)
            tabsNums[i] = [int(x.strip(), 16) for x in map_strs[i].split("|")]
            if len(tabsNums[i]) != self.hauteur * self.largeur:
                raise Exception, "Hauteur et/ou largeur ne correspondent pas à la liste de nums pour les %s dans la map %s" % (i, map)
        
        return tabsNums


class MapSrv(BaseMap):
    """La map ne contenant aucune texture, destinée à être utilisée seulement par le serveur"""
    
    def __init__(self, map):
        BaseMap.__init__(self, map)
    
    
    def zoneDeplacement(self, coord, mvt, origine="0", coordsPossibles=[], chemin=[], route=[], mvtRestant=[]):
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
                self.zoneDeplacement(coord+1,mvt-terrain,"g",coordsPossibles,chemin,route,mvtRestant)
                route.pop()
            
            if coord%self.largeur != 0 and origine != "g" and True:
                route.append("g")
                self.zoneDeplacement(coord-1,mvt-terrain,"d",coordsPossibles,chemin,route,mvtRestant)
                route.pop()
            
            if coord-self.largeur >= 0 and origine != "h" and True:
                route.append("h")
                self.zoneDeplacement(coord-self.largeur,mvt-terrain,"b",coordsPossibles,chemin,route,mvtRestant)
                route.pop()
            
            if coord+self.largeur < self.largeur*self.hauteur and origine != "b" and True:
                route.append("b")
                self.zoneDeplacement(coord+self.largeur,mvt-terrain,"h",coordsPossibles,chemin,route,mvtRestant)
                route.pop()
        
        return (coordsPossibles, chemin, mvtRestant)


class Map(BaseMap):
    """La Map côté client, dessinable"""
    
    def __init__(self, map, gestImages):
        BaseMap.__init__(self, map)
        
        self.LIMITE_ELEMENTS = int(tailles.HAUTEUR_TUILES*(4.0/5))  # Sur une case, l'avant de l'élément ne peut pas dépasser cette limite
        self.POSITION_PERSOS = int(tailles.HAUTEUR_TUILES*(9.0/10))   # Position des persos statiques sur leur case
        
        tabsNums = self.parserMap(map)
        
        gestImages.chargerImagesMap(tabsNums)
        gestImages.chargerImage("tuiles", self.bordure)

        if self.bordure==0x00:
            self.imageBordure=sf.Image(tailles.LARGEUR_TUILES, tailles.HAUTEUR_TUILES, defaut.COULEUR_BORDURE)
        else:
            self.imageBordure=gestImages["tuiles"][self.bordure].image
        
        self.coordsCases = [] # Coordonnées du point en haut à gauche de chaque case dans le plan (0xy)
        for x in range(0, self.hauteur):
            for y in range(0, self.largeur):
                self.coordsCases.append((x, y, 0))
        
        self.__recupTexturesEtInfosSurMap(tabsNums, gestImages) # On remplit le dico self
        
        self.__statut = statut.INFOS_SEULEMENT    # Si True, les cases de la map ne sont pas sélectionnables, et le fait de les survoler ne change rien à l'affichage
    
    
    def __recupTexturesEtInfosSurMap(self, tabsNums, gestImages):
        """Comme son nom l'indique...
        Ne s'occupe pas des personnages"""
        
        # Constructions des listes de textures :
        for typeObj in ["tuiles", "elements"]:
            recopier, remonterDe = 0, 1    # recopier est un int, qui indique si l'élément de la case précédente doit chevaucher encore d'autres cases et si oui, combien (ex. recopier == 2 si l'élément de la case précédent s'étale encore sur 2 cases)
            # remonterDe sert pour les infos : si un élément s'étend sur 3 cases, et que la case concernée est la dernière de ces 3 cases, alors remonterDe == 2
            for ind, num in enumerate(tabsNums[typeObj]):
                texture, infos = None, None
                nouvelObj = None
                
                if recopier >= 1: # L'élément s'étend sur plusieurs cases, on recopie les infos de la case précédente
                    nouvelObj = remonterDe
                    recopier -= 1
                    remonterDe += 1
                
                elif num >= 0x01 and (typeObj=="tuiles" or (typeObj != "tuiles" and self.objets["tuiles"][ind] != None)):   # Si il n'y a pas de tuile à cet endroit-là (0x00), on ne met pas non plus d'élément(s)
                    imgETinfos = gestImages[typeObj][num]
                    texture = imgETinfos.image
                    infos = imgETinfos.infos
                    etalement = infos["etalement"]
                    
                    # A FAIRE : VERIFICATION : VOIR SI L'ELEMENT NE DEBORDE PAS DE LA MAP
                    
                    if etalement >= 2:
                        remonterDe = 1
                        recopier = etalement - 1
                    
                    if typeObj == "tuiles":
                        nouvelObj = Tuile(infos, texture)
                    else:
                        nouvelObj = Element(infos, [texture])
                
                self.objets[typeObj].append(nouvelObj)
    
    
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
    
    
    def GL_Dessin(self):
        """Dessine la Map dans le plan (0xy)"""
        glMatrixMode(GL_MODELVIEW)
        glEnable(GL_TEXTURE_2D)
        
        if self.__statut == statut.NOIRCIR:
            glColor3ub(50, 50, 50)
        else:
            glColor3ub(255, 255, 255)
        
        for numCase, coordsCase in enumerate(self.coordsCases):
            glPushMatrix()
            glTranslatef(*coordsCase)
            
            # DESSIN DE LA TUILE ET DE L'ELEMENT:
            for typeObj in ["tuiles", "elements"]:
                if isinstance(self.objets[typeObj][numCase], ObjetMap):     # Si il y a bien un ObjetMap à cette case
                    self.objets[typeObj][numCase].GL_Dessin()
            
            glPopMatrix()
        
        
        
        # Dessin du plateau:
        self.imageBordure.Bind()
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT)
        glBegin(GL_QUADS)
        glTexCoord2d(0, 0); glVertex3f(0, 0, 0)
        glTexCoord2d(0, defaut.HAUTEUR_BORDURE); glVertex3f(0, 0, -defaut.HAUTEUR_BORDURE)
        glTexCoord2d(self.hauteur, defaut.HAUTEUR_BORDURE); glVertex3f(self.hauteur, 0, -defaut.HAUTEUR_BORDURE)
        glTexCoord2d(self.hauteur, 0); glVertex3f(self.hauteur, 0, 0)
        
        glTexCoord2d(0, 0); glVertex3f(0, 0, 0)
        glTexCoord2d(0, defaut.HAUTEUR_BORDURE); glVertex3f(0, 0, -defaut.HAUTEUR_BORDURE)
        glTexCoord2d(self.largeur, defaut.HAUTEUR_BORDURE); glVertex3f(0, self.largeur, -defaut.HAUTEUR_BORDURE)
        glTexCoord2d(self.largeur, 0); glVertex3f(0, self.largeur, 0)
        
        glTexCoord2d(0, 0); glVertex3f(self.hauteur, self.largeur, 0)
        glTexCoord2d(0, defaut.HAUTEUR_BORDURE); glVertex3f(self.hauteur, self.largeur, -defaut.HAUTEUR_BORDURE)
        glTexCoord2d(self.hauteur, defaut.HAUTEUR_BORDURE); glVertex3f(0, self.largeur, -defaut.HAUTEUR_BORDURE)
        glTexCoord2d(self.hauteur, 0); glVertex3f(0, self.largeur, 0)
        
        glTexCoord2d(0, 0); glVertex3f(self.hauteur, 0, 0)
        glTexCoord2d(0, defaut.HAUTEUR_BORDURE); glVertex3f(self.hauteur, 0, -defaut.HAUTEUR_BORDURE)
        glTexCoord2d(self.largeur, defaut.HAUTEUR_BORDURE); glVertex3f(self.hauteur, self.largeur, -defaut.HAUTEUR_BORDURE)
        glTexCoord2d(self.largeur, 0); glVertex3f(self.hauteur, self.largeur, 0)
        
        glColor3ub(70, 70, 70)
        glVertex3f(self.hauteur, 0, -defaut.HAUTEUR_BORDURE); glVertex3f(0, 0, -defaut.HAUTEUR_BORDURE); glVertex3f(0, self.largeur, -defaut.HAUTEUR_BORDURE); glVertex3f(self.hauteur, self.largeur, -defaut.HAUTEUR_BORDURE)
        
        
        glEnd()
        
        glDisable(GL_TEXTURE_2D)
    
    
    def gererClic(self, evt):
        # A FAIRE
        
        res = ResultatClicMap()
        return res