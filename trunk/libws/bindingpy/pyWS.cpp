#include "pyWS.hpp"


extern PyTypeObject pyMoteurJeuType;
extern PyTypeObject pyMoteurCombatType;


static PyMethodDef pyWS_methods[] = {
    {NULL}      // Sentinel
};


#ifndef PyMODINIT_FUNC /* declarations for DLL import/export */
#define PyMODINIT_FUNC void
#endif
PyMODINIT_FUNC initws(void)
{
    PyObject* WSModule;
    WSModule = Py_InitModule3("ws", pyWS_methods, "Moteur Graphique pour Schpanzerbruck");
    if(WSModule == NULL)
        return;
    
    if (PyType_Ready(&pyMoteurJeuType) < 0)
        return;
    Py_INCREF(&pyMoteurJeuType);
    PyModule_AddObject(WSModule, "MoteurJeu", (PyObject *)&pyMoteurJeuType);
    
    if (PyType_Ready(&pyMoteurCombatType) < 0)
        return;
    Py_INCREF(&pyMoteurCombatType);
    PyModule_AddObject(WSModule, "MoteurCombat", (PyObject *)&pyMoteurCombatType);
    
    pyWS_initConsts(WSModule);
}


void pyWS_initConsts(PyObject* WSModule)
{
    PyObject *cst;
    
    cst = PyInt_FromLong(ws::RAS);
    PyModule_AddObject(WSModule, "RAS", cst);
    
    cst = PyInt_FromLong(ws::QUITTER);
    PyModule_AddObject(WSModule, "QUITTER", cst);
    
    cst = PyInt_FromLong(ws::DEPLACEMENT_DEMANDE);
    PyModule_AddObject(WSModule, "DEPLACEMENT_DEMANDE", cst);
    
    cst = PyInt_FromLong(ws::MAITRISES_CHOISIES);
    PyModule_AddObject(WSModule, "MAITRISES_CHOISIES", cst);
    
    cst = PyInt_FromLong(ws::CASE_CHOISIE);
    PyModule_AddObject(WSModule, "CASE_CHOISIE", cst);
    
    cst = PyInt_FromLong(ws::CIBLE_CHOISIE);
    PyModule_AddObject(WSModule, "CIBLE_CHOISIE", cst);
    
    cst = PyInt_FromLong(ws::LISTE_MAITRISES_DEMANDEE);
    PyModule_AddObject(WSModule, "LISTE_MAITRISES_DEMANDEE", cst);
    
    cst = PyInt_FromLong(ws::FIN_DU_TOUR);
    PyModule_AddObject(WSModule, "FIN_DU_TOUR", cst);
    
    cst = PyInt_FromLong(ws::GRADE_E);
    PyModule_AddObject(WSModule, "GRADE_E", cst);
    
    cst = PyInt_FromLong(ws::GRADE_D);
    PyModule_AddObject(WSModule, "GRADE_D", cst);
    
    cst = PyInt_FromLong(ws::GRADE_C);
    PyModule_AddObject(WSModule, "GRADE_C", cst);
    
    cst = PyInt_FromLong(ws::GRADE_B);
    PyModule_AddObject(WSModule, "GRADE_B", cst);
    
    cst = PyInt_FromLong(ws::GRADE_A);
    PyModule_AddObject(WSModule, "GRADE_A", cst);
    
    cst = PyInt_FromLong(ws::TUILE);
    PyModule_AddObject(WSModule, "TUILE", cst);
    
    cst = PyInt_FromLong(ws::ELEMENT);
    PyModule_AddObject(WSModule, "ELEMENT", cst);
    
    cst = PyInt_FromLong(ws::PERSO);
    PyModule_AddObject(WSModule, "PERSO", cst);
    
    cst = PyInt_FromLong(ws::GAUCHE);
    PyModule_AddObject(WSModule, "GAUCHE", cst);
    
    cst = PyInt_FromLong(ws::DROITE);
    PyModule_AddObject(WSModule, "DROITE", cst);
    
    cst = PyInt_FromLong(ws::HAUT);
    PyModule_AddObject(WSModule, "HAUT", cst);
    
    cst = PyInt_FromLong(ws::BAS);
    PyModule_AddObject(WSModule, "BAS", cst);
}
