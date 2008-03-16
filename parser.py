# /usr/bin/env python
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



from optparse import OptionParser, make_option
from constantes import defaut

def retourneOptions():
    """Renvoie la liste des options"""
    return __options

def retourneArguments():
    """Renvoie la liste des arguments"""
    return __args

class ParserTraduit(OptionParser):
    """Traduit le message d'aide et de version en français."""
    def _add_help_option(self):
        self.add_option("-h", "--help",
                        action="help",
                        help=(u"montre ce message d'aide et s'arrête"))

    def _add_version_option(self):
        self.add_option("--version",
                        action="version",
                        help=(u"montre la version du programme et s'arrête"))

# Ajouter des options ci-dessous. Utiliser u"" pour contourner le bug d'encodage.
optionList=[make_option(u"-c", u"--carte", action=u"store", type=u"string", dest=u"carte", help=u"charge la carte à partir de FICHIER", metavar=u"FICHIER", default=defaut.CARTE),
            make_option(u"-f", u"--fenetre", action=u"store_false", dest=u"plein_ecran", default=defaut.PLEIN_ECRAN, help=u"lance le jeu en mode fenêtré"),
           ]

# Instanciation du parser
__parser=ParserTraduit(option_list=optionList)
__options, __args=__parser.parse_args()
