# encoding=UTF-8

import copy
from mapbase import MapBase


class MapSrv(MapBase):
    """La map ne contenant aucune texture, destinée à être utilisée seulement par le serveur"""
    
    def __init__(self, map, gestInfos):
        MapBase.__init__(self, map)
        self.gestInfos = gestInfos
    
    
    def zoneDeplacement(self, coord, mvt, origine="0", coordsPossibles=[], chemin=[], route=[], mvtRestant=[]):
        """Renvoie 3 listes : les coords accessibles, le chemin à parcourir pour chaque et le mouvement restant au personnage pour chaque
        Arguments:
        - coord: la case actuelle du perso
        - mvt: la capacité de mouvement du perso
        Les autres arguments servent pour la recursivité"""
        
        if not coord in coordsPossibles:
            coordsPossibles.append(coord)
            chemin.append(copy.copy(route))
            mvtRestant.append(mvt)
        else:
            index = coordsPossibles.index(coord)
            if mvtRestant[index] < mvt:
                chemin[index] = copy.copy(route)
                mvtRestant[index] = mvt
        
        terrain = 1   # Cout de la case actuelle (coord) en mouvement POUR SORTIR DE LA CASE
        
        if mvt-terrain >= 0:
            if (coord+1)%self.largeur != 0 and origine != "d" and True:
                route.append("d")
                self.zoneDeplacement(coord+1,mvt-terrain,"g",coordsPossibles,chemin,route,mvtRestant)
                route.pop()
            
            if coord%self.largeur != 0 and origine != "g" and True:
                route.append("g")
                self.zoneDeplacement(coord-1,mvt-terrain,"d",coordsPossibles,chemin,route,mvtRestant)
                route.pop()
            
            if coord-self.largeur >= 0 and origine != "h" and True:
                route.append("h")
                self.zoneDeplacement(coord-self.largeur,mvt-terrain,"b",coordsPossibles,chemin,route,mvtRestant)
                route.pop()
            
            if coord+self.largeur < self.largeur*self.hauteur and origine != "b" and True:
                route.append("b")
                self.zoneDeplacement(coord+self.largeur,mvt-terrain,"h",coordsPossibles,chemin,route,mvtRestant)
                route.pop()
        
        return (coordsPossibles, chemin, mvtRestant)
