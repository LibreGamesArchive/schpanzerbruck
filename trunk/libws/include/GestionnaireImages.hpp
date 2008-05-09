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
    int etalement;

    ImageAnnotee(int _etalement=1);
};


class GestionnaireImages
{
private:
    map<char*, map<int, ImageAnnotee*> > objets;

public:
    ~GestionnaireImages();
    
    bool chargerImage(char* typeObj, int num, char* chemin, int etalement=1);

    bool chargerImagesMap(unsigned int _nbrCases, vector<unsigned int> _numsTuiles, vector<unsigned int> _numsElements, vector<char*> _cheminsTuiles, vector<char*> _cheminsElements);

    sf::Image* obtenirImage(char* typeObj, int num);
};

}

#endif
