# encoding=UTF-8

from OpenGL.GL import *
from OpenGL.GLU import gluPerspective, gluPickMatrix, gluLookAt
from mapbase import MapBase
import mapfx
from constantes import defaut
from utils import ConstsContainer


statut = ConstsContainer()
statut.PAS_DE_SELECTION = 0
statut.NOIRCIR = 1
statut.INFOS_SEULEMENT = 2
statut.DEPLACEMENT = 3
statut.CIBLAGE = 4


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
    
    def GL_DessinSansTexture(self):
        glBegin(GL_QUADS)
        glVertex3f(0, 0, 0)
        glVertex3f(1, 0, 0)
        glVertex3f(1, 1, 0)
        glVertex3f(0, 1, 0)
        glEnd()

class Element(ObjetMap):
    """Représente un élément (Arbre, maison, etc.)"""
    
    def __init__(self, infos, textures=[]):
        ObjetMap.__init__(self, infos)
        self.textures = textures    # Liste des références vers les textures nécessaires
        # Vide dans le cas d'un contexte non graphique (côté serveur)
        self.vie = infos["vie"]     # La vie d'un élément est modifiable, elle doit donc être extraite
    
    def GL_Dessin(self, inclinaison):
        glTranslatef(0.5, 0, 0)
        glRotatef(inclinaison-90, 0, 1, 0)
        self.textures[0].Bind()
        glBegin(GL_QUADS)
        glTexCoord2f(0, 0); glVertex3f(0, 0, 1.15)
        glTexCoord2f(0, 1); glVertex3f(0, 0, 0)
        glTexCoord2f(1, 1); glVertex3f(0, 1, 0)
        glTexCoord2f(1, 0); glVertex3f(0, 1, 1.15)
        glEnd()
    
    def GL_DessinSansTexture(self, inclinaison):
        glTranslatef(0.5, 0, 0)
        glRotatef(inclinaison-90, 0, 1, 0)
        glBegin(GL_QUADS)
        glVertex3f(0, 0, 1.15)
        glVertex3f(0, 0, 0)
        glVertex3f(0, 1, 0)
        glVertex3f(0, 1, 1.15)
        glEnd()


class Map(MapBase):
    """La Map côté client, dessinable"""
    
    def __init__(self, map, gestImages):
        MapBase.__init__(self, map)
        
        tabsNums = self.parserMap(map)
        
        gestImages.chargerImagesMap(tabsNums)
        gestImages.chargerImage("tuiles", self.bordure)

        if self.bordure==0x00:
            self.imageBordure=defaut.TEXTURE_BORDURE
        else:
            self.imageBordure=gestImages["tuiles"][self.bordure]
        
        self.coordsCases = [] # Coordonnées du point en haut à gauche de chaque case dans le plan (0xy)
        for x in range(0, self.hauteur):
            for y in range(0, self.largeur):
                self.coordsCases.append((x, y, 0))
        
        self.__recupTexturesEtInfosSurMap(tabsNums, gestImages) # On remplit le dico self
        
        self.__statut = statut.INFOS_SEULEMENT
        
        self.inclinaisonElements = 1
        
        self.__FXActives = []      # Liste des effets spéciaux utilisables sur la Map
        
        self.lancerFX(mapfx.DeploiementElements())
        
        self.picked = [-1, -1]    # ObjetMap sélectionné par le picking.
        # self.picked == [-1, -1] : Pas d'objet sélectionné
        # self.picked = [numCase, typeObjet] : Objet sélectionné :
        #       typeObjet: ==0 : tuile; ==1 : élément; ==2 : perso
    
    
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
                    texture = gestImages[typeObj][num]
                    infos = gestImages.gestInfos[typeObj][num]
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
    
    
    def lancerFX(self, nouvFX):
        """nouvFX doit être callable, et prendre en arguments map et getFrameTime (fonction pour obtenir le frameTime)"""
        self.__FXActives.append(nouvFX)
    
    
    def GL_RectUni(self, coordsHG, vectLarg, vectHaut=(0, 0, -defaut.HAUTEUR_BORDURE)):
        glVertex3f(coordsHG[0]+vectLarg[0], coordsHG[1]+vectLarg[1], coordsHG[2]+vectLarg[2])
        glVertex3f(*coordsHG)
        glVertex3f(coordsHG[0]+vectHaut[0], coordsHG[1]+vectHaut[1], coordsHG[2]+vectHaut[2])
        glVertex3f(coordsHG[0]+vectLarg[0]+vectHaut[0], coordsHG[1]+vectLarg[1]+vectHaut[1], coordsHG[2]+vectLarg[2]+vectHaut[2])
    
    
    def GL_DessinPourPicking(self, getFrameTime, appL, appH, camera, curseurX, curseurY, elemsON):
        """GL_Dessin pour le picking"""
        glSelectBuffer(128)
        
        glRenderMode(GL_SELECT)
        glInitNames()
        
        glMatrixMode(GL_PROJECTION)
        glLoadIdentity()
        gluPickMatrix(curseurX, appH - curseurY, 1, 1, glGetIntegerv(GL_VIEWPORT))
        gluPerspective(70, float(appL)/appH, 1, 50)
        
        glMatrixMode(GL_MODELVIEW)
        glLoadIdentity()
        gluLookAt(camera.pos[0], camera.pos[1], camera.pos[2], camera.cible[0], camera.cible[1], camera.cible[2], 0, 0, 1)
        
        glDisable(GL_TEXTURE_2D)
        glDisable(GL_DEPTH_TEST)
        glDisable(GL_BLEND)
        glDisable(GL_ALPHA_TEST)
        
        if self.__statut != statut.CIBLAGE :
            # DESSIN DES TUILES
            for numCase, tuile in enumerate(self.objets["tuiles"]):
                if isinstance(tuile, ObjetMap):
                    glPushMatrix()
                    glTranslatef(*(self.coordsCases[numCase]))
                    
                    glPushName(numCase); glPushName(0)
                    tuile.GL_DessinSansTexture()
                    glPopName(); glPopName()
                    
                    glPopMatrix()
        
        if elemsON and self.__statut == statut.CIBLAGE:
            # DESSIN DES ELEMENTS
            for numCase, element in enumerate(self.objets["elements"]):
                if isinstance(element, ObjetMap):
                    glPushMatrix()
                    glTranslatef(*(self.coordsCases[numCase]))
                    
                    glPushName(numCase); glPushName(1)
                    element.GL_DessinSansTexture(self.inclinaisonElements)
                    glPopName(); glPopName()
                    
                    glPopMatrix()
        
        nameStack = glRenderMode(GL_RENDER)
        if len(nameStack) > 0:
            self.picked = nameStack[-1].names
        else:
            self.picked = [-1, -1]
    
    
    def GL_Dessin(self, getFrameTime, appL, appH, camera, curseurX, curseurY, elemsON=True):
        """Dessine la Map dans le plan (0xy)"""
        
        glMatrixMode(GL_PROJECTION)
        glLoadIdentity()
        gluPerspective(70, float(appL)/appH, 1, 50)
        
        glMatrixMode(GL_MODELVIEW)
        glLoadIdentity()
        gluLookAt(camera.pos[0], camera.pos[1], camera.pos[2], camera.cible[0], camera.cible[1], camera.cible[2], 0, 0, 1)
        
        glEnable(GL_TEXTURE_2D)
        glEnable(GL_DEPTH_TEST)
        glEnable(GL_BLEND)
        glEnable(GL_ALPHA_TEST)
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)
        glAlphaFunc(GL_GREATER, 0)
        
        if self.__statut == statut.NOIRCIR:
            factAssomb = 5
        else:
            factAssomb = 1
            for ind, FX in enumerate(self.__FXActives):
                if FX(self, getFrameTime):   # Si le FX revoie True, il est terminé
                    self.__FXActives.pop(ind)
        
        nvGris = 255/factAssomb
        glColor3ub(nvGris, nvGris, nvGris)
        
        # DESSIN DES TUILES
        for numCase, tuile in enumerate(self.objets["tuiles"]):
            if isinstance(tuile, ObjetMap):
                glPushMatrix()
                glTranslatef(*(self.coordsCases[numCase]))
                
                selec = False
                if self.picked[0] == numCase:
                    if self.picked[1] == 0:
                        selec = True
                
                if selec:
                    glColor3ub(0, nvGris/2, nvGris)
                tuile.GL_Dessin()
                if selec:
                    glColor3ub(nvGris, nvGris, nvGris)
                
                glPopMatrix()
        
        # DESSIN DES ELEMENTS
        for numCase, element in enumerate(self.objets["elements"]):
            if isinstance(element, ObjetMap):
                glPushMatrix()
                glTranslatef(*(self.coordsCases[numCase]))
                
                selec = False
                if self.picked[0] == numCase:
                    if self.picked[1] == 1:
                        selec = True
                
                if selec:
                    glColor3ub(0, nvGris/2, nvGris)
                elif not elemsON:
                    glColor4ub(nvGris, nvGris, nvGris, 80)
                element.GL_Dessin(self.inclinaisonElements)
                if selec or not elemsON:
                    glColor3ub(nvGris, nvGris, nvGris)
                
                glPopMatrix()
        
        
        # Dessin du plateau:
        self.imageBordure.Bind()
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT)
        glBegin(GL_QUADS)
        
        nvGris = 195/factAssomb
        glColor3ub(nvGris, nvGris, nvGris)
        glTexCoord2d(0, 0); glVertex3f(0, 0, 0)
        glTexCoord2d(0, defaut.HAUTEUR_BORDURE); glVertex3f(0, 0, -defaut.HAUTEUR_BORDURE)
        glTexCoord2d(self.hauteur, defaut.HAUTEUR_BORDURE); glVertex3f(self.hauteur, 0, -defaut.HAUTEUR_BORDURE)
        glTexCoord2d(self.hauteur, 0); glVertex3f(self.hauteur, 0, 0)
        
        nvGris = 85/factAssomb
        glColor3ub(nvGris, nvGris, nvGris)
        glTexCoord2d(0, 0); glVertex3f(0, 0, 0)
        glTexCoord2d(0, defaut.HAUTEUR_BORDURE); glVertex3f(0, 0, -defaut.HAUTEUR_BORDURE)
        glTexCoord2d(self.largeur, defaut.HAUTEUR_BORDURE); glVertex3f(0, self.largeur, -defaut.HAUTEUR_BORDURE)
        glTexCoord2d(self.largeur, 0); glVertex3f(0, self.largeur, 0)
        
        nvGris = 135/factAssomb
        glColor3ub(nvGris, nvGris, nvGris)
        glTexCoord2d(0, 0); glVertex3f(self.hauteur, self.largeur, 0)
        glTexCoord2d(0, defaut.HAUTEUR_BORDURE); glVertex3f(self.hauteur, self.largeur, -defaut.HAUTEUR_BORDURE)
        glTexCoord2d(self.hauteur, defaut.HAUTEUR_BORDURE); glVertex3f(0, self.largeur, -defaut.HAUTEUR_BORDURE)
        glTexCoord2d(self.hauteur, 0); glVertex3f(0, self.largeur, 0)
        
        nvGris = 255/factAssomb
        glColor3ub(nvGris, nvGris, nvGris)
        glTexCoord2d(0, 0); glVertex3f(self.hauteur, 0, 0)
        glTexCoord2d(0, defaut.HAUTEUR_BORDURE); glVertex3f(self.hauteur, 0, -defaut.HAUTEUR_BORDURE)
        glTexCoord2d(self.largeur, defaut.HAUTEUR_BORDURE); glVertex3f(self.hauteur, self.largeur, -defaut.HAUTEUR_BORDURE)
        glTexCoord2d(self.largeur, 0); glVertex3f(self.hauteur, self.largeur, 0)
        
        glEnd()
        
        if self.__statut not in (statut.NOIRCIR, statut.PAS_DE_SELECTION):
            self.GL_DessinPourPicking(getFrameTime(), appL, appH, camera, curseurX, curseurY, elemsON)
    
    
    def gererClic(self, evt):
        # A FAIRE
        
        res = ResultatClicMap()
        return res
