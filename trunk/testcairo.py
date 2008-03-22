#! /usr/bin/env python
# encoding=UTF-8

from PySFML import sf
import cairo
from sfcairo import sfImageDepuisSurface
import time, math


def imageHorloge():
	surf = cairo.ImageSurface(cairo.FORMAT_ARGB32, 300, 300)
	cr = cairo.Context(surf)
	
	t = time.localtime()
	minutes, hours, seconds = t.tm_min*math.pi/30, t.tm_hour*math.pi/6, t.tm_sec*math.pi/30
	
	cr.set_source_rgba(1, 1, 1, 1)
	cr.paint()
	
	cr.set_line_cap(cairo.LINE_CAP_ROUND)
	cr.set_line_width(0.1)
	
	cr.set_source_rgba(0, 0, 0, 1)
	cr.translate(0.5, 0.5)
	cr.arc(0, 0, 0.4, 0, math.pi*2)
	cr.stroke()
	
	surf.flush()
	img = sf.Image()
	img.LoadFromPixels(300, 300, surf.get_data())
	return img


app = sf.RenderWindow(sf.VideoMode(800, 600, 32), "Do Ya Get It ?", sf.Style.Close)
app.UseVerticalSync(True)
app.SetFramerateLimit(60)

evt = sf.Event()
run = True
while run:
	while app.GetEvent(evt):
		if evt.Type == sf.Event.Closed:
			run = False
	
	img = imageHorloge()
	spr = sf.Sprite(img)
	
	t = app.GetFrameTime()
	print ("FPS : %s" % (1.0/t if t!=0 else 0.0))
	
	app.Draw(spr)
	app.Display()
