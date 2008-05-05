#include "../include/MapFX.hpp"


FX::~FX()
{
    // Rien à faire
}


bool DeploiementElements::effet(MapClient& map, float frameTime)
{
    if (map.inclinaisonElements < 70)
    {
        map.inclinaisonElements += 130 * frameTime;
        if (map.inclinaisonElements > 70)
        {
            map.inclinaisonElements = 70;
            return true;
        }
    }
    return false;
}
