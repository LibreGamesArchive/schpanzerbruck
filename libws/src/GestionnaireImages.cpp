#include <GestionnaireImages.hpp>


namespace ws
{
ImageAnnotee::ImageAnnotee(int _etalement)
{
    etalement = _etalement;
}


bool GestionnaireImages::chargerImagesMap(int _nbrCases, vector<unsigned int> _numsTuiles, vector<unsigned int> _numsElements, vector<char*> _cheminsTuiles, vector<char*> _cheminsElements)
{
    bool b = false;
    for(int i=0; i<_nbrCases; i++)
    {
        b = b && chargerImage("tuiles", _numsTuiles[i], _cheminsTuiles[i]) && chargerImage("elements", _numsElements[i], _cheminsElements[i]);    // ETALEMENT !! A REGLER !
    }
    return b;
}


bool GestionnaireImages::chargerImage(string typeObj, int num, char* chemin, int etalement)
{
    bool chargementOK = true;
    // Les numéros des images sont en HEXADECIMAL
    // num == 0 (0x00) signifie qu'il n'y a pas d'image de ce type pour cette case
    if (num != 0)
        if (objets[typeObj].find(num) == objets[typeObj].end()) // Si l'image n'a pas déjà été chargée
        {
            objets[typeObj][num] = ImageAnnotee();
            if (!objets[typeObj][num].LoadFromFile(chemin))
                chargementOK = false;
        }

    return chargementOK;
}

sf::Image* GestionnaireImages::obtenirImage(string typeObj, int num)
{
    if (objets.find(typeObj) != objets.end())
        if (objets[typeObj].find(num) != objets[typeObj].end())
            return &(objets[typeObj][num]);
    return NULL;
}
}

