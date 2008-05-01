# encoding=UTF-8

import os.path
from xml.dom import minidom
from PySFML.sf import Key


class Container:
    pass


class ConstsContainer:
    def __init__(self):
        pass
    
    def __getattr__(self, attr):
        try:
            return self.__dict__[attr]
        except:
            raise AttributeError, "%s attribute does not exist yet" % attr
    
    def __setattr__(self, attr, value):
        if attr in self.__dict__:
            raise AttributeError, "Cannot reassign constant %s" % attr
        else:
            self.__dict__[attr] = value
    
    def __str__(self):
        return '\n'.join(['%s: %s' % (str(k), str(v)) for k, v in self.__dict__.items()])
    
    def __repr__(self):
        return '\n'.join(['%s: %s' % (str(k), str(v)) for k, v in self.__dict__.items()])
    
    def __iter__(self):
        return iter(self.__dict__.values())

    
def sauverConfig(defaut, fichier):
    doc = minidom.Document()
    
    root = doc.createElement("config")
    
    psyco = doc.createElement("psyco")
    psyco.setAttribute("UTILISER", str(defaut.PSYCO))
    root.appendChild(psyco)
    
    ecran = doc.createElement("ecran")
    for i in ["PLEIN_ECRAN", "SYNCHRO_VERTICALE"]:
        ecran.setAttribute(i, str(defaut.__dict__[i]))
    ecran.setAttribute("FPS_MAX", str(defaut.FPS_MAX))
    mode = doc.createElement("mode")
    mode.setAttribute("AUTO", str(defaut.MODE_AUTO))
    mode.setAttribute("W", str(defaut.MODE[0]))
    mode.setAttribute("H", str(defaut.MODE[1]))
    mode.setAttribute("BPP", str(defaut.MODE[2]))
    ecran.appendChild(mode)
    root.appendChild(ecran)
    
    touches = doc.createElement("touches")
    for i in ["CAPTURE", "ZOOM_AVANT", "ZOOM_ARRIERE", "DEFIL_GAUCHE", "DEFIL_DROITE", "DEFIL_HAUT", "DEFIL_BAS"]:
        touches.setAttribute(i, str(defaut.touches.__dict__[i]))
    root.appendChild(touches)
    
    doc.appendChild(root)
    
    f = open(fichier, "w")
    f.write(doc.toprettyxml(indent="    "))
    f.close()


def chargerConfig(fichier):
    """Renvoie un ConstsContainer en lui attribuant les options contenues dans le fichier de config"""
    
    defaut = ConstsContainer()
    
    def chargerDepuisXML(doc):
        """Revoie True si le chargement s'est correctement effectué"""
        
        root = doc.documentElement
        
        try:
            if root.getElementsByTagName("psyco")[0].attributes["UTILISER"].value == "True":
                defaut.PSYCO = True
            else:   defaut.PSYCO = False
            
            paramsEcran = root.getElementsByTagName("ecran")[0]
            for i in ["PLEIN_ECRAN", "SYNCHRO_VERTICALE"]:
                if paramsEcran.attributes[i].value == "True":
                    defaut.__dict__[i] = True
                else:   defaut.__dict__[i] = False
            defaut.FPS_MAX = int(paramsEcran.attributes["FPS_MAX"].value)
            
            paramsMode = paramsEcran.getElementsByTagName("mode")[0]
            if paramsMode.attributes["AUTO"].value == "True":
                defaut.MODE_AUTO = True
            else:
                defaut.MODE_AUTO = False
            
            W = int(paramsMode.attributes["W"].value)
            H = int(paramsMode.attributes["H"].value)
            BPP = int(paramsMode.attributes["BPP"].value)
            defaut.MODE = (W, H, BPP)
            
            defaut.touches = Container()
            codesTouches = root.getElementsByTagName("touches")[0].attributes
            for i in ["CAPTURE", "ZOOM_AVANT", "ZOOM_ARRIERE", "DEFIL_GAUCHE", "DEFIL_DROITE", "DEFIL_HAUT", "DEFIL_BAS"]:
                defaut.touches.__dict__[i] = int(codesTouches[i].value)
        except:
            print "Certaines options du fichier de config sont manquantes,\nla config par défaut sera donc chargée"
            return False
        return True
    

    cfgParDefaut = False
    
    if os.path.exists(fichier):
        try:
            doc = minidom.parse(fichier)
        except:
            cfgParDefaut = True
            print "Le fichier de configuration n'est pas valide,\nil sera recréé avec la config par défaut"
        else:
            cfgParDefaut = not chargerDepuisXML(doc)
    else:
        cfgParDefaut = True
    
    if cfgParDefaut:
        defaut = ConstsContainer()	# Recréation de defaut (pour le vider)
        defaut.PSYCO = True
        defaut.PLEIN_ECRAN = True
        defaut.SYNCHRO_VERTICALE = True
        defaut.FPS_MAX = 240
        defaut.MODE = (800, 600, 32)
        defaut.MODE_AUTO = True
        
        defaut.touches = Container()
        defaut.touches.CAPTURE = Key.F9
        defaut.touches.ZOOM_AVANT = Key.PageUp
        defaut.touches.ZOOM_ARRIERE = Key.PageDown
        defaut.touches.DEFIL_GAUCHE = Key.Left
        defaut.touches.DEFIL_DROITE = Key.Right
        defaut.touches.DEFIL_HAUT = Key.Up
        defaut.touches.DEFIL_BAS = Key.Down
        sauverConfig(defaut, fichier)
    
    return defaut