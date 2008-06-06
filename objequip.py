# encoding=UTF-8

from constantes import chemins
import maitrises
import os.path
from xml.dom import minidom


class ObjetEquip:
    """Decrit un objet d'equipement"""
    
    def __init__(self, type, nom):
        """Lit dans le fichier XML correspondant au type d'equipement specifie l'equipement "nom" """
        
        self.stats = { "FCP": 0, "FCM": 0, "DFP": 0, "DFM": 0, "AGI": 0, "INI": 0, "MVT": 0, "portee": 0 }
        # Ce sont les modificateurs apportés au perso (peuvent donc être négatifs)
        
        self.VAL = 0
        
        self.requiert = []  # contient des tuples (maitrise(*), grade)       (*)de type ObjetMaitrise
        
        self.nom = nom
        self.nomComplet = ""
        self.type = type
        self.desc = ""
        
        doc = minidom.parse(os.path.join(chemins.OBJETS_EQUIP, type.lower()+"s.xml"))
        for obj in doc.documentElement.getElementsByTagName("obj"):
            if obj.attributes["nom"].value == nom:
                self.nomComplet = obj.attributes["nomComplet"].value
                self.VAL = int(obj.attributes["VAL"].value)
                for nomStat, stat in obj.getElementsByTagName("stats")[0].attributes.items():
                    self.stats[nomStat] = int(stat)
                for requis in obj.getElementsByTagName("requis"):
                    self.requiert.append((maitrises.__dict__[requis.attributes["maitrise"].value], requis.attributes["grade"].value))
                self.desc = obj.getElementsByTagName("desc")[0].firstChild.data
                break
