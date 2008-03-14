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
tailles.DECALAGE_TUILES = 30

tailles.DECALAGE_PTS_ELEMENTS_Y = int(tailles.HAUTEUR_TUILES*(3.0/4))
tailles.DECALAGE_PTS_ELEMENTS_X = int(tailles.DECALAGE_TUILES*(3.0/4))   # Th. de Thalès
tailles.DECALAGE_GDS_ELEMENTS_Y = tailles.HAUTEUR_TUILES/2
tailles.DECALAGE_GDS_ELEMENTS_X = tailles.DECALAGE_TUILES/2   # Th. de Thalès