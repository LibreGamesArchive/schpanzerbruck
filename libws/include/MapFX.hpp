#ifndef MAP_FX_HEADER
#define MAP_FX_HEADER

#include <MapClient.hpp>

// EFFETS SPECIAUX:  Modifient à chaque image plusieurs variables de la map.
// Retournent true si l'effet est fini, false sinon
namespace ws
{
class MapClient;
class FX
{
public:
    bool effet(MapClient& carte, float frameTime);
};

class DeploiementElements : public FX
{
public:
    bool effet(MapClient& carte, float frameTime);
};
} // ws

#endif // MAP_FX_HEADER

