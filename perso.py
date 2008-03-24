# encoding=UTF-8

from PySFML import sf
from xml.dom import minidom
from utils import Container


class Maitrise:
    """DÃ©crit une maÃ®trise"""

    def __init__(self, nom="", nomComplet=""):

        self.grades =  { "A": Container(), "B": Container(), "C": Container(), "D": Container(), "E": Container() }
        
        for k in self.grades.keys():
            self.grades[k].nom = ""
            self.grades[k].attaque = ""
        
        self.dependances = [] # Liste contenant des tuples (matrise, grade) dans lesquels maitrise et grade sont des strings.
        # Donne les maitrises et grades nÃ©cessaires pour utiliser cette maÃ®trise
        
        self.nom = nom
        self.nomComplet = nomComplet


class ObjetEquip:
    """DÃ©crit un objet d'Ã©quipement"""
    
    def __init__(self, type, num):
        """Lit dans le fichier XML correspondant au type d'Ã©quipement spÃ©cifiÃ© l'Ã©quipement num"""
        pass


class Personnage:
    """LA classe dÃ©crivant un personnage"""
    
    def __init__(self,maitrise_premiere): # maitrise_premiere est une string
        """DÃ©finit les attributs (stats, etc.) du personnage"""

        self.nom = ""

        self.stats = { "FCP": 0, "FCM": 0, "DFP": 0, "DFM": 0, "AGI": 0, "INI": 0, "MVT": 0 }

        self.VAL = 0 # VALEUR
        self.VIE = 100.0
        self.FTG = 0.0
        # VAL, VIE et FTG ne sont pas mises dans les stats car :
        # - VAL est recalculÃ©e (afin d'Ã©viter au maximum la triche)
        # - VIE et FTG dÃ©marrent toujours Ã  des valeurs dÃ©pendant des maÃ®trises (le plus souvent 100 et 0)

        self.maitrises = {} # Dictionnaire des maitrises (objets de type Maitrise) possÃ©dÃ©es par le perso, on initialise avec la maitrise trouvée par le questionnaire
		self.maitrise[maitrise_premiere]="E" 

        self.equipement = { "bras_droit": None, "bras_gauche": None, "tete": None, "torse": None, "pieds": None, "acc1": None, "acc2": None, "special": None }
        # acc1 et acc2 sont les accessoires (colliers, bracelets pour booster), et special est un objet spÃ©cial (cheval, catapulte) dÃ©pendant gÃ©nÃ©ralement d'une maÃ®trise particuliÃ¨re
        # Tous les items Ã©quipÃ©s sont des instances de la classe ObjetEquip

        self.sprite = None
    
    
    def addMaitrise(self, maitrise):
        """ Ajoute une maitrise(de type string) à la liste de maitrise du personnage"""
        self.maitrises[maitrise]="E" #Le dictionnaire des maitrises possédées par le personnage est augmentée par une nouvelle maitrise, initialisée au plus faible grade
	
	def changeGrade(self,maitrise,grade)
		""" Change le grade d'une maitrise donnée """
		if
	
	
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
            maitrises_text.data = maitrises_text.data[0:-1] # On vire la derniÃ¨re ","
        maitrises.appendChild(maitrises_text)
        root.appendChild(maitrises)
        
        equipement = doc.createElement("equipement")
        for typeEq, objEq in self.equipement.items():
            if objEq != None:
                equipement.setAttribute(typeEq, objEq.nom)  # objEq.num si les ObjetsEquip dÃ©finissent un attribut nom (non dÃ©finitif, of course)
            else:
                equipement.setAttribute(typeEq, "aucun")
        root.appendChild(equipement)
        
        doc.appendChild(root)
        
        return doc