#ifndef GESTIONNAIRE_IMAGES_HEADER
#define GESTIONNAIRE_IMAGES_HEADER

#include <SFML/Graphics.hpp>
#include <map>
#include <vector>
#include <iostream>
#include <string>

using namespace std;

namespace ws
{

class ImageAnnotee : public sf::Image
{
public:
    unsigned int etalement;

    ImageAnnotee(unsigned int _etalement=1);
};


class GestionnaireImages
{
private:
    map<string, map<unsigned int, ImageAnnotee*> > objets;

public:
    ~GestionnaireImages();
    
    bool chargerImage(string typeObj, unsigned int num, string chemin, unsigned int etalement=1);

    bool chargerImagesMap(unsigned int _nbrCases, vector<unsigned int> _numsTuiles, vector<unsigned int> _numsElements, vector<string> _cheminsTuiles, vector<string> _cheminsElements);

    ImageAnnotee* obtenirImage(string typeObj, unsigned int num);
    
    const map<unsigned int, ImageAnnotee*>& operator[](string typeObj);
};

}

#endif
