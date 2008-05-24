#ifndef PYMOTEURCOMBAT_HEADER
#define PYMOTEURCOMBAT_HEADER

#include <Python.h>
#include <structmember.h>
#include <MoteurCombat.hpp>


typedef struct {
    PyObject_HEAD
    ws::MoteurCombat* instc;
} pyMoteurCombat;

#endif
