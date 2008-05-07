# encoding=UTF-8

import os.path
import constantes
from xml.dom import minidom
from PySFML.sf import Key
from copy import copy

class Container:
    def __init__(self):
        pass


class ConstsContainer(Container):
    
    def __init__(self):
        Container.__init__(self)
    
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


class Config(Container):
    """Contient la configuration courante"""
    _instance=None
    
    def __init__(self):
        assert self._instance==None
        self._instance=self
        Container.__init__(self)
        self.__initialised=False
    
    @staticmethod
    def getInstance():
        if Config._instance==None:
            Config._instance=Config()
        return Config._instance


    def sauverConfig(self, fichier):
        """Sauvegarde la config dans un fichier"""
        assert self.__initialised==True
        doc = minidom.Document()
        
        root = doc.createElement("config")
        
        psyco = doc.createElement("psyco")
        psyco.setAttribute("UTILISER", str(self.PSYCO))
        root.appendChild(psyco)
        
        ecran = doc.createElement("ecran")
        for i in ["PLEIN_ECRAN", "SYNCHRO_VERTICALE"]:
            ecran.setAttribute(i, str(self.__dict__[i]))
        ecran.setAttribute("FPS_MAX", str(self.FPS_MAX))
        mode = doc.createElement("mode")
        mode.setAttribute("AUTO", str(self.MODE_AUTO))
        mode.setAttribute("W", str(self.MODE[0]))
        mode.setAttribute("H", str(self.MODE[1]))
        mode.setAttribute("BPP", str(self.MODE[2]))
        ecran.appendChild(mode)
        root.appendChild(ecran)
        
        touches = doc.createElement("touches")
        for i in ["CAPTURE", "ZOOM_AVANT", "ZOOM_ARRIERE", "DEFIL_GAUCHE", "DEFIL_DROITE", "DEFIL_HAUT", "DEFIL_BAS"]:
            touches.setAttribute(i, str(self.touches.__dict__[i]))
        root.appendChild(touches)
        
        doc.appendChild(root)
        
        f = open(fichier, "w")
        f.write(doc.toprettyxml(indent="    "))
        f.close()
    
    
    def __chargerDepuisXML(self, doc):
        """Revoie True si le chargement s'est correctement effectué"""
        root = doc.documentElement
        
        try:
            if root.getElementsByTagName("psyco")[0].attributes["UTILISER"].value == "True":
                self.PSYCO = True
            else:
                self.PSYCO = False
            
            paramsEcran = root.getElementsByTagName("ecran")[0]
            for i in ["PLEIN_ECRAN", "SYNCHRO_VERTICALE"]:
                if paramsEcran.attributes[i].value == "True":
                    self.__dict__[i] = True
                else:
                    self.__dict__[i] = False
            self.FPS_MAX = int(paramsEcran.attributes["FPS_MAX"].value)
            
            paramsMode = paramsEcran.getElementsByTagName("mode")[0]
            if paramsMode.attributes["AUTO"].value == "True":
                self.MODE_AUTO = True
            else:
                self.MODE_AUTO = False
            
            W = int(paramsMode.attributes["W"].value)
            H = int(paramsMode.attributes["H"].value)
            BPP = int(paramsMode.attributes["BPP"].value)
            self.MODE = (W, H, BPP)
            
            self.touches = Container()
            codesTouches = root.getElementsByTagName("touches")[0].attributes
            for i in ["CAPTURE", "ZOOM_AVANT", "ZOOM_ARRIERE", "DEFIL_GAUCHE", "DEFIL_DROITE", "DEFIL_HAUT", "DEFIL_BAS"]:
                self.touches.__dict__[i] = int(codesTouches[i].value)
        except:
            print "Certaines options du fichier de config sont manquantes,\nla config par défaut sera donc chargée"
            return False
        self.__initialised=True
        return True
    
    def chargerConfigParDefaut(self):
        self.PSYCO = copy(constantes.defaut.PSYCO)
        self.PLEIN_ECRAN = copy(constantes.defaut.PLEIN_ECRAN)
        self.SYNCHRO_VERTICALE = copy(constantes.defaut.SYNCHRO_VERTICALE)
        self.FPS_MAX = copy(constantes.defaut.FPS_MAX)
        self.MODE = copy(constantes.defaut.MODE)
        self.MODE_AUTO = copy(constantes.defaut.MODE_AUTO)
        
        self.touches = copy(constantes.defaut.touches)
        self.__initialised=True

    def chargerConfig(self, fichier):
        """Charger la config à partir d'un fichier"""
        cfgParDefaut = False
        
        if os.path.exists(fichier):
            try:
                doc = minidom.parse(fichier)
            except:
                cfgParDefaut = True
                print "Le fichier de configuration n'est pas valide,\nil sera recréé avec la config par défaut"
            else:
                cfgParDefaut = not self.__chargerDepuisXML(doc)
        else:
            cfgParDefaut = True
        
        if cfgParDefaut:
            self.chargerConfigParDefaut()