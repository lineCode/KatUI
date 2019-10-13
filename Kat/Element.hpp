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

    class Element
	{
		//friend Form;
	protected:
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
		Element* parent;
		Element* owner;
		ChangeEvent OnChange;
		ChangeEvent callback = [&] {if (OnChange)OnChange();};

		//***************************override in derived classes***********************************************************
		virtual void Paint(CrossPlatform::Graphic* graphic) {};
	public:
		virtual void ProcessMsg(Args args)
		{
			switch (args.msg)
			{
			case Message::Paint:
				Paint(args.graphic);
				break;
			}
		};
		//***************************Events**************************************************************************************
		ClickEvent     Click;
		MouseEnterEvent OnMouseEnter;
		MouseExitEvent  OnMouseExit;
		MouseDownEvent  OnMouseDown;
		MouseUpEvent    OnMouseUp;
		GotFocusEvent   OnGotFocus;
		LostFocusEvent  OnLostFocus;
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

		void foreach(std::function<void(Element* element)> loop)
		{
			loop(this);
			if (Content != nullptr)
				((Element*)Content)->foreach(loop);
		}

	};

}
#endif