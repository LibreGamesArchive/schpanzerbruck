# encoding=UTF-8

# EFFETS SPECIAUX:   # Modifient à chaque image plusieurs variables de la map.
# Retournent True si l'effet est fini, False sinon
class DeploiementElements:
    def __call__(self, map, frameTime):
        if map.inclinaisonElements < 75:
            map.inclinaisonElements += 130*frameTime
            if map.inclinaisonElements > 75:
                map.inclinaisonElements = 75
                return True
        return False
# FIN EFFETS SPECIAUX