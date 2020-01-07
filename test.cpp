#include "Kat/Kat.hpp"
#include<stdio.h>
using namespace Kat;
#include<iostream>
int main()
{

    Form* form = createForm("lalalal",0,50,800,500);

	auto r1 = new Kat::Ellipse(Color(0, 1, 1, 0), 0x0000FF);
	form->add(new Layout(400, 0, 0, 0), new Kat::Ellipse(Color(0, 0, 0, 0.4F), 0x00FF00));

	auto l = new Layout(100,100,100,100);
	auto b = new Kat::RoundRectangle(10,Color(0, 1, 1, 0), 0xFF0000);
	b->add(l,r1);
	//form->add(new Layout(60),b);
	form->add(new Layout(0),b);

    startUp(form);

}