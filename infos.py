# encoding=UTF-8

import os.path
from xml.dom import minidom
from constantes import chemins


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
        
        assigner((str, "fichier", ""), (int, "nivpass", 0), (int, "etalement", 1), (str, "type", "aucun"), (str, "nom", ""), (int, "vie", -1))


class GestionnaireInfos(dict):
    
    def __init__(self):
        dict.__init__(self, { "tuiles":{}, "elements":{} })
        for typeObj in ["tuiles", "elements"]:
            # self est un dictionnaires d'objets InfosImage
            objets_nodes = minidom.parse(os.path.join(chemins.OBJETS_MAP, "%s.xml" % typeObj)).documentElement.getElementsByTagName("obj")
            for obj in objets_nodes:
                attrs = obj.attributes
                self[typeObj][int(attrs["num"].value, 16)] = InfosImage(attrs)