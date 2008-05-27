#ifndef PYMOTEURCOMBAT_HEADER
#define PYMOTEURCOMBAT_HEADER

#include <Python.h>
#include <structmember.h>
#include <MoteurCombat.hpp>

#include <vector>
#include <string>

using namespace std;


typedef struct {
    PyObject_HEAD
    ws::MoteurCombat* instc;
} pyMoteurCombat;

void pyMoteurCombat_initConsts();

#endif
