#include <InterfaceCombat.hpp>


namespace ws
{

InterfaceCombat::InterfaceCombat()
{
    menuEchapON = false;
}

void InterfaceCombat::switchMenuEchap()
{
    menuEchapON = !menuEchapON;
}

void InterfaceCombat::GL_MenuEchapPourSelection()
{
    
}

void InterfaceCombat::GL_MenuEchap()
{
    
}

void InterfaceCombat::GL_DessinPourSelection()
{
    if(menuEchapON)
        GL_MenuEchapPourSelection();
}

void InterfaceCombat::GL_Dessin()
{
    if(menuEchapON)
        GL_MenuEchap();
}

}
