#ifndef KAT_INTERFACE
#define KAT_INTERFACE
#include<list>
#include"Helper.hpp"
#include"Element.hpp"
namespace Kat
{
	class Form:public Element
	{
	public:
		virtual void Show()=0;
		virtual void Hide()=0;
		virtual void Minimize()=0;
		virtual void Maximize()=0;
	};

    namespace CrossPlatform
    {

#ifdef __linux__
        enum GraphicBackends
		{
			Cario = 0
        }Render;
#elif _WIN32
        enum GraphicBackends
		{
			D2D = 0,
			GDI = 1
        }Render;
#endif

		class FormManager
		{
		public:
			static FormManager* singleton;
			std::list<Kat::Form*> forms;
			virtual Kat::Form* Create(std::string title, int x, int y, int width, int height) = 0;
			virtual void Delete(Kat::Form* form) = 0;
            virtual void MainLoop(Kat::Form* form)=0;
            virtual Rect GetFormRect(Kat::Form* form)=0;
            virtual void SetFormRect(Kat::Form* form,Rect rect)=0;
			virtual Graphic* CreateGraphic(Kat::Form* form) = 0;
		};


		class Graphic
		{
		public:
			virtual void Begin() = 0;
			virtual void Clear(Color color) = 0;
			virtual void FillRectangle(Rect rect, Color* color) = 0;
			virtual void Resize(int width, int height) = 0;
			virtual void End() = 0;
			//virtual void DrawRectangle(Rect rect, DataHelper::Color* color) = 0;
			//virtual void DrawEllipse(Rect rect, DataHelper::Color* color) = 0;
			//virtual void FillEllipse(Rect rect, DataHelper::Color* color) = 0;
			//virtual void DrawLine(Point begin, DataHelper::Point end, int Stroke, Color color) = 0;
			//virtual void DrawArc(Point pos, int startAngle, int endAngle) = 0;
			//virtual void FillPie(Point pos, int startAngle, int endAngle) = 0;
			//virtual void DrawString(Rect rect, std::string str) = 0;
			//virtual void DrawImage(Rect rect, IImageHander image) = 0;
			//virtual IImageHander CreateImage(std::string path) = 0;

		};

		

    }

}



#endif
