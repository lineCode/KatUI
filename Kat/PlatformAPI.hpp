#ifndef KAT_INTERFACE
#define KAT_INTERFACE
#include<list>
#include<vector>
#include<string>
#include"Helper.hpp"
#include"Layout.hpp"
#include"Widget.hpp"
namespace Kat
{
	class Form:public Widget
	{
		friend Widget;
		// Widget* hightlight = nullptr;
		// Widget* focus = nullptr;
	protected:
		Graphic* graphic = nullptr;
		Color* background = new Color(0xFFFFFF);
		virtual void foreach(std::function<void(Widget*)> loop)override
		{
			for (auto iter = childs.begin(); iter != childs.end(); iter++)
			{
				(*iter)->foreach(loop);
			}
		}
	public:
		Property<Color> Background = Property<Color>(
			[&]() { return background;  },
			[&](Color* value) { background = value; /*printf("background:%06X\n",*background);*/}
		);

		virtual void Show()=0;
		virtual void Hide()=0;
		virtual void Minimize()=0;
		virtual void Maximize()=0;
		virtual void ProcessMsg(Message msg)override;
	};

#ifdef __linux__
        enum GraphicBackends
		{
			Xlib = 0
        }Render;
#elif _WIN32
        enum GraphicBackends
		{
			D2D = 0,
			GDI = 1
        }Render;
#endif
		class Font
		{
			std::string mname;
			bool munderline = false;
			bool mbold = false;
			bool m;
		//public:
		};

		class Timer
		{
		public:
			Event Tick;
			int interval = 0;
			virtual void Enable()=0;
			virtual void UnEnable()=0;
		};
		
		class Graphic;
		class FormManager
		{
		public:
			static FormManager* singleton;
			//Point mouse;
			std::list<Form*> forms;
			std::vector<Timer*> timers;
			virtual Kat::Form* Create(std::string title, int x, int y, int width, int height) = 0;
			virtual void Delete(Kat::Form* form) = 0;
            virtual void MainLoop(Kat::Form* form)=0;
            virtual Rect GetFormRect(Kat::Form* form)=0;
            virtual void SetFormRect(Kat::Form* form,Rect rect)=0;
			virtual Graphic* CreateGraphic(Kat::Form* form)=0;
			virtual Timer* CreateTimer(int interval) = 0;
		};

		Layout::operator Kat::Rect()
		{
			Layout p = *this;
			Rect ret = Rect(Left,Top,Bottom,Right);
			while (p.owner->parent != nullptr)
			{
				p = p.owner->parent->layout;
				ret.x += (float)p.Left;
				ret.y += (float)p.Top;
				ret.w += (float)p.Right;
				ret.h += (float)p.Bottom;
			};
			Rect rect = FormManager::singleton->GetFormRect((Form*)p.owner);
			ret.w = rect.w - ret.x - ret.w;
			ret.h = rect.h - ret.y - ret.h;
			return ret;
		}

		Squre::Squre(float dist)
		{
			this->distance = dist;
			auto setter = [&](float* value) { distance = *value; if(Change)Change(); };
			auto xgetter = [&]()
			{
				Rect rect = *this->owner->parent->layout;
				if (rect.h < rect.w)
					return new float((rect.w - rect.h) / 2 + distance);
				else
					return new float(distance);
			};
			auto ygetter = [&]()
			{
				Rect rect = *this->owner->parent->layout;
				if (rect.h > rect.w)
					return new float((rect.h - rect.w) / 2 + distance);
				else
					return new float(distance);
			};

			Left = Property<float>(xgetter, setter);
			Top = Property<float>(ygetter, setter);
			Right = Property<float>(xgetter, setter);
			Bottom = Property<float>(ygetter, setter);
		}

		//Squre::operator Kat::Rect()
		//{
		//	Layout p = *this;
		//	Rect ret = Rect(left, top, bottom, right);
		//	while (p.owner->parent != nullptr)
		//	{
		//		p = p.owner->parent->layout;
		//		ret.x += p.left;
		//		ret.y += p.top;
		//		ret.w += p.right;
		//		ret.h += p.bottom;
		//	};
		//	Rect rect = FormManager::singleton->GetFormRect((Form*)p.owner);
		//	ret.w = rect.w - ret.x - ret.w;
		//	ret.h = rect.h - ret.y - ret.h;
		//	return ret;
		//}

		class Graphic
		{
		protected:
			int width = 0,height = 0;
		public:
			// Marign offset;
			virtual void Begin() = 0;
			virtual void Clear(Color color) = 0;
			virtual void FillRectangle(Rect rect, Color color) = 0;
			virtual void DrawRectangle(Rect rect, Color color,float strokeWidth=1.0) = 0;
			virtual void FillRoundRectangle(Rect rect, Color color, float corner) = 0;
			virtual void DrawRoundRectangle(Rect rect, Color color, float corner, float strokeWidth = 1.0) = 0;
			virtual void DrawEllipse(Rect rect, Color color,float strokeWidth=1.0) = 0;
			virtual void FillEllipse(Rect rect, Color color) = 0;
			virtual void DrawString(Rect rect,Color color,std::string text) = 0;
			virtual void Resize(int width, int height) = 0;
			virtual void End() = 0;
			//virtual void DrawLine(Point begin, Point end, int Stroke, Color color) = 0;
			//virtual void DrawArc(Point pos, int startAngle, int endAngle) = 0;
			//virtual void FillPie(Point pos, int startAngle, int endAngle) = 0;
			//virtual void DrawImage(Rect rect, IImageHander image) = 0;
			//virtual IImageHander CreateImage(std::string path) = 0;

		};

		void Form::ProcessMsg(Message msg)
		{
			Rect tmp;
			auto processer = [&](Widget* child) { child->ProcessMsg(msg); };
			switch (msg.content)
			{
			case Message::Paint:
				msg.graphic = this->graphic;
				this->graphic->Begin();
				this->graphic->Clear(Background);
				this->foreach(processer);
				this->graphic->End();
				break;
			case Message::Resize:
				tmp = FormManager::singleton->GetFormRect(this);
				this->graphic->Resize(tmp.w, tmp.h);
				this->foreach(processer);
				break;
			case Message::MouseEnter:
			case Message::MouseMove:
			case Message::MouseDown:
			case Message::MouseUp:
			case Message::MouseExit:
				this->foreach(processer);
				break;
			case Message::Destory:
				this->foreach(processer);
				FormManager::singleton->Delete(this);
				break;
			}
		}
}


#endif
