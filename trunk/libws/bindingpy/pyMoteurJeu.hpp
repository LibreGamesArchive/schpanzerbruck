#ifndef PYMOTEURJEU_HEADER
#define PYMOTEURJEU_HEADER

#include <Python.h>
#include <structmember.h>
#include <MoteurJeu.hpp>
#include <MapClient.hpp>
#include "pyMoteurCombat.hpp"

typedef struct {
    PyObject_HEAD
    ws::MoteurJeu* instc;    // L'instance C++ associée
} pyMoteurJeu;

#endif
