#include <GestionnaireImages.hpp>


namespace ws
{
ImageAnnotee::ImageAnnotee(unsigned int _etalement)
{
    etalement = _etalement;
}


GestionnaireImages::~GestionnaireImages()
{
    for(map<string, map<unsigned int, ImageAnnotee*> >::iterator objsAct=objets.begin() ; objsAct!=objets.end() ; objsAct++)
    {
        for(map<unsigned int, ImageAnnotee*>::iterator it=objsAct->second.begin(); it!=objsAct->second.end(); it++)
            if(it->second != NULL)
                delete it->second; // Delete la valeur
    }
}


bool GestionnaireImages::chargerImagesMap(unsigned int _nbrCases, vector<unsigned int> _numsTuiles, vector<unsigned int> _numsElements, vector<string> _cheminsTuiles, vector<string> _cheminsElements)
{
    // DEBUG : AFFICHAGE DES NUMEROS ET CHEMINS
    cout << "Chargement des images suivantes:" << endl << endl;
    
    bool b = true;
    for(unsigned int i=0; i<_nbrCases; i++)
    {
        cout << "Case " << i << ":" << endl;
        bool tuileOK=true, elemOK=true;
        tuileOK = chargerImage("tuiles", _numsTuiles[i], _cheminsTuiles[i]);
        //elemOK = chargerImage("elements", _numsElements[i], _cheminsElements[i]);
        b = b && tuileOK && elemOK;    // ETALEMENT !! A REGLER !
        
        cout << "\tTuile no." << _numsTuiles[i] << " : " << _cheminsTuiles[i] << " : " << tuileOK << endl;
        cout << "\tElement no." << _numsElements[i] << " : " << _cheminsElements[i] << " : " << elemOK << endl;
    }
    return b;
}


bool GestionnaireImages::chargerImage(string typeObj, unsigned int num, string chemin, unsigned int etalement)
{
    bool chargementOK = true;
    // Les numéros des images sont en HEXADECIMAL
    // num == 0 (0x00) signifie qu'il n'y a pas d'image de ce type pour cette case
    if (num != 0)
    {   // Si l'image n'a pas déjà été chargée
        //if (objets[typeObj].find(num) == objets[typeObj].end())     // UNEXPLAINED CORE DUMP !
        //{
            objets[typeObj][num] = new ImageAnnotee();    // UNEXPLAINED CORE DUMP TOO !
            if ( !((objets[typeObj][num])->LoadFromFile(chemin)) )
                chargementOK = false;
        //}
    }
    
    return chargementOK;
}

ImageAnnotee* GestionnaireImages::obtenirImage(string typeObj, unsigned int num)
{
    return objets[typeObj][num];
}
}
