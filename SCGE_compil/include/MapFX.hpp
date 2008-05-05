#ifndef MAP_FX_HEADER
#define MAP_FX_HEADER

#include "MapClient.hpp"


// EFFETS SPECIAUX:  Modifient à chaque image plusieurs variables de la map.
// Retournent true si l'effet est fini, false sinon
class FX
{
public:
    virtual ~FX();
    virtual bool effet(MapClient& map, float frameTime);
};

class DeploiementElements : public FX
{
public:
    virtual bool effet(MapClient& map, float frameTime);
};

#endif
