# encoding=UTF-8

# Dernière version par charlie du 25/04/08
# Correction mineure encodage UTF-8 par florian 01/05/08

from PySFML import sf
from xml.dom import minidom
from utils import Container
from maitrises import *



class Personnage:
    """LA classe décrivant un personnage"""
    
    def __init__(self): 
        """Définit les attributs (stats, etc.) du personnage"""

        self.nom = ""

        self.stats = { "FCP": 0, "FCM": 0, "DFP": 0, "DFM": 0, "AGI": 0, "INI": 0, "MVT": 0 }

        self.VAL = 0 # VALEUR
        self.VIE = 100.0
        self.FTG = 0.0
        # VAL, VIE et FTG ne sont pas mises dans les stats car :
        # - VAL est recalculée (afin d'éviter au maximum la triche)
        # - VIE et FTG démarrent toujours à des valeurs dépendant des maîtrises (le plus souvent 100 et 0)

        self.maitrises = [] # Liste des maitrises (objets de type Maitrise) possédées par le perso, on initialise avec la maitrise trouvée par le questionnaire
		 

        self.equipement = { "bras_droit": None, "bras_gauche": None, "tete": None, "torse": None, "pieds": None, "acc1": None, "acc2": None, "special": None }
        # acc1 et acc2 sont les accessoires (colliers, bracelets pour booster), et special est un objet spécial (cheval, catapulte) dépendant généralement d'une maîtrise particulière
        # Tous les items équipés sont des instances de la classe ObjetEquip

        self.sprite = None
    
    
    def ajMaitrise(self, maitrise,grade):
        """ Ajoute une maitrise à la liste de maitrise du personnage"""
        self.maitrises.append([maitrise, grade]) #maitrise de type maitrises
	
	def detMaitrise(self,indiceMaitrise):
		""" Détruis une maitrise existante"""
		if (indiceMaitrise >= 0) and (indiceMaitrise < len(self.maitrise))
			self.maitrise.remove(indiceMaitrise)
	
	def changeGrade(self,indiceMaitrise,grade)
		""" Change le grade d'une maitrise donnée """
		self.maitrises[indiceMaitrise][1] = grade
	
	def changeNom(self,nom)
		self.nom = nom
	
    def importerDepuisXML(self, doc):
        """Charge le perso depuis un minidom.Document"""
        root = doc.documentElement
        self.nom = root.attributes["nom"].value
        
        stats = root.getElementsByTagName("stats")[0]
        for st in self.stats.keys():
            self.stats[st] = int(stats.attributes[st].value)
        
        listeMaitrises_str = root.getElementsByTagName("maitrises")[0].firstChild.data.strip().split(",")
        # FAUT FAIRE LE CHARGEMENT DES MAITRISES EN FONCTION DE LEUR NOM
        
        dictEquipement_noms = {}
        for typeEq, objEq in root.getElementsByTagName("equipement")[0].attributes.items():
            dictEquipement_noms[typeEq] = objEq.value
        # PAREIL POUR L'EQUIPEMENT
    
    
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
            maitrises_text.data += m.nom + ","
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