#! /usr/bin/env python
# encoding=UTF-8

##########################################################################
### Schpanzerbrück
### Copyright (C) 2008 The_Schpanzerteam
###
### This program is free software: you can redistribute it and/or modify
### it under the terms of the GNU General Public License as published by
### the Free Software Foundation, either version 3 of the License, or
### (at your option) any later version.
###
### This program is distributed in the hope that it will be useful,
### but WITHOUT ANY WARRANTY; without even the implied warranty of
### MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
### GNU General Public License for more details.
###
### You should have received a copy of the GNU General Public License
### along with this program.  If not, see <http://www.gnu.org/licenses/>.
##########################################################################

import os.path, sys
import ctxclient
from constantes import chemins
import parser
import utils

try:
    from PyWS import ws
except ImportError:
    print >> sys.stderr, "Schpanzerbrück a besoin de SFML en plus d'OpenGL pour fonctionner correctement.\n Vous pouvez l'installer à partir de http://www.sfml-dev.org"
    sys.exit()

# Chargement de la config
config=utils.Config.getInstance()
config.chargerConfig(os.path.join(chemins.SAUVEGARDES, "schpbr.cfg"))

# Parsage de ligne de commande
options = parser.retourneOptions()

try:
    if options.psyco:
        import psyco    # A quoi ça sert ?
        psyco.full()    # A accélerer la compilation "on the fly" du code Python, mais mange plus de RAM
        print "Psyco: ON"
    else:
        print "Psyco: OFF"
except ImportError:
    print "Psyco: ** Not found **"
    options.psyco = False

if options.fenetre:     # On lance le jeu en fenêtré
    print "Fullscreen: OFF"
else:
    print "Fullscreen: ON"

welt = ws.MoteurJeu(not options.fenetre, config.MODE_AUTO, config.SYNCHRO_VERTICALE, config.MODE[0], config.MODE[1], config.MODE[2]);
#welt.limiterFPS(defaut.FPS_MAX)


if os.path.exists(os.path.join(chemins.MAPS, options.carte)):
    carte = os.path.join(chemins.MAPS, options.carte)
else:
    carte = os.path.join(chemins.MAPS, defaut.CARTE)
# Fin parsage


CTX = ctxclient.ContexteClient(app, carte)
CTX.lancerBoucle()
