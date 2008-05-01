# encoding=UTF-8

from PySFML import sf
import os.path
from xml.dom import minidom
from constantes import chemins
from utils import ConstsContainer


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
        # Chaque case de ces dictionnaires (tuiles, elements, etc.) est un ConstsContainer de 2 choses:
        #   - infos : un objet de type InfosImage
        #   - image : l'objet sf.Image lui-même
        # Ces dictionnaires sont remplis par lecture des fichiers XML du dossier "struct"
        dict.__init__(self, { "tuiles":{}, "elements":{}, "persos":{}, "interface":{} })
        # Parsage des documents tuiles.xml et elements.xml et, et chargement des images correspondantes :
        self.__objetsDispos = {"tuiles":{}, "elements":{}, "persos":{}, "interface":{}}
        for typeObj in ["tuiles", "elements"]:
            # self.__objetsDispos est un dictionnaires d'objets InfosImage
            objets_nodes = minidom.parse(os.path.join(chemins.OBJETS_MAP, "%s.xml" % typeObj)).documentElement.getElementsByTagName("obj")
            for obj in objets_nodes:
                attrs = obj.attributes
                self.__objetsDispos[typeObj][int(attrs["num"].value, 16)] = InfosImage(attrs)
    
    def chargerImagesMap(self, tabsNums):
        """Charge les tuiles et les éléments"""
        for typeObj in ["tuiles", "elements"]:
            for num in tabsNums[typeObj]:
                self.chargerImage(typeObj, num)
    
    def chargerImage(self, typeObj, num):
        """Charge une image"""
        chargementOK = True
        if num == 0x00: # Les numéros des images sont en HEXADECIMAL
            pass    # Zéro (0x00) signifie qu'il n'y a pas d'image de ce type pour cette case
        elif num not in self[typeObj].keys(): # Si l'image n'a pas déjà été chargée
            try:
                infosImg = self.__objetsDispos[typeObj][num]
            except:
                raise Exception, "L'image numéro %d demandée n'a pas été trouvée dans le document XML des %s" % (num, typeObj)
            img = sf.Image()
            if not img.LoadFromFile(os.path.join(eval("chemins.IMGS_%s" % typeObj.upper()), infosImg["fichier"])):
                chargementOK = False
            self[typeObj][num] = ConstsContainer()
            self[typeObj][num].infos = infosImg
            self[typeObj][num].image = img
        
        if chargementOK == False:
            raise Exception, "Une ou plusieurs images sont manquantes"
    
    def chargerPersos(self, persos):
        # A FAIRE
        pass