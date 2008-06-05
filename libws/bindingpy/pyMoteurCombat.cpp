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

static PyObject* pyMoteurCombat_evenementsEtAffichage(pyMoteurCombat* self, PyObject* args)
{
    unsigned int whatHappens = self->instc->evenementsEtAffichage();
    return Py_BuildValue("i", whatHappens);
}

static PyObject* pyMoteurCombat_getFPS(pyMoteurCombat* self, PyObject* args)
{
    float fps = self->instc->getFPS();
    return Py_BuildValue("f", fps);
}

static PyObject* pyMoteurCombat_selectMapActuelle(pyMoteurCombat* self, PyObject* args)
{
    int* select = self->instc->selectMapActuelle();
    return Py_BuildValue("(ii)", select[0], select[1]);
}

static PyObject* pyMoteurCombat_maitrisesChoisies(pyMoteurCombat* self, PyObject* args)
{
    int mtrChoisies[3], gradesChoisis[3];
    self->instc->maitrisesChoisies(mtrChoisies, gradesChoisis);
    return Py_BuildValue("[(ii)(ii)(ii)]", mtrChoisies[0], gradesChoisis[0], mtrChoisies[1], gradesChoisis[1], mtrChoisies[2], gradesChoisis[2]);
}

static PyObject* pyMoteurCombat_setInfosDsBarre(pyMoteurCombat* self, PyObject* args)
{
    PyObject *tuile=NULL, *element=NULL, *perso=NULL;
    if( !PyArg_ParseTuple(args, "OOO", &tuile, &element, &perso) )
        return NULL;
    
    self->instc->setInfosDsBarre(PyString_AsString(tuile), PyString_AsString(element), PyString_AsString(perso));
    
    Py_RETURN_NONE;
}

static PyObject* pyMoteurCombat_setChrono(pyMoteurCombat* self, PyObject* args)
{
    float temps;
    if( !PyArg_ParseTuple(args, "f", &temps) )
        return NULL;
    
    self->instc->setChrono(temps);
    
    Py_RETURN_NONE;
}

static PyObject* pyMoteurCombat_setPersoCourant(pyMoteurCombat* self, PyObject* args)
{
    PyObject* nomPerso=NULL;
    float VIE = 0, FTG = 0;
    int numPerso=-1;
    if( !PyArg_ParseTuple(args, "iOff", &numPerso, &nomPerso, &VIE, &FTG) )
        return NULL;
    
    self->instc->setPersoCourant(numPerso, PyString_AsString(nomPerso), VIE, FTG);
    
    Py_RETURN_NONE;
}

static PyObject* pyMoteurCombat_modifVieFtgPersoCourant(pyMoteurCombat* self, PyObject* args)
{
    float VIE=0, FTG=0;
    if( !PyArg_ParseTuple(args, "ii", &VIE, &FTG) )
        return NULL;
    
    self->instc->modifVieFtgPersoCourant(VIE, FTG);
    
    Py_RETURN_NONE;
}

static PyObject* pyMoteurCombat_setMaitrisesAffichees(pyMoteurCombat* self, PyObject* args)
{
    PyObject* pyListeMtr=NULL;
    if( !PyArg_ParseTuple(args, "O", &pyListeMtr) )
        return NULL;
    
    vector<string> listeMtr;
    vector<int> listeGrades;
    for(int i=0; i<PyList_Size(pyListeMtr); i++)
    {
        PyObject* nomMaitrise=NULL; int gradeMaitrise;
        if( !PyArg_ParseTuple(PyList_GetItem(pyListeMtr, i), "Oi", &nomMaitrise, &gradeMaitrise) )
            return NULL;
        listeMtr.push_back(PyString_AsString(nomMaitrise));
        listeGrades.push_back(gradeMaitrise);
    }
    
    self->instc->setMaitrisesAffichees(listeMtr, listeGrades);
    
    Py_RETURN_NONE;
}

static PyObject* pyMoteurCombat_setListePersos(pyMoteurCombat* self, PyObject* args)
{
    PyObject* pyListePersos=NULL;
    if( !PyArg_ParseTuple(args, "O", &pyListePersos) )
        return NULL;
    
    vector<ws::PersoGraphique> listePG;
    for(int i=0; i<PyList_Size(pyListePersos); i++)
    {
        ws::PersoGraphique persoAct;
        if( !PyArg_ParseTuple(PyList_GetItem(pyListePersos, i), "ii(iii)", &persoAct.pos, &persoAct.arme, &persoAct.clr.R, &persoAct.clr.V, &persoAct.clr.B) )
            return NULL;
        listePG.push_back(persoAct);
    }
    
    self->instc->setListePersos(listePG);
    
    Py_RETURN_NONE;
}

static PyObject* pyMoteurCombat_chargerImagesPersos(pyMoteurCombat* self, PyObject* args)
{
    PyObject *pyCheminFantome=NULL, *pyCheminHalo=NULL, *pyDictCheminsArmes=NULL;
    if( !PyArg_ParseTuple(args, "OOO", &pyCheminFantome, &pyCheminHalo, &pyDictCheminsArmes) )
        return NULL;
    
    map<int, string> cheminsArmes;
    PyObject *key, *value;
    int pos = 0;
    while(PyDict_Next(pyDictCheminsArmes, &pos, &key, &value))
        cheminsArmes[PyInt_AsLong(key)] = PyString_AsString(value);
    
    self->instc->chargerImagesPersos(PyString_AsString(pyCheminFantome), PyString_AsString(pyCheminHalo), cheminsArmes);
    
    Py_RETURN_NONE;
}

static PyObject* pyMoteurCombat_deplacerPersoCourant(pyMoteurCombat* self, PyObject* args)
{
    PyObject* pyChemin=NULL;
    if( !PyArg_ParseTuple(args, "O", &pyChemin) )
        return NULL;
    
    list<int> chemin;
    for(int i=0; i<PyList_Size(pyChemin); i++)
        chemin.push_back(PyInt_AsLong(PyList_GetItem(pyChemin, i)));
    
    self->instc->deplacerPersoCourant(chemin);
    
    Py_RETURN_NONE;
}

static PyObject* pyMoteurCombat_setCasesPossibles(pyMoteurCombat* self, PyObject* args)
{
    PyObject* pyCases=NULL;
    if( !PyArg_ParseTuple(args, "O", &pyCases) )
        return NULL;
    
    vector<int> casesPossibles;
    for(int i=0; i<PyList_Size(pyCases); i++)
        casesPossibles.push_back(PyInt_AsLong(PyList_GetItem(pyCases, i)));
    
    self->instc->setCasesPossibles(casesPossibles);
    
    Py_RETURN_NONE;
}

static PyMethodDef pyMoteurCombat_methods[] = {
    {"centrerCurseur", (PyCFunction)pyMoteurCombat_centrerCurseur, METH_NOARGS, "Centre le curseur"},
    {"evenementsEtAffichage", (PyCFunction)pyMoteurCombat_evenementsEtAffichage, METH_NOARGS, "Traite les evenements venant du clavier/de la souris et effectue les operations en decoulant (scrolling, zoom...)"},
    {"getFPS", (PyCFunction)pyMoteurCombat_getFPS, METH_NOARGS, "Renvoie le FPS actuel"},
    {"selectMapActuelle", (PyCFunction)pyMoteurCombat_selectMapActuelle, METH_NOARGS, "Renvoie la sélection actuellement faite sur la Map"},
    {"maitrisesChoisies", (PyCFunction)pyMoteurCombat_maitrisesChoisies, METH_NOARGS, "Renvoie la liste des 3 maîtrises actuellement choisies"},
    {"setInfosDsBarre", (PyCFunction)pyMoteurCombat_setInfosDsBarre, METH_VARARGS, "Met à jour les infos de la BarreInfos"},
    {"setChrono", (PyCFunction)pyMoteurCombat_setChrono, METH_VARARGS, "Met à jour le chronomètre"},
    {"setPersoCourant", (PyCFunction)pyMoteurCombat_setPersoCourant, METH_VARARGS, "Met à jour les infos succintes sur le perso en train de jouer. Signale le début d'un tour"},
    {"modifVieFtgPersoCourant", (PyCFunction)pyMoteurCombat_modifVieFtgPersoCourant, METH_VARARGS, "Modifie la vie et la fatique du perso en train de jouer"},
    {"setMaitrisesAffichees", (PyCFunction)pyMoteurCombat_setMaitrisesAffichees, METH_VARARGS, "Met à jour la liste des maîtrises affichées et donc sélectionnables"},
    {"setListePersos", (PyCFunction)pyMoteurCombat_setListePersos, METH_VARARGS, "Indique au moteur la liste des cases sur lesquelles il y a un personnage, avec la couleur de l'équipe et le type d'arme associées au chaque perso"},
    {"chargerImagesPersos", (PyCFunction)pyMoteurCombat_chargerImagesPersos, METH_VARARGS, "Spécifie les images liées aux persos qu'il faut charger"},
    {"deplacerPersoCourant", (PyCFunction)pyMoteurCombat_deplacerPersoCourant, METH_VARARGS, "Lance le déplacement d'un personnage"},
    {"setCasesPossibles", (PyCFunction)pyMoteurCombat_setCasesPossibles, METH_VARARGS, "Définit les cases accessibles pour un déplacement ou une attaque"},
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
