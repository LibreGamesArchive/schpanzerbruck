# encoding=utf-8

from distutils.core import setup, Extension
import glob, os.path


listeSources = [f for f in glob.glob(os.path.join("src","*")) if f.endswith(".cpp")]
listeSources += [f for f in glob.glob(os.path.join("bindingpy","*")) if f.endswith(".cpp")]


setup(
name = 'Weltanschauung',
version = '1.0',
description = 'Moteur graphique pour Schpanzerbrück',
license = 'GPL v3',
ext_modules = [
                Extension('PyWS.ws',
                    listeSources, \
                    libraries=['sfml-graphics', 'sfml-window', 'sfml-audio', 'sfml-system', 'GL', 'GLU'],
                    include_dirs=["include"],
                    extra_compile_args=["-O3", "-ffast-math", "-fomit-frame-pointer", "-funroll-loops"]
                )
              ],
packages = ["PyWS"],
package_dir = {"PyWS":""}
)
