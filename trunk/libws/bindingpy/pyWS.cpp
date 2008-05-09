#include "pyWS.hpp"

#include "pyMoteurJeu.hpp"
#include "pyMoteurCombat.hpp"


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
}
