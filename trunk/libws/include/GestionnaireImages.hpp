#ifndef GESTIONNAIRE_IMAGES_HEADER
#define GESTIONNAIRE_IMAGES_HEADER

#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <vector>

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
    map<string, map<int, ImageAnnotee> > objets;

public:
    bool chargerImage(string typeObj, int num, char* chemin, int etalement=1);

    bool chargerImagesMap(int _nbrCases, vector<unsigned int> _numsTuiles, vector<unsigned int> _numsElements, vector<char*> _cheminsTuiles, vector<char*> _cheminsElements);

    sf::Image* obtenirImage(string typeObj, int num);
};

}

#endif
