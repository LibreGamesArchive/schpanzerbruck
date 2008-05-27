#Formatage des Chaines de caracteres
#    "c'est trop bien stringIO" F.LEGER

from xml.dom import minidom
from StringIO import StringIO

# Fonctions de formatage de String -> XML	effectuées par le SERVEUR
def finAttaque(cible,posAtt,santAtt,fatAtt):	
	'''renvoie les caracteristiques des persos suites a une attaque '''
	
	a=""
	b=""
	c=""
	
	for i in cible[0] :
		a=a+str(i)+"|"
	
	for i in cible[1] :
		b=b+str(i)+"|"
	
	for i in cible[2] :
		c=c+str(i)+"|"
	
	a=a[0:len(a)-1]
	
	b=b[0:len(b)-1]
	
	c=c[0:len(c)-1]
	
	return (StringIO("<finatt><cible><pos>"+a+"</pos><sante>"+b+"</sante><fat>"+c+"</fat></cible><attaquant>"+str(posAtt)+"|"+str(santAtt)+"|"+str(fatAtt)+"</attaquant></finatt>"))
			
		
				
def posiAccess(listePos):
	'''renvoie la liste des positions accessibles sans leur chemin'''
	a=""
	for i in listePos :
		a=a+str(i)+"|"
	
	a=a[0:len(a)-1]
	return(StringIO("<Dep>"+a+"<Dep>"))

def chemin(liste):
	'''renvoie le chemin pour une case definie'''
	a=""
	for i in liste :
		a=a+str(i)+"|"
	a=a[0:len(a)-1]
	return(StringIO("<leChemin>"+a+"</leChemin>"))
	
def posAtt(liste,nomClient):
	'''renvoie la liste des positions ou l'on peut attaquer(calcule en fonction des maitrises envoyees par le "demandeur" avant'''	
	a=""
	for i in liste :
		a=a+str(i)+"|"
	a=a[0:len(a)-1]
	return(StringIO("<posAtt droits=\""+nomClient+"\">"+a+"</posAtt>")) 
	


# Fonctions de formatage de String -> XML	effectuées par le CLIENT
def demandeDeplacement(posIni,nbMouv):
	'''Requête demandée au serveur pour obtenir la liste des positions accessibles en nbMouv'''
	return(StringIO("<demDeplacement posIni="+str(posIni)+" nbMouv="+str(nbMouv)+"></demDeplacement>"))

def debutAttaque(listeMaitrise1,listeMaitrise2,listeMaitrise3):
	''' Envoi des données nécessaires à l'attaque  '''
	#listeMaitrise1(ou2 ou 3) est une liste du type [objetdetypemaitrise,caracteredeGrade]
	return(StringIO("<debatt><maitrise1 nom="+listeMaitrise1[0]+" grade="+listeMaitrise1[1]+"><maitrise2 nom="+listeMaitrise2[0]+" grade="+listeMaitrise2[1]+"><maitrise3 nom="+listeMaitrise3[0]+" grade="+listeMaitrise3[1]+"></maitrise1></maitrise2></maitrise3></debatt>"))

def finTour():
	'''Envoie de la requête fin de tour'''
	








truc=attaque([[20,18,32],[48,22,0],[60,50,100]],5,50,60)	
truc2=chemin(["g","g","g","h","g","h","h","g","g","g","g","g","g","h"])	
att=minidom.parse(truc)
chemin=minidom.parse(truc2)
print att.toxml()
print "\n\n"+chemin.toxml()
