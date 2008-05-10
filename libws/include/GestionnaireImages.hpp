#ifndef GESTIONNAIRE_IMAGES_HEADER
#define GESTIONNAIRE_IMAGES_HEADER

#include <SFML/Graphics.hpp>
#include <map>
#include <vector>
#include <iostream>

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
    map<char*, map<unsigned int, ImageAnnotee*> > objets;

public:
    ~GestionnaireImages();
    
    bool chargerImage(char* typeObj, unsigned int num, char* chemin, unsigned int etalement=1);

    bool chargerImagesMap(unsigned int _nbrCases, vector<unsigned int> _numsTuiles, vector<unsigned int> _numsElements, vector<char*> _cheminsTuiles, vector<char*> _cheminsElements);

    ImageAnnotee* obtenirImage(char* typeObj, unsigned int num);
};

}

#endif
