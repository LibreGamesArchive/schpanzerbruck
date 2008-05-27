#Comprehension des chaines de caracteres

from xml.dom import minidom
from StringIO import StringIO


def traduction(action):
	trad=minidom.parse(action)
	
	if trad.firstChild.tagName=="demDeplacement" : #demande de deplacement du client
		nod=trad.firstChild
		posIni=int(nod.attributes["posIni"].value)
		nbMouv=int(nod.attributes["nbMouv"].value)
		liste,chemin,mouv=zoneDeplacement(posIni,nbMouv)
		#liste.index()
		posAccess(liste)
		
		
	if trad.firstChild.tagName=="demChemin" : #demande du chemin
		pass
	
	
		
		
	
traduction(StringIO("<att>yo<clo>po</clo></att>"))	

