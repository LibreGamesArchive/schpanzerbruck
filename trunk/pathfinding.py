import copy, sys

def pathfinding(coord, mvt, origine="0", coordsPossibles=[], chemin=[], route=[], mvtRestant=[]):
	"""Renvoie un tuple de 2 listes : les positions accessibles et les chemins correspondants a parcourir

	Arguments: coord: la case actuelle du perso
	mvt: la capacité de mouvement du perso
	Les autres arguments servent pour la recursivite"""

	if not coord in coordsPossibles:
		coordsPossibles.append(coord)
		chemin.append(copy.copy(route))
		mvtRestant.append(mvt)
	else:
		index=coordsPossibles.index(coord)
		if mvtRestant[index] < mvt:
			chemin[index] = copy.copy(route)
			mvtRestant[index] = mvt
	
	largeur=10
	hauteur=10
	terrain=1 # Cout de la case actuelle (coord) en mouvement POUR SORTIR DE LA CASE
	if mvt-terrain >= 0:
		if (coord+1)%largeur != 0 and origine != "d" and True:
			route.append("d")
			pathfinding(coord+1,mvt-terrain,"g",coordsPossibles,chemin,route,mvtRestant)
			route.pop()
		
		if coord%largeur != 0 and origine != "g" and True:
			route.append("g")
			pathfinding(coord-1,mvt-terrain,"d",coordsPossibles,chemin,route,mvtRestant)
			route.pop()
		
		if coord-largeur >= 0 and origine != "h" and True:
			route.append("h")
			pathfinding(coord-largeur,mvt-terrain,"b",coordsPossibles,chemin,route,mvtRestant)
			route.pop()
		
		if (coord+largeur) < largeur*hauteur and origine != "b" and True:
			route.append("b")
			pathfinding(coord+largeur,mvt-terrain,"h",coordsPossibles,chemin,route,mvtRestant)
			route.pop()

	return coordsPossibles, chemin, mvtRestant


if __name__ == "__main__":
	liste,chemin,mvt=pathfinding(25,int(sys.argv[1]))
	print liste
	print chemin
	print mvt
