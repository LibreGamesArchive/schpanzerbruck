#ifndef GESTIONNAIRE_IMAGES_HEADER
#define GESTIONNAIRE_IMAGES_HEADER

#include <SFML/Graphics.hpp>
#include <map>
#include <string>

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
    std::map<std::string, std::map<int, ImageAnnotee> > objets;

public:
    bool chargerImage(std::string typeObj, int num, char* chemin, int etalement=1);

    bool chargerImagesMap(int _nbrCases, int* _numsTuiles, int* _numsElements, char** _cheminsTuiles, char** _cheminsElements);

    sf::Image* obtenirImage(std::string typeObj, int num);
};

}

#endif