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
from constantes import chemins, defaut
import parser

try:
    from PySFML import sf
except ImportError:
    print >> sys.stderr, "Schpanzerbrück a besoin de PySFML pour fonctionner correctement.\n Vous pouvez l'installer à partir de http://www.sfml-dev.org"
    sys.exit()

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

if not options.fenetre:     # On lance le jeu en plein écran
    if defaut.MODE_AUTO:
        videoMode = sf.VideoMode.GetDesktopMode()
    else:
        videoMode = sf.VideoMode(*defaut.MODE)
    style = sf.Style.Close | sf.Style.Fullscreen
    print "Fullscreen: ON"
else:   # Ou pas (mais faut éviter)
    videoMode = sf.VideoMode(*defaut.MODE)
    style = sf.Style.Close
    print "Fullscreen: OFF"
    

app = sf.RenderWindow(videoMode, "SCHPANZERBRUCK", style)
app.SetFramerateLimit(defaut.FPS_MAX)
app.UseVerticalSync(defaut.SYNCHRO_VERTICALE)
app.PreserveOpenGLStates(True)

if os.path.exists(os.path.join(chemins.MAPS, options.carte)):
    carte = os.path.join(chemins.MAPS, options.carte)
else:
    carte = os.path.join(chemins.MAPS, defaut.CARTE)
# Fin parsage


CTX = ctxclient.ContexteClient(app, carte)
CTX.lancerBoucle()
