#include "pyMoteurJeu.hpp"


extern PyTypeObject pyMoteurCombatType;

// ATTRIBUTS MEMBRES (ACCESSIBLES DEPUIS PYTHON):
static PyMemberDef pyMoteurJeu_members[] = {
    {NULL}         // Sentinel
};
// FIN ATTRIBUTS MEMBRES


// CONSTRUCTION / DESTRUCTION:
static PyObject* pyMoteurJeu_new(PyTypeObject* type, PyObject* args, PyObject* kwds)
{
    pyMoteurJeu *self;
    self = (pyMoteurJeu *)type->tp_alloc(type, 0);
    if (self != NULL)
        self->instc = NULL;
    return (PyObject *)self;
}

static int pyMoteurJeu_init(pyMoteurJeu* self, PyObject* args, PyObject* kwds)
// Doit retourner 0 si réussite et -1 si échec
{
    static char* kwlist[] = {"pleinEcran", "modeAuto", "synchroVert", "appL", "appH", "bpp", NULL};
    int pleinEcran=0, modeAuto=0, synchroVert=1;
    int appL=800, appH=600, bpp=32;
    
    if ( !PyArg_ParseTupleAndKeywords(args, kwds, "|iiiiii", kwlist,
                                      &pleinEcran, &modeAuto, &synchroVert, &appL, &appH, &bpp) )
        return -1;

    if (self->instc != NULL)
        delete self->instc;
    self->instc = new ws::MoteurJeu(pleinEcran, modeAuto, synchroVert, appL, appH, bpp);
    return 0;
}

static void pyMoteurJeu_dealloc(pyMoteurJeu* self)
{
    if(self->instc != NULL)
        delete self->instc;
    self->ob_type->tp_free((PyObject*)self);
}
// FIN CONSTRUCTION / DESTRUCTION


// METHODES:
static PyObject* pyMoteurJeu_limiterFPS(pyMoteurJeu* self, PyObject* args)
{
    int fpsMax;
    if(!PyArg_ParseTuple(args, "i", &fpsMax))
        return NULL;
    
    self->instc->limiterFPS(fpsMax);
    Py_RETURN_NONE;
}

static PyObject* pyMoteurJeu_demarrerMoteurCombat(pyMoteurJeu* self, PyObject* args, PyObject* kwds)
{
    //mj.demarrerMoteurCombat(2, 1, [2, 4], [5, 9], ["tuile2.png", "tuile4.png"], ["elem5.png", "elem9.png"], 4, 0.9)
    ws::DonneesMap DM;
    PyListObject *numsTuiles=NULL, *numsElements=NULL, *cheminsTuiles=NULL, *cheminsElements=NULL;
    static char* kwlist[] = {"largeurMap", "hauteurMap", "numsTuiles", "numsElements", "cheminsTuiles", "cheminsElements", "numTexBordure", "hauteurBordure", NULL};
    if(!PyArg_ParseTupleAndKeywords(args, kwds, "iiO!O!O!O!if", kwlist,
                     &DM.largeur, &DM.hauteur, &PyList_Type, &numsTuiles, &PyList_Type, &numsElements, &PyList_Type, &cheminsTuiles, &PyList_Type, &cheminsElements, &DM.numTexBordure, &DM.hauteurBordure))
        return NULL;
    
    unsigned int nbrCases = DM.largeur*DM.hauteur;
    
    for(unsigned int i=0; i<nbrCases; i++)
    {
        unsigned int tuileAct = (unsigned int)PyInt_AsLong(PyList_GetItem((PyObject*)numsTuiles, i));
        unsigned int elemAct = (unsigned int)PyInt_AsLong(PyList_GetItem((PyObject*)numsElements, i));
        string chemTuileAct = PyString_AsString(PyList_GetItem((PyObject*)cheminsTuiles, i));
        string chemElemAct = PyString_AsString(PyList_GetItem((PyObject*)cheminsElements, i));
        
        DM.numsTuiles.push_back(tuileAct);
        DM.numsElements.push_back(elemAct);
        DM.cheminsTuiles.push_back(chemTuileAct);
        DM.cheminsElements.push_back(chemElemAct);
    }
    
    self->instc->demarrerMoteurCombat(DM);
    Py_RETURN_NONE;
}

static PyObject* pyMoteurJeu_getMoteurCombat(pyMoteurJeu* self, PyObject* args)
{
    ws::MoteurCombat* MC = self->instc->getMoteurCombat();
    if (MC == NULL)
        Py_RETURN_NONE;
    pyMoteurCombat* pyMC = PyObject_New(pyMoteurCombat, &pyMoteurCombatType);
    pyMC->instc = MC;
    return (PyObject*)pyMC;
}

static PyObject* pyMoteurJeu_arreterMoteurCombat(pyMoteurJeu* self, PyObject* args)
{
    self->instc->arreterMoteurCombat();
    Py_RETURN_NONE;
}

static PyMethodDef pyMoteurJeu_methods[] = {
    {"limiterFPS", (PyCFunction)pyMoteurJeu_limiterFPS, METH_VARARGS, "Bloque le FPS a une certaine valeur"},
    {"demarrerMoteurCombat", (PyCFunction)pyMoteurJeu_demarrerMoteurCombat, METH_VARARGS | METH_KEYWORDS, "Crée le MoteurCombat"},
    {"getMoteurCombat", (PyCFunction)pyMoteurJeu_getMoteurCombat, METH_NOARGS, "Renvoie une instance du moteur de combat"},
    {"arreterMoteurCombat", (PyCFunction)pyMoteurJeu_arreterMoteurCombat, METH_NOARGS, "Arrête le moteur de combat"},
    {NULL}
};
// FIN METHODES


// DEFINITION DU TYPE:
PyTypeObject pyMoteurJeuType = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "ws.MoteurJeu",             /*tp_name*/
    sizeof(pyMoteurJeu),             /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    (destructor)pyMoteurJeu_dealloc, /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,                         /*tp_compare*/
    0,                         /*tp_repr*/
    0,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
    "Moteur de jeu Weltenschauun",           /* tp_doc */
    0,               /* tp_traverse */
    0,               /* tp_clear */
    0,               /* tp_richcompare */
    0,               /* tp_weaklistoffset */
    0,               /* tp_iter */
    0,               /* tp_iternext */
    pyMoteurJeu_methods,             /* tp_methods */
    pyMoteurJeu_members,             /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)pyMoteurJeu_init,      /* tp_init */
    0,                         /* tp_alloc */
    pyMoteurJeu_new,                 /* tp_new */
};
// FIN DEFINITION DU TYPE
