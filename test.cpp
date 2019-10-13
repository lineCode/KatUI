#include "Kat/Kat.hpp"
using namespace Kat;

int main()
{
    Form* form = CrossPlatform::FormManager::singleton->Create("lalalal",0,50,800,500);
    form->Show();
    form->Content=new Kat::Rectangle(Marign(10,10,10,50),Color(0,255,255,0.5));
    form->Content->Content = new Kat::Rectangle(Marign(30,15,10,-20),Color(255,0,255,0.5));
    CrossPlatform::FormManager::singleton->MainLoop(form);
}