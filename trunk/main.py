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
from vars import chemins, cfg

try:
    from PySFML import sf
except ImportError:
    print >> sys.stderr, "Schpanzerbrück a besoin de PySFML pour fonctionner correctement.\n Vous pouvez l'installer à partir de http://www.sfml-dev.org"
    sys.exit()

try:
    import OpenGL   # On ne s'en sert pas dans le main, c'est juste pour vérifier qu'il est bien installé
except ImportError:
    print >> sys.stderr, "Schpanzerbrück a besoin de PyOpenGL pour fonctionner."
    sys.exit()

try:
    if cfg.PSYCO:
        import psyco    # A quoi ça sert ?
        psyco.full()    # A accélerer la compilation "on the fly" du code Python, mais en mangeant plus de RAM
        print "Psyco: ON"
    else:
        print "Psyco: OFF"
except ImportError:
    print "Psyco: ** Not found **"


if cfg.PLEIN_ECRAN:     # On lance le jeu en plein écran
    style = sf.Style.Close | sf.Style.Fullscreen
    print "Fullscreen: ON"
else:   # Ou pas (mais faut éviter)
    style = sf.Style.Close
    print "Fullscreen: OFF"

app = sf.RenderWindow(cfg.mode, "SCHPANZERBRUCK", style)
app.SetFramerateLimit(cfg.FPS_MAX)
app.UseVerticalSync(cfg.SYNCHRO_VERTICALE)


carte = os.path.join(chemins.MAPS, "maptest2-cold.xml")


CTX = ctxclient.ContexteClient(app, carte)
CTX.lancerBoucle()
