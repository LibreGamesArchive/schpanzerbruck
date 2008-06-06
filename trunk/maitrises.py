# encoding=utf-8

from random import randrange


"""Classe des maitrises (les fonctions des maitrises reçoivent en arg le grade qu'elle doivent utiliser)"""


class Maitrise:
    """Décrit une maitrise"""

    def __init__(self, nom, nomComplet, antagonismes, FCP, FCM, AGI, portee):
        self.nom = nom				        # Nom courant utilisé pour la comparaison
        self.nomComplet = nomComplet		# Nom complet, utilisé pour les crétins qui vont jouer à ce jeu stupide
        self.antagonismes = antagonismes	# Liste des maitrises qui ne peuvent pas etre utilisés ensemble
        self.FCP = FCP
        self.FCM = FCM
        self.AGI = AGI
        self.portee = portee
    
    def __calculCoeff(self, grade):
        
        if grade == "E":
            ret = randrange(11)     # Génère un nombre compris entre 0 et 11-1=10
        elif grade == "D":
            ret = randrange(11) + 10
        elif grade == "C":
            ret = randrange(21) + 20
        elif grade == "B":
            ret = randrange(21) + 40
        elif grade == "A":
            ret = randrange(41) + 60
        
        return ret / 100.0		# Adapté à l'algorithme de combat : chaque stat est sur une base 100, donc modificateur compris entre 0 et 1
    
    def calculStatEnFctGrade(self, stat, grade):
        return int(self.__dict__[stat] * self.__calculCoeff(grade))
    
    def calcVal(self, grade):
        if grade == "A":
            return 100
        if grade == "B":
            return 75
        if grade == "C":
            return 50
        if grade == "D":
            return 25
        if grade == "E":
            return 10



feu = Maitrise("feu","Maîtrise du Feu",["eau"],22,15,23,12)

eau = Maitrise("eau","Maîtrise de l'Eau",["feu"],15,22,23,12)

epees = Maitrise("epees","Maniement des épées",["arcs"],30,0,1,14)

armures = Maitrise("armures","Techniques de défenses à l'armure",[],0,0,1,0)

arcs = Maitrise("arcs","Maniement des arcs",["epees","eau"],0,25,33,2)
