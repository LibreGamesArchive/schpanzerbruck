# encoding=UTF-8

# Dernière version par charlie du 25/04/08
# Correction mineure encodage UTF-8 par florian 01/05/08

from xml.dom import minidom
from utils import Container
from objequip import *
import maitrises
from maitrises import *



class Personnage:
    """LA classe décrivant un personnage"""
    
    def __init__(self, doc=None):
        """Définit les attributs (stats, etc.) du personnage"""
        
        self.nom = ""
        
        self.stats = { "FCP": 0, "FCM": 0, "DFP": 0, "DFM": 0, "AGI": 0, "INI": 0, "MVT": 0 }
        
        self.VAL = 0 # VALEUR
        self.VIE = 100.0
        self.FTG = 0.0
        # VAL, VIE et FTG ne sont pas mises dans les stats car :
        # - VAL est recalculée (afin d'éviter au maximum la triche)
        # - VIE et FTG démarrent toujours à des valeurs dépendant des maîtrises (le plus souvent 100 et 0)
        
        self.pos = 0   # La case sur laquelle se trouve le perso
        self.idClient = ""
        
        self.maitrises = [] # Liste des (maitrises, grade) (objets de type Maitrise) possédées par le perso, on initialise avec la maitrise trouvée par le questionnaire
        
        self.equipement = { "arme": None, "bouclier": None, "casque": None, "plastron": None }
        # Tous les items équipés sont des instances de la classe ObjetEquip
        
        if doc != None:
            self.importerDepuisXML(doc)
    
    
    def statAvecEquip(self, stat):
        """Donne la stat du perso prenant en compte son équipement"""
        if stat == "portee":
            return self.equipement["arme"].stats["portee"]
        return self.stats[stat] + sum([eq.stats[stat] for eq in self.equipement.values()])
    
    
    def ajMaitrise(self, maitrise, grade):
        """ Ajoute une maitrise à la liste de maitrise du personnage"""
        self.maitrises.append([maitrise, grade]) #maitrise de type maitrises
    
    def detMaitrise(self, indiceMaitrise):
        """ Détruit une maitrise existante"""
        if (indiceMaitrise >= 0) and (indiceMaitrise < len(self.maitrise)):
            self.maitrise.remove(indiceMaitrise)
    
    def changeGrade(self, indiceMaitrise, grade):
        """ Change le grade d'une maitrise donnée """
        self.maitrises[indiceMaitrise][1] = grade
    
    def changeNom(self,nom):
        self.nom = nom
    
    def importerDepuisXML(self, doc):
        """Charge le perso depuis un minidom.Document"""
        root = doc.documentElement
        self.nom = root.attributes["nom"].value
        
        stats = root.getElementsByTagName("stats")[0]
        for st in self.stats.keys():
            self.stats[st] = int(stats.attributes[st].value)
        
        listeMaitrises_str = root.getElementsByTagName("maitrises")[0].firstChild.data.strip().split(",")
        for maitriseEtGrade_str in listeMaitrises_str:
            maitrise_str, grade = maitriseEtGrade_str.split(":")
            self.ajMaitrise(maitrises.__dict__[maitrise_str], grade)
        
        for typeEq, objEq in root.getElementsByTagName("equipement")[0].attributes.items():
            if objEq != "aucun":
                self.equipement[typeEq] = objequip.ObjetEquip(typeEq, objEq)
        
        valEquip = 0
        valMaitrises = 0
        if len(self.equipement) != 0:
            valEquip = sum([eq.VAL for eq in self.equipement.values() if eq != None])
        if len(self.maitrises) != 0:
            valMaitrises = sum([mtr.calcVal(grade) for mtr, grade in self.maitrises])
        self.VAL = valEquip + valMaitrises
    
    
    def exporterEnXML(self):
        """Renvoie le minidom.Document correspondant au perso"""
        doc = minidom.Document()
        root = doc.createElement("perso")
        root.setAttribute("nom", self.nom)
        
        stats = doc.createElement("stats")
        for st, val in self.stats.items():
            stats.setAttribute(st, str(val))
        root.appendChild(stats)
        
        maitrises = doc.createElement("maitrises")
        maitrises_text = doc.createTextNode("")
        for m in self.maitrises:
            maitrises_text.data += m[0].nom + ":" + m[1] + ","
        if maitrises_text.data != "":
            maitrises_text.data = maitrises_text.data[0:-1] # On vire la dernière ","
        maitrises.appendChild(maitrises_text)
        root.appendChild(maitrises)
        
        equipement = doc.createElement("equipement")
        for typeEq, objEq in self.equipement.items():
            if objEq != None:
                equipement.setAttribute(typeEq, objEq.nom)  # objEq.num si les ObjetsEquip définissent un attribut nom (non définitif, of course)
            else:
                equipement.setAttribute(typeEq, "aucun")
        root.appendChild(equipement)
        
        doc.appendChild(root)
        
        return doc
