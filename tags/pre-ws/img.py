# encoding=UTF-8

import os.path
from PySFML import sf
from xml.dom import minidom
from utils import ConstsContainer
from constantes import chemins


class GestionnaireImages(dict):
    """Le Gestionnaire d'Images est un dictionnaire
    Il ne contient que des images (si elles ont été chargées) et leurs informations
    Il ne contient aucun sprite ni aucun autre élément affichable
    Son rôle est de centraliser les chargements d'images de manière à éviter
    par exemple de dupliquer une même image en mémoire"""
    
    def __init__(self, gestInfos):
        # Ces dictionnaires sont remplis par lecture des fichiers XML du dossier "struct"
        dict.__init__(self, { "tuiles":{}, "elements":{}, "persos":{}, "interface":{} })
        self.gestInfos = gestInfos
    
    
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
                infosImg = self.gestInfos[typeObj][num]
            except:
                raise Exception, "L'image numéro %d demandée n'a pas été trouvée dans le document XML des %s" % (num, typeObj)
            img = sf.Image()
            if not img.LoadFromFile(os.path.join(eval("chemins.IMGS_%s" % typeObj.upper()), infosImg["fichier"])):
                chargementOK = False
            self[typeObj][num] = img
        
        if chargementOK == False:
            raise Exception, "Une ou plusieurs images sont manquantes"
    
    
    def chargerPersos(self, persos):
        # A FAIRE
        pass