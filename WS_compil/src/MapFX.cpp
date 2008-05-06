#include <MapFX.hpp>


namespace ws
{
bool FX::effet(MapClient& carte, float frameTime)
{
    return true;
}


bool DeploiementElements::effet(MapClient& carte, float frameTime)
{
    if (carte.inclinaisonElements < 70)
    {
        carte.inclinaisonElements += 130 * frameTime;
        if (carte.inclinaisonElements > 70)
        {
            carte.inclinaisonElements = 70;
            return true;
        }
    }
    return false;
}
}

