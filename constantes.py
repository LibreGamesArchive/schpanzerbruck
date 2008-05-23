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
chemins.POLICES_BITMAP = os.path.join(cheminDossierRacine, "rsc", "bmpfonts")
chemins.SONS = os.path.join(cheminDossierRacine, "rsc", "sons")
chemins.MAPS = os.path.join(cheminDossierRacine, "maps")
chemins.SAUVEGARDES = os.path.expanduser(os.path.join("~", ".schpanzerbruck"))
chemins.CAPTURES = os.path.join(cheminDossierRacine, "captures")

defaut=utils.ConstsContainer()    # Création d'une nouvelle instance vide de defaut
defaut.PSYCO = True
defaut.PLEIN_ECRAN = True
defaut.SYNCHRO_VERTICALE = True
defaut.FPS_MAX = 240
defaut.MODE = (800, 600, 32)
defaut.MODE_AUTO = True

# FIXME
defaut.touches = utils.Container()
defaut.touches.CAPTURE = sf.Key.F9
defaut.touches.ZOOM_AVANT = sf.Key.PageUp
defaut.touches.ZOOM_ARRIERE = sf.Key.PageDown
defaut.touches.DEFIL_GAUCHE = sf.Key.Left
defaut.touches.DEFIL_DROITE = sf.Key.Right
defaut.touches.DEFIL_HAUT = sf.Key.Up
defaut.touches.DEFIL_BAS = sf.Key.Down

defaut.CARTE = u"maptest2-cold.xml"
defaut.TEXTURE_BORDURE = sf.Image(1, 1, sf.Color(128, 128, 128))
defaut.HAUTEUR_BORDURE = 0.6

reseau = utils.ConstsContainer()
reseau.IP_PORT_SERVEUR = ("", 3088)
reseau.PORT_BROADCAST = 3066