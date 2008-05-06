# encoding=utf-8

from distutils.core import setup, Extension
import glob, os.path


listeSources = [f for f in glob.glob(os.path.join("src","*")) if f.endswith(".cpp")]


setup(
name = 'Weltanschauung',
version = '1.0',
description = 'Moteur graphique pour Schpanzerbruck',
license = 'GPL v3',
ext_modules = [
                Extension('PyWS._ws',
                    ["ws_wrap.cxx"] + listeSources, \
                    libraries=['sfml-graphics', 'sfml-window', 'sfml-audio', 'sfml-system', 'GL', 'GLU'],
                    include_dirs=["include"])
              ],
packages = ["PyWS"],
package_dir = {"PyWS":""}
)
