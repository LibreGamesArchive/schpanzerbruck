# encoding=UTF-8

# EFFETS SPECIAUX:   # Modifient à chaque image plusieurs variables de la map.
# Retournent True si l'effet est fini, False sinon
class DeploiementElements:
    def __call__(self, map, getFrameTime):
        if map.inclinaisonElements < 70:
            map.inclinaisonElements += 130*getFrameTime()
            if map.inclinaisonElements > 70:
                map.inclinaisonElements = 70
                return True
        return False
# FIN EFFETS SPECIAUX