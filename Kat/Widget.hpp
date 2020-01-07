#ifndef KAT_WIDGET
#define KAT_WIDGET
#include<list>
#include"Helper.hpp"
#include"Layout.hpp"
namespace Kat
{
	class Form;
    class Widget
	{
		friend Form;
		friend Layout;
	protected:
		Event Change;
		std::list<Widget*> childs;
		Widget* mparent = nullptr;
		Layout* mlayout = nullptr;
		Widget* selected  = nullptr; //最近一次被点击的子部件
		Widget* highlight = nullptr; //最近一次被高亮的子部件
		virtual void foreach(std::function<void(Widget*)> loop)
		{
			loop(this);
			for(auto iter=childs.begin();iter!=childs.end();iter++)
			{
				(*iter)->foreach(loop);
			}
		}
	public:

		Property<Layout> layout = Property<Layout>(
			[&](){ return mlayout; },
			[&](Layout* value){ if(mlayout!=nullptr)mlayout->owner = nullptr; value->owner = this; mlayout=value; }
		);

		Property<Widget> parent = Property<Widget>(
			[&]() { return mparent; },
			[&](Widget* value) { if(mparent!=nullptr)mparent->childs.remove(this); value->childs.push_back(this); mparent = value; }
		);

		virtual void add(Layout* layout, Widget* widget)
		{
			widget->layout = layout;
			widget->parent = this;
		}

		std::function<void()> MouseDown,MouseUp,MouseMove;
		std::function<void()> Resize,Click,MouseEnter,MouseExit,Selected,UnSelected;

		virtual void Paint(Graphic* graphic) {};

		virtual void ProcessMsg(Message msg)
		{
			switch (msg.content)
			{
			case Message::Paint:  	
				Paint(msg.graphic); 
				break;
			case Message::Resize: 	
				layout->checkXAxis();
				layout->checkYAxis();
				if(Resize)Resize(); 
				break;
			case Message::MouseDown:	
				//call(MouseDown,Point(args.mouse.x-(int)Left,args.mouse.y-(int)Top),args.button);
			break;
			case Message::MouseMove:
				if(parent!=nullptr)
				if(parent->highlight!=nullptr)
				{
					Widget* tmp = parent->highlight;
					parent->highlight=this;
					tmp->MouseExit();
					MouseEnter();
				}
				//MouseMove();
				break;
			case Message::MouseUp:
				if(parent != nullptr)
				if(parent->selected)
				{
					Widget* tmp = parent->selected;
					parent->selected=this;
					tmp->UnSelected();
					Selected();
				}
				//MouseUp(Point(args.mouse.x-(int)Left,args.mouse.y-(int)Top),args.button);
				break;
			case Message::MouseEnter:
				MouseEnter();
				break;
			case Message::MouseExit:
				MouseExit();
				break;
			}
		}
	};

	class Row :public Widget
	{
	public:
		virtual void add(Layout* layout, Widget* widget)override
		{
			auto container = new Widget();
			container->add(layout,widget);
			container->layout = new Layout(0,0,0,0);
			container->parent = this;
			ProcessMsg(Message::Resize);
		}

		virtual void ProcessMsg(Message msg)override
		{
			if(layout==nullptr)return;
			if (msg.content == Message::Resize)
			{
				Rect rect = *layout;
				float itemH = rect.h / childs.size();
				int i = 0;
				for (auto iter = childs.begin(); iter != childs.end(); iter++)
				{
					(*iter)->layout->Top = itemH * i;
					(*iter)->layout->Bottom = itemH * (childs.size() - i - 1);
					i++;
				}
			}
			else
				Widget::ProcessMsg(msg);
		}
	};

	class Columns :public Widget
	{
	public:
		virtual void add(Layout* layout, Widget* widget)override
		{
			auto container = new Widget();
			container->add(layout, widget);
			container->layout = new Layout(0, 0, 0, 0);
			container->parent = this;
			//ProcessMsg(Message::Resize);
		}

		virtual void ProcessMsg(Message msg)override
		{
			//if (layout == nullptr)return;
			if (msg.content == Message::Resize)
			{
				Rect rect = *layout;
				double itemW = (double)rect.w / (double)childs.size();
				int i = 0;
				for (auto iter = childs.begin(); iter != childs.end(); iter++)
				{
					(*iter)->layout->Left = itemW * i;
					(*iter)->layout->Right = itemW * (childs.size() - i - 1);
					i++;
				}
			}
			else
				Widget::ProcessMsg(msg);
		}
	};

		//***************************************************************************************************************************

		// virtual void foreach(std::function<void(Element* element)> loop)
		// {
		// 	loop(this);
		// 	if (Content != nullptr)
		// 		Content->foreach(loop);
		// }
		
		// Marign GetRelativeMarign(Element* root)
		// {
		// 	Marign marign;
		// 	Element* tmp = this;
		// 	while (tmp != root)
		// 	{
		// 		marign.left += tmp->Left;
		// 		marign.top += tmp->Top;
		// 		marign.right += tmp->Right;
		// 		marign.bottom += tmp->Bottom;
		// 		tmp = tmp->parent;
		// 	}
		// 	return marign;
		// }

}
#endif