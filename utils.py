# encoding=UTF-8

import os.path
from xml.dom import minidom
from PySFML.sf import Key, VideoMode


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


class Config:
	"""Contient/Charge/Sauvegarde les options du jeu"""
	
	def __init__(self, fichier = "schpbr.cfg"):
		self.fichier = fichier
		
		cfgParDefaut = False
		
		if os.path.exists(self.fichier):
			try:
				doc = minidom.parse(self.fichier)
			except:
				cfgParDefaut = True
				print "Le fichier de configuration n'est pas valide,\nil sera recréé avec la config par défaut"
			else:
				cfgParDefaut = not self.__chargerConfigXML(doc.documentElement)
		else:
			cfgParDefaut = True
		
		if cfgParDefaut:
			self.PSYCO = True
			self.PERSPECTIVE = 30   # Positif: Perspective droite, Négatif: Perspective gauche
			self.PLEIN_ECRAN = True
			self.SYNCHRO_VERTICALE = True
			self.FPS_MAX = 240
			self.mode = VideoMode.GetDesktopMode()
			self.MODE_AUTO = True
			self.touches = Container()
			self.touches.ZOOM_AVANT = Key.PageUp
			self.touches.ZOOM_ARRIERE = Key.PageDown
			self.touches.DEFIL_GAUCHE = Key.Left
			self.touches.DEFIL_DROITE = Key.Right
			self.touches.DEFIL_HAUT = Key.Up
			self.touches.DEFIL_BAS = Key.Down
			
			self.sauvegarder()
	
	
	def __chargerConfigXML(self, root):
		"""Revoie True si le chargement s'est correctement effectué"""
		
		try:
			if root.getElementsByTagName("psyco")[0].attributes["UTILISER"].value == "True":
				self.PSYCO = True
			else:	self.PSYCO = False
			
			self.PERSPECTIVE = int(root.getElementsByTagName("perspective")[0].attributes["VAL"].value)
			
			paramsEcran = root.getElementsByTagName("ecran")[0]
			for i in ["PLEIN_ECRAN", "SYNCHRO_VERTICALE"]:
				if paramsEcran.attributes[i].value == "True":
					self.__dict__[i] = True
				else:	self.__dict__[i] = False
			self.FPS_MAX = int(paramsEcran.attributes["FPS_MAX"].value)
			
			paramsMode = paramsEcran.getElementsByTagName("mode")[0]
			if paramsMode.attributes["AUTO"].value == "True":
				self.mode = VideoMode.GetDesktopMode()
				self.MODE_AUTO = True
			else:
				W = int(paramsMode.attributes["W"].value)
				H = int(paramsMode.attributes["H"].value)
				BPP = int(paramsMode.attributes["BPP"].value)
				self.mode = VideoMode(W, H, BPP)
				self.MODE_AUTO = False
			
			self.touches = Container()
			codesTouches = root.getElementsByTagName("touches")[0].attributes
			for i in ["ZOOM_AVANT", "ZOOM_ARRIERE", "DEFIL_GAUCHE", "DEFIL_DROITE", "DEFIL_HAUT", "DEFIL_BAS"]:
				self.touches.__dict__[i] = int(codesTouches[i].value)
		except:
			print "Certaines options du fichier de config sont manquantes,\nla config par défaut sera donc chargée"
			return False
		return True
	
	
	def sauvegarder(self):
		doc = minidom.Document()
		
		root = doc.createElement("config")
		
		psyco = doc.createElement("psyco")
		psyco.setAttribute("UTILISER", str(self.PSYCO))
		root.appendChild(psyco)
		
		perspective = doc.createElement("perspective")
		perspective.setAttribute("VAL", str(self.PERSPECTIVE))
		root.appendChild(perspective)
		
		ecran = doc.createElement("ecran")
		for i in ["PLEIN_ECRAN", "SYNCHRO_VERTICALE"]:
			ecran.setAttribute(i, str(self.__dict__[i]))
		ecran.setAttribute("FPS_MAX", str(self.FPS_MAX))
		mode = doc.createElement("mode")
		mode.setAttribute("AUTO", str(self.MODE_AUTO))
		mode.setAttribute("W", str(self.mode.Width))
		mode.setAttribute("H", str(self.mode.Height))
		mode.setAttribute("BPP", str(self.mode.BitsPerPixel))
		ecran.appendChild(mode)
		root.appendChild(ecran)
		
		touches = doc.createElement("touches")
		for i in ["ZOOM_AVANT", "ZOOM_ARRIERE", "DEFIL_GAUCHE", "DEFIL_DROITE", "DEFIL_HAUT", "DEFIL_BAS"]:
			touches.setAttribute(i, str(self.touches.__dict__[i]))
		root.appendChild(touches)
		
		doc.appendChild(root)
		
		f = open(self.fichier, "w")
		f.write(doc.toprettyxml(indent="    "))
		f.close()