# encoding=UTF-8

from PySFML import sf
import os.path
from xml.dom import minidom
from constantes import chemins, tailles
from utils import ConstsContainer

def tord(image, decalage, antialiasing=False, background=sf.Color(0,0,0,0)):
    """Tord une image pour lui donner un effet de perspective."""
    nHauteur=image.GetHeight()
    nLargeur=image.GetWidth()+abs(decalage)
    nImage=sf.Image(Width=nLargeur, Height=nHauteur, Color=background)
    nImage.SetSmooth(False)
    for y in range(image.GetHeight()):
        decalageLigne=decalage-(y*decalage)/nHauteur
        for x in range(image.GetWidth()):
            pix=image.GetPixel(x, y)
            if antialiasing:
                if x==0 or x==image.GetWidth():
                    pix=sf.Color(pix.r, pix.g, pix.b, 200)
                elif x==1 or x==image.GetWidth()-1:
                    pix=sf.Color(pix.r, pix.g, pix.b, 100)
            nx=x+decalageLigne
            if nx>=0:
                nImage.SetPixel(nx, y, pix)
    return nImage

class InfosImage(dict):
    """Contient tous les renseignement nécessaires sur une image
    (chemin du fichier, niveau de passabilité, etc.)"""
    
    def __init__(self, attrs_xml):
        dict.__init__(self)
        
        def assigner(*attrs):
            for type, attr, defaut in attrs:
                try:
                    self[attr] = type(attrs_xml[attr].value)
                except:
                    self[attr] = defaut
        
        assigner((str, "fichier", ""), (int, "nivpass", 0), (int, "etalement", 1), (str, "type", "aucun"), (int, "vie", -1))


class GestionnaireImages(dict):
    """Le Gestionnaire d'Images est un dictionnaire
    Il ne contient que des images (si elles ont été chargées) et leurs informations
    Il ne contient aucun sprite ni aucun autre élément affichable
    Son rôle est de centraliser les chargements d'images de manière à éviter
    par exemple de dupliquer une même image en mémoire"""
    
    def __init__(self):
        # Chaque case de ces dictionnaires (tuiles, gdsElements, etc.) est un ConstsContainer de 2 choses:
        #   - infos : un objet de type InfosImage
        #   - image : l'objet sf.Image lui-même
        # Ces dictionnaires sont remplis par lecture des fichiers XML du dossier "objets"
        dict.__init__(self, { "tuiles":{}, "gdsElements":{}, "ptsElements":{}, "persos":{}, "interface":{} })
    
    
    def chargerImagesMap(self, **tabsNums):
        """Charge les tuiles, gdsElements et ptsElements:
        
        gestionnaire.chargerImagesMap(tuiles=listeNumsTuiles, gdsElements=listeNumsGdsElements, ptsElements=listeNumsPtsElements)"""
        
        chargementOK = True
        
        # Parsage des documents tuiles.xml, gdsElements.xml et ptsElements.xml, et chargement des images correspondantes :
        for typeObj in ["tuiles", "gdsElements", "ptsElements"]:
            objsDispos = {}
            # objsDispos est un dictionnaires d'objet InfosImage
            objs_nodes = minidom.parse(os.path.join(chemins.OBJETS, "%s.xml" % typeObj)).documentElement.getElementsByTagName("obj")
            for obj in objs_nodes:
                attrs = obj.attributes
                objsDispos[int(attrs["num"].value, 16)] = InfosImage(attrs)
                
            for num in tabsNums[typeObj]:
                if num == 0x00: # Les numéros des images sont en HEXADECIMAL
                    continue    # Zéro (0x00) signifie qu'il n'y a pas d'image de ce type pour cette case
                if not self[typeObj].has_key(num): # Si l'image n'a pas déjà été chargée
                    try:
                        infosImg = objsDispos[num]
                    except:
                        raise Exception, "L'image numéro %d demandée par la map n'a pas été trouvée dans le document XML des %s" % (num, typeObj)
                    img = sf.Image()
                    if not img.LoadFromFile(os.path.join(eval("chemins.IMGS_%s" % typeObj.upper()), infosImg["fichier"])):
                        chargementOK = False
                    if typeObj == "tuiles":
                        img=tord(img, tailles.DECALAGE_TUILES)
                    self[typeObj][num] = ConstsContainer()
                    self[typeObj][num].infos = infosImg
                    self[typeObj][num].image = img
        
        if chargementOK == False:
            raise Exception, "Une ou plusieurs images sont manquantes"
    
    
    def chargerPersos(self, persos):
        # A FAIRE
        pass
    
    
    def chargerInterface(self, imagesInterface):
        # A FAIRE
        pass