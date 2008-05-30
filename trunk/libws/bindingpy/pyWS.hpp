#ifndef PYWS_MODULE_HEADER
#define PYWS_MODULE_HEADER

#include <Python.h>
#include <structmember.h>
#include "pyMoteurJeu.hpp"
#include "pyMoteurCombat.hpp"

void pyWS_initConsts(PyObject *WSModule);

#endif
