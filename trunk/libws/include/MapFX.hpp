#ifndef MAP_FX_HEADER
#define MAP_FX_HEADER

#include <MapGraphique.hpp>

// EFFETS SPECIAUX:  Modifient à chaque image plusieurs variables de la map.
// Retournent true si l'effet est fini, false sinon
namespace ws
{
class MapGraphique;

class FX
{
public:
    virtual ~FX();
    virtual bool effet(MapGraphique* carte, float frameTime)=0;
};

class DeploiementElements : public FX
{
public:
    virtual bool effet(MapGraphique* carte, float frameTime);
};
} // ws

#endif // MAP_FX_HEADER

