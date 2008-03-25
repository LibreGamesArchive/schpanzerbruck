# encoding=UTF-8

import utils, os.path

cheminDossierRacine = os.path.split(os.path.realpath(__file__))[0]

chemins = utils.ConstsContainer()

chemins.OBJETS_MAP = os.path.join(cheminDossierRacine, "struct", "objets_map")
chemins.OBJETS_EQUIP = os.path.join(cheminDossierRacine, "struct", "objets_equip")
chemins.MAITRISES = os.path.join(cheminDossierRacine, "struct", "maitrises")
chemins.IMGS_TUILES = os.path.join(cheminDossierRacine, "rsc", "visuels", "tuiles")
chemins.IMGS_GDSELEMENTS = os.path.join(cheminDossierRacine, "rsc", "visuels", "gdsElements")
chemins.IMGS_PTSELEMENTS = os.path.join(cheminDossierRacine, "rsc", "visuels", "ptsElements")
chemins.IMGS_PERSOS = os.path.join(cheminDossierRacine, "rsc", "visuels", "persos")
chemins.SONS = os.path.join(cheminDossierRacine, "rsc", "sons")
chemins.MAPS = os.path.join(cheminDossierRacine, "maps")
chemins.SAUV_JOUEUR = os.path.join(cheminDossierRacine, "sauv")


tailles = utils.ConstsContainer()

tailles.LARGEUR_TUILES = 128
tailles.HAUTEUR_TUILES = 80


defaut = utils.ConstsContainer()

defaut.PERSPECTIVE = 30   # Positif: Perspective droite, Négatif: Perspective gauche
defaut.CARTE = u"maptest2-cold.xml"
defaut.PLEIN_ECRAN = True
defaut.PSYCO = True