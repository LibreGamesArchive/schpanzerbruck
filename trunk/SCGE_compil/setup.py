# encoding=utf-8

from distutils.core import setup, Extension
import glob, os.path


listeSources = glob.glob(os.path.join("SRC","*"))


setup(
name = 'SCGE',
version = '1.0',
description = 'Schpanzerbruck Compiled Grapics Engine',
license = 'GPL v3',
ext_modules = [
                Extension('SCGE._gren',
                    ["gren_wrap.cxx"] + listeSources, \
                    libraries=['sfml-graphics', 'sfml-window', 'sfml-audio', 'sfml-system', 'GL', 'GLU'])
              ],
packages = ["SCGE"],
package_dir = {"SCGE":"SCGE"}
)
