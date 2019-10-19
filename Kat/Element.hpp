#ifndef KAT_ELEMENT
#define KAT_ELEMENT
#include"Helper.hpp"
namespace Kat
{
    namespace CrossPlatform
    {
        class Graphic;
    }
	class Form;
	
	typedef std::function<void()> Event;
	typedef std::function<void(Point,ButtonType)> MouseEvent;

    class Element
	{
		//friend Form;
	protected:
		Element* parent = nullptr;
		Element* owner = nullptr;
		
		Element* selected = nullptr;
		Element* highlight = nullptr;

		ChangeEvent OnChange;
		ChangeEvent callback = [&] {if (OnChange)OnChange();};

	public:
		Style style;
	    //***************************Events**************************************************************************************
		MouseEvent MouseDown;
		MouseEvent MouseUp;
		MouseEvent MouseMove;
		Event Resize;
		Event Click;
		Event MouseEnter;
		Event MouseExit;
		Event Selected;
		Event UnSelected;

		//***************************override in derived classes***********************************************************
		virtual void Paint(CrossPlatform::Graphic* graphic) {};

		virtual void ProcessMsg(Args args)
		{
			switch (args.msg)
			{
			case Message::Paint:
				Paint(args.graphic);
				break;
			case Message::Resize:
				Resize();
				break;
			case Message::MouseMove:
				if(owner->highlight!=nullptr)
				{
					Element* tmp = owner->highlight;
					owner->highlight=this;
					tmp->MouseExit();
					MouseEnter();
				}
				MouseMove(Point(args.mouse.x-(int)Left,args.mouse.y-(int)Top),args.button);
				break;
			case Message::MouseDown:
				MouseDown(Point(args.mouse.x-(int)Left,args.mouse.y-(int)Top),args.button);
				break;
			case Message::MouseUp:
				if(parent->selected)
				{
					Element* tmp = parent->selected;
					parent->selected=this;
					tmp->UnSelected();
					Selected();
				}
				MouseUp(Point(args.mouse.x-(int)Left,args.mouse.y-(int)Top),args.button);
				break;
			case Message::MouseEnter:
				MouseEnter();
				break;
			case Message::MouseExit:
				MouseExit();
				break;
			}
		};
		//***************************Location**************************************************************************************
		Property<int>       Left = Property<int>(callback, 0);
		Property<int>       Top = Property<int>(callback, 0);
		Property<int>       Right = Property<int>(callback, 0);
		Property<int>       Bottom = Property<int>(callback, 0);
		Property<int>       MaxWidth = Property<int>(callback, 0);
		Property<int>       MinWidth = Property<int>(callback, 0);
		Property<int>       MaxHeight = Property<int>(callback, 0);
		Property<int>       MinHeight = Property<int>(callback, 0);
		Property<int>       Width = Property<int>([&] {return (int)Right - (int)Left;}, [&](int value) {Width = value;}, 0);
		Property<int>       Height = Property<int>([&] {return (int)Bottom - (int)Top;}, [&](int value) {Height = value;}, 0);
		Property<Element*>  Content = Property<Element*>([&] {return Content;}, [&](Element* e) {Content = e;e->parent = this;e->foreach([&](Element* element) {element->owner = this->owner;});});
		//***************************************************************************************************************************

		virtual void foreach(std::function<void(Element* element)> loop)
		{
			loop(this);
			if (Content != nullptr)
				Content->foreach(loop);
		}
		
		Marign GetRelativeMarign(Element* root)
		{
			Marign marign;
			Element* tmp = this;
			while (tmp != root)
			{
				marign.left += tmp->Left;
				marign.top += tmp->Top;
				marign.right += tmp->Right;
				marign.bottom += tmp->Bottom;
				tmp = tmp->parent;
			}
			return marign;
		}

	};

}
#endif