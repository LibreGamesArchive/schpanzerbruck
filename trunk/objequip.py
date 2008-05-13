# encoding=UTF-8

from xml.dom import minidom

class ObjetEquip:
    """Decrit un objet d'equipement"""
    
    def __init__(self, typeEquip, nom):
        """Lit dans le fichier XML correspondant au type d'equipement, specifie l'equipement num"""
        
        self.typeEquip = typeEquip    #type d'equipement, correspondant au nom du fichier XML
        self.nom = nom                #nom reduit de l'équipement dans le fichier XML
        
        self.__importerDepuisXML()
    
    def __importerDepuisXML(self):
        """Charge l'equipement depuis un fichier XML"""
        
        self.doc = minidom.parse("typeEquip") #la flemme pour l'instant de recuperer le nom du XML
        
        equip_node = doc.documentElement
        self.nomComplet = equip_node.attributes["nomComplet"].value
        
        equip_node.getElementsByTagName("stats")[0]
