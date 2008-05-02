# encoding=UTF-8

import utils, os.path
from PySFML import sf


cheminDossierRacine = os.path.split(os.path.realpath(__file__))[0]

chemins = utils.ConstsContainer()

chemins.OBJETS_MAP = os.path.join(cheminDossierRacine, "struct", "objets_map")
chemins.OBJETS_EQUIP = os.path.join(cheminDossierRacine, "struct", "objets_equip")
chemins.MAITRISES = os.path.join(cheminDossierRacine, "struct", "maitrises")
chemins.IMGS_TUILES = os.path.join(cheminDossierRacine, "rsc", "visuels", "tuiles")
chemins.IMGS_ELEMENTS = os.path.join(cheminDossierRacine, "rsc", "visuels", "elements")
chemins.IMGS_PERSOS = os.path.join(cheminDossierRacine, "rsc", "visuels", "persos")
chemins.SONS = os.path.join(cheminDossierRacine, "rsc", "sons")
chemins.MAPS = os.path.join(cheminDossierRacine, "maps")
chemins.SAUVEGARDES = os.path.join(cheminDossierRacine, "sauv")
chemins.CAPTURES = os.path.join(cheminDossierRacine, "captures")


defaut = utils.chargerConfig(os.path.join(chemins.SAUVEGARDES, "schpbr.cfg"))

defaut.CARTE = u"maptest2-cold.xml"
defaut.TEXTURE_BORDURE = sf.Image(1, 1, sf.Color(128, 128, 128))
defaut.HAUTEUR_BORDURE = 0.6


reseau = utils.ConstsContainer()
reseau.IP_PORT_SERVEUR = ("", 3088)
reseau.PORT_BROADCAST = 3066