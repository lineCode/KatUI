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
#pragma once
#include<iostream>
#include<functional>
#include<map>
#include<list>
#include<stdexcept>
#include<string>
#include<vector>
#include<memory>

#include"Helper.hpp"
#include"Element.hpp"
#include"PlatformAPI.hpp"

#ifdef __linux__
#include"Linux.hpp"
#elif _WIN32
#include"Win32.hpp"
#endif

#define print(x) std::cout<<x<<std::endl;

namespace Kat
{
	class SheetItem
	{
	public:
		SheetItem(int index, Element item) :Index(index), element(&item) {}
		SheetItem(int index, int size, Element item) :Index(index), Size(size), element(&item) {}
		int Index = 0, Size = 1;
		Element* element;
	};

	class Sheet :public Element
	{
		std::vector<SheetItem> childs;
		int count = 0;
		Direction _direction;

		void ProcessMsg(Args args)
		{
			switch (args.msg)
			{
			case Message::Paint:
				Paint(args.graphic);
				break;
			case Message::Resize:
				if (_direction == Direction::Horizontal)
				{
					int w = this->Width / childs.size();
					for (int i = 0;i < childs.size();i++)
					{
						childs[i].element->Top = 0;
						childs[i].element->Bottom = 0;
						childs[i].element->Left = i * w;
						childs[i].element->Right = i * (w + 1);
					}
				}
				else
				{
					int h = this->Height / childs.size();
					for (int i = 0;i < childs.size();i++)
					{
						childs[i].element->Top = i * h;
						childs[i].element->Bottom = i * (h + 1);
						childs[i].element->Left = 0;
						childs[i].element->Right = 0;
					}
				}
				break;
			}
		}

	public:
		Sheet(Direction direction, std::initializer_list<Element> elements)
		{
			count = elements.size();
			_direction = direction;
			int i = 0;
			for (auto iter = elements.begin();iter != elements.end();iter++)
			{
				childs.push_back(SheetItem(i, *iter));
				i++;
			}
		}

		Sheet(Direction direction, std::initializer_list<SheetItem> items)
		{
			count = items.size();
			_direction = direction;
			for (auto iter = items.begin();iter != items.end();iter++)
				childs.push_back(*iter);
		}

		void foreach(std::function<void(Element* element)> loop)
		{
			if (Content != nullptr) ((Element*)Content)->foreach(loop);
			for (auto iter = childs.begin();iter != childs.end();iter++)
				iter->element->foreach(loop);
			loop(this);
		}
	};

	class Rectangle :public Element
	{
		void Paint(CrossPlatform::Graphic* graphic)override
		{
			auto marign = GetRelativeMarign(owner);
            Rect size = CrossPlatform::FormManager::singleton->GetFormRect((Form*)owner);
			Rect rect = Rect(marign.left, marign.top, size.right - marign.right, size.bottom - marign.bottom);
			Color color = Background;
			graphic->FillRectangle(rect, &color);
		}
	public:
		Property<Color> Background;
		Rectangle() = delete;
		Rectangle(Marign rect, Color background)
		{
			Left = rect.left;
			Top = rect.top;
			Right = rect.right;
			Bottom = rect.bottom;
			Background = Property<Color>(callback, background);
		}
	};
	Rectangle* CreateRectangle(Marign rect, Color background)
	{
		Rectangle* tmp = new Rectangle(rect, background);
		return tmp;
	}

	void StartUp(Kat::Form& form)
	{
		form.Show();
		CrossPlatform::FormManager::singleton->MainLoop(&form);
	}

}

#endif
