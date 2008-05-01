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


tailles = utils.ConstsContainer()

tailles.LARGEUR_TUILES = 128
tailles.HAUTEUR_TUILES = 80


defaut = utils.chargerConfig(os.path.join(chemins.SAUVEGARDES, "schpbr.cfg"))

#defaut.PSYCO = True
#defaut.PLEIN_ECRAN = True
#defaut.SYNCHRO_VERTICALE = True
#defaut.FPS_MAX = 240
#defaut.MODE = (800, 600, 32)
#defaut.MODE_AUTO = True

#defaut.touches = Container()
#defaut.touches.ZOOM_AVANT = Key.PageUp
#defaut.touches.ZOOM_ARRIERE = Key.PageDown
#defaut.touches.DEFIL_GAUCHE = Key.Left
#defaut.touches.DEFIL_DROITE = Key.Right
#defaut.touches.DEFIL_HAUT = Key.Up
#defaut.touches.DEFIL_BAS = Key.Down

defaut.CARTE = u"maptest2-cold.xml"


reseau = utils.ConstsContainer()
reseau.IP_PORT_SERVEUR = ("", 3088)
reseau.PORT_BROADCAST = 3066