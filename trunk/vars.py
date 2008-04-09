# encoding=UTF-8

import utils, os.path
from PySFML import sf


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


cfg = utils.Config()
# Contient tout d'abord les valeurs des options par défaut (qui seront conservées si le fichier de config n'est pas trouvé)


tailles = utils.ConstsContainer()

if cfg.PLEIN_ECRAN:
    pass
else:
    pass
tailles.LARGEUR_TUILES = 128
tailles.HAUTEUR_TUILES = 80


reseau = utils.ConstsContainer()
reseau.IP_PORT_SERVEUR = ("", 3088)
reseau.PORT_BROADCAST = 3066