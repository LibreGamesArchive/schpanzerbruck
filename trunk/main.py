#! /usr/bin/env python
# encoding=UTF-8

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