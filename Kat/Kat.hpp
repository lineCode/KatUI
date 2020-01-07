/*	 _  __     _   _   _ ___ 
*	| |/ /__ _| |_| | | |_ _|
*	| ' // _` | __| | | || | 
*	| . \ (_| | |_| |_| || | 
*	|_|\_\__,_|\__|\___/|___|
*
*
*	Copyright (c) 2019 Yorkin
*
*
* 	note:
* 	In Windows,add -lgdiplus and -ld2d1 to your linker options when building with gcc.
*
*/


#ifndef KAT_UI
#define KAT_UI

#include<string>

#include"Helper.hpp"
#include"Layout.hpp"
#include"Widget.hpp"
#include"PlatformAPI.hpp"

#ifdef __linux__
#include"Linux.hpp"
#elif _WIN32
#include"Win32.hpp"
#endif

namespace Kat
{
	Form* createForm(std::string title,int x,int y,int width,int height)
	{
		auto ret = FormManager::singleton->Create(title,x,y,width,height);
		ret->layout = new Layout(0, 0, 0, 0);
		return ret;
	}
	void startUp(Form* form)
	{
		form->Show();
		FormManager::singleton->MainLoop(form);
	}
	// class SheetItem
	// {
	// public:
	// 	SheetItem(int index, Element item) :Index(index), element(&item) {}
	// 	SheetItem(int index, int size, Element item) :Index(index), Size(size), element(&item) {}
	// 	int Index = 0, Size = 1;
	// 	Element* element;
	// };

	// class Sheet :public Element
	// {
	// 	std::vector<SheetItem> childs;
	// 	int count = 0;
	// 	Direction _direction;

	// 	void ProcessMsg(Args args)
	// 	{
	// 		switch (args.msg)
	// 		{
	// 		case Message::Paint:
	// 			Paint(args.graphic);
	// 			break;
	// 		case Message::Resize:
	// 			if (_direction == Direction::Horizontal)
	// 			{
	// 				int w = this->Width / childs.size();
	// 				for (int i = 0;i < childs.size();i++)
	// 				{
	// 					childs[i].element->Top = 0;
	// 					childs[i].element->Bottom = 0;
	// 					childs[i].element->Left = i * w;
	// 					childs[i].element->Right = i * (w + 1);
	// 				}
	// 			}
	// 			else
	// 			{
	// 				int h = this->Height / childs.size();
	// 				for (int i = 0;i < childs.size();i++)
	// 				{
	// 					childs[i].element->Top = i * h;
	// 					childs[i].element->Bottom = i * (h + 1);
	// 					childs[i].element->Left = 0;
	// 					childs[i].element->Right = 0;
	// 				}
	// 			}
	// 			break;
	// 		}
	// 	}

	// public:
	// 	Sheet(Direction direction, std::initializer_list<Element> elements)
	// 	{
	// 		count = elements.size();
	// 		_direction = direction;
	// 		int i = 0;
	// 		for (auto iter = elements.begin();iter != elements.end();iter++)
	// 		{
	// 			childs.push_back(SheetItem(i, *iter));
	// 			i++;
	// 		}
	// 	}

	// 	Sheet(Direction direction, std::initializer_list<SheetItem> items)
	// 	{
	// 		count = items.size();
	// 		_direction = direction;
	// 		for (auto iter = items.begin();iter != items.end();iter++)
	// 			childs.push_back(*iter);
	// 	}

	// 	void foreach(std::function<void(Element* element)> loop)
	// 	{
	// 		if (Content != nullptr) ((Element*)Content)->foreach(loop);
	// 		for (auto iter = childs.begin();iter != childs.end();iter++)
	// 			iter->element->foreach(loop);
	// 		loop(this);
	// 	}
	// };

	 class Geometry :public Widget
	 {
	 protected:
		Color *background,*foreground;
		float *strokeWidth = new float(1);
	 public:
		 Property<Color> Background = Property<Color>(
			 [&]() { return background; },
			 [&](Color* value) { delete strokeWidth; background = value; if (Change)Change(); }
		 );

		 Property<Color> Foreground = Property<Color>(
			 [&]() { return foreground; },
			 [&](Color* value) { delete strokeWidth; foreground = value; if (Change)Change(); }
		 );

		 Property<float> StrokeWidth = Property<float>(
			 [&]() { return strokeWidth; },
			 [&](float* value) { delete strokeWidth; strokeWidth = value; if (Change)Change(); }
		 );

		 Geometry(Color background,Color foreground):background(new Color(background)),foreground(new Color(foreground)){};
		 ~Geometry()
		 {
			 delete background;
			 delete foreground;
			 delete strokeWidth;
		 }
	 };

	 class Rectangle :public Geometry
	 {
		 void Paint(Graphic* graphic)override
		 {
			 graphic->FillRectangle(*layout, *background);
			 graphic->DrawRectangle(*layout, *foreground, *strokeWidth);
		 }
	 public:
		 Rectangle(Color background, Color foreground):Geometry(background, foreground){};
	 };

	 class RoundRectangle :public Geometry
	 {
		 float* mcorner;
		 void Paint(Graphic* graphic)override
		 {
			 graphic->FillRoundRectangle(*layout,*background,*mcorner);
			 graphic->DrawRoundRectangle(*layout, *foreground, *mcorner, *strokeWidth);
		 }
	 public:
		 Property<float> corner = Property<float>(
			 [&]()				{ return mcorner; },
			 [&](float* value)	{ delete mcorner; mcorner = value; if(Change)Change(); }
		 );
		 RoundRectangle(float corner,Color background, Color foreground) :Geometry(background, foreground),mcorner(new float(corner)){};
		 ~RoundRectangle(){ delete mcorner; }
	 };

	 class Ellipse :public Geometry
	 {
		 void Paint(Graphic* graphic)override
		 {
			 graphic->FillEllipse(*layout, *background);
			 graphic->DrawEllipse(*layout, *foreground, *strokeWidth);
		 }
	 public:
		 Ellipse(Color background, Color foreground):Geometry(background, foreground){};
	 };

	 //class Text :public Widget
	 //{
		// void Paint(Graphic* graphic)override
		// {
		//	 graphic->
		// }
	 //};


	// class RectangleBorder:public Element
	// {
	// 	void Paint(CrossPlatform::Graphic* graphic)override
	// 	{
	// 		graphic->FillRectangle(Marign(0,0,0,0), color);
	// 	}
	// public:
	// 	Property<Color*> color;
	// 	RectangleBorder() = delete;
	// 	RectangleBorder(Marign marign, Color color)
	// 	{
	// 		Left = marign.left;
	// 		Top = marign.top;
	// 		Right = marign.right;
	// 		Bottom = marign.bottom;
	// 		color = Property<Color>(callback, color);
	// 	}
	// };

	// class Button:public Element
	// {
	// public:
	// 	Button(Marign marign,std::string text)
	// 	{

	// 	}
	// };

}

#endif
