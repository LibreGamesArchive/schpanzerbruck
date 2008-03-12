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


from PySFML import sf
import os.path
from img import GestionnaireImages
from mapmng import Map
from constantes import chemins


app = sf.RenderWindow(sf.VideoMode(800, 600, 32), "SCHPANZERBRUCK", sf.Style.Close)

gest = GestionnaireImages()

map = Map(os.path.join(chemins.MAPS, "maptest.xml"), gest)


# MAIN LOOP:
run = True
evt = sf.Event()
while run:
    while app.GetEvent(evt):
            if evt.Type == sf.Event.Closed:
                  run = False
            elif evt.Type == sf.Event.KeyPressed:
                  if evt.Key.Code == sf.Key.Escape:
                        run = False
    
    map.dessinerSur(app)
    app.Display()
