# encoding=UTF-8

import utils, os.path

cheminDossierRacine = os.path.split(os.path.realpath(__file__))[0]

chemins = utils.ConstsContainer()

chemins.OBJETS = os.path.join(cheminDossierRacine, "objets")
chemins.IMGS_TUILES = os.path.join(cheminDossierRacine, "visuels", "tuiles")
chemins.IMGS_GDSELEMENTS = os.path.join(cheminDossierRacine, "visuels", "gdsElements")
chemins.IMGS_PTSELEMENTS = os.path.join(cheminDossierRacine, "visuels", "ptsElements")
chemins.IMGS_PERSOS = os.path.join(cheminDossierRacine, "visuels", "persos")
chemins.MAPS = os.path.join(cheminDossierRacine, "maps")


tailles = utils.ConstsContainer()

tailles.LARGEUR_TUILES = 128
tailles.HAUTEUR_TUILES = 80


defaut = utils.ConstsContainer()

defaut.PERSPECTIVE = 30   # Positif: Perspective droite, Négatif: Perspective gauche
defaut.CARTE = u"maptest2.xml"
defaut.PLEIN_ECRAN = False