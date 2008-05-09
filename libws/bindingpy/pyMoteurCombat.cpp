#include "pyMoteurCombat.hpp"



// ATTRIBUTS MEMBRES (ACCESSIBLES DEPUIS PYTHON):
static PyMemberDef pyMoteurCombat_members[] = {
    {NULL}         // Sentinel
};
// FIN ATTRIBUTS MEMBRES


// CONSTRUCTION / DESTRUCTION:
static PyObject* pyMoteurCombat_new(PyTypeObject* type, PyObject* args, PyObject* kwds)
{
    pyMoteurCombat *self;
    self = (pyMoteurCombat *)type->tp_alloc(type, 0);
    if (self != NULL)
        self->instc = NULL;
    return (PyObject *)self;
}


static void pyMoteurCombat_dealloc(pyMoteurCombat* self)
{
    // On ne fait pas de delete self->instc ! Le MoteurCombat étant crée par le MoteurJeu,
    // c'est au MoteurJeu de le faire.
    self->ob_type->tp_free((PyObject*)self);
}
// FIN CONSTRUCTION / DESTRUCTION


// METHODES:
static PyObject* pyMoteurCombat_centrerCurseur(pyMoteurCombat* self, PyObject* args)
{
    self->instc->centrerCurseur();
    Py_RETURN_NONE;
}

static PyObject* pyMoteurCombat_afficher(pyMoteurCombat* self, PyObject* args)
{
    self->instc->afficher();
    Py_RETURN_NONE;
}

static PyObject* pyMoteurCombat_traiterEvenements(pyMoteurCombat* self, PyObject* args)
{
    bool running = self->instc->traiterEvenements();
    if(running)
        Py_RETURN_TRUE;
    Py_RETURN_FALSE;
}

static PyObject* pyMoteurCombat_getFPS(pyMoteurCombat* self, PyObject* args)
{
    float fps = self->instc->getFPS();
    return Py_BuildValue("f", &fps);
}

static PyMethodDef pyMoteurCombat_methods[] = {
    {"centrerCurseur", (PyCFunction)pyMoteurCombat_centrerCurseur, METH_NOARGS, "Centre le curseur"},
    {"afficher", (PyCFunction)pyMoteurCombat_afficher, METH_NOARGS, "Met a jour l'affichage du jeu"},
    {"traiterEvenements", (PyCFunction)pyMoteurCombat_traiterEvenements, METH_NOARGS, "Traite les evenements venant du clavier/de la souris et effectue les operations en decoulant (scrolling, zoom...)"},
    {"getFPS", (PyCFunction)pyMoteurCombat_getFPS, METH_NOARGS, "Renvoie le FPS actuel"},
    {NULL}
};
// FIN METHODES


// DEFINITION DU TYPE:
PyTypeObject pyMoteurCombatType = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "ws.MoteurCombat",             /*tp_name*/
    sizeof(pyMoteurCombat),             /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    (destructor)pyMoteurCombat_dealloc, /*tp_dealloc*/
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
    Py_TPFLAGS_DEFAULT,     /*tp_flags*/
    "Moteur de combat Weltenschauun",           /* tp_doc */
    0,               /* tp_traverse */
    0,               /* tp_clear */
    0,               /* tp_richcompare */
    0,               /* tp_weaklistoffset */
    0,               /* tp_iter */
    0,               /* tp_iternext */
    pyMoteurCombat_methods,             /* tp_methods */
    pyMoteurCombat_members,             /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    0,      /* tp_init */
    0,                         /* tp_alloc */
    pyMoteurCombat_new,                 /* tp_new */
};
// FIN DEFINITION DU TYPE
