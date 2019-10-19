#ifndef KAT_LINUX
#define KAT_LINUX
namespace xlib{
using namespace xlib;
#include<Xlib.h>
#define _XPrivDisplay xlib::_XPrivDisplay
}

namespace Kat
{
    namespace CrossPlatform
    {
		class Linux_Manager;

		class Linux_Graphic:public Graphic
		{
			friend Linux_Manager;
			Linux_Graphic()=default;
		public:
			void Begin()override{}
			void Clear(Color color)override{}
			void FillRectangle(Marign marign, Color* color)override{}
			void Resize(int width, int height)override{}
			void End()override{}
		};

		class Linux_Form :public Form
		{
			friend Linux_Manager;
			xlib::Window window = 0;
			xlib::Display* display = nullptr;
			int screen = 0;
			Graphic* graphic;
		public:
			void Show()override
			{
				XMapWindow(display, window);
			}
			void Hide()override
			{

			}
			void Minimize()override
			{

			}
			void Maximize()override
			{

			}
		};

		class Linux_Manager :public FormManager
		{
			
		public:
			Kat::Form* Create(std::string title, int x, int y, int width, int height)
			{
				Linux_Form* form = new Linux_Form();
				xlib::Display* display = xlib::XOpenDisplay(NULL);
				if(display==NULL)
				{
					fprintf(stderr, "\033[;31mUnable to open display,please try to run \"export DISPLAY=:0.0\" on the terminal first.\033[0m\n");
					exit(0);
				}
    			int screen_number = DefaultScreen(display);
    			xlib::Window wnd = xlib::XCreateSimpleWindow(display,RootWindow(display,screen_number),
                                            x,y,width,height,0,
                                            BlackPixel(display, screen_number), WhitePixel(display, screen_number));
				  /* select kind of events we are interested in */
				xlib::XSelectInput(display, wnd, ExposureMask | KeyPressMask | StructureNotifyMask);
				form->window=wnd;
				form->display=display;
				form->screen=screen_number;
				form->graphic=CreateGraphic(form);
				FormManager::singleton->forms.push_back(form);
				return form;
			}
			void Delete(Kat::Form* form)
			{
				xlib::XDestroyWindow(((Linux_Form*)form)->display,((Linux_Form*)form)->window);
				//CrossPlatform::FormManager::singleton->forms.erase(form);
			}
            Rect GetFormRect(Kat::Form* form)
			{
				xlib::XWindowAttributes attributes;
				XGetWindowAttributes(((Linux_Form*)form)->display, ((Linux_Form*)form)->window, &attributes);
				return Rect(attributes.x,attributes.y,attributes.width,attributes.height);			
			}
            void SetFormRect(Kat::Form* form,Rect rect)
			{
				xlib::XMoveResizeWindow(((Linux_Form*)form)->display,((Linux_Form*)form)->window,rect.left,rect.top,rect.right,rect.bottom);
			}
			Timer* CreateTimer(int interval)
			{

			}
			Graphic* CreateGraphic(Kat::Form* form)override
			{
				return new Linux_Graphic();
			}
			void MainLoop(Kat::Form* form)override
			{
				form->Show();
				Args args;
				bool done = false;
				for(auto iter=CrossPlatform::FormManager::singleton->forms.begin();
						 iter!=CrossPlatform::FormManager::singleton->forms.end();iter++)	
				{
					xlib::XEvent event;
					Linux_Form* form = (Linux_Form*)(*iter);
					while (!done) 
					{
						xlib::XNextEvent(((Linux_Form*)form)->display,&event);
						switch(event.type)
						{
						case Expose:
							xlib::XFillRectangle(((Linux_Form*)form)->display,((Linux_Form*)form)->window, DefaultGC(((Linux_Form*)form)->display,((Linux_Form*)form)->screen), 20, 20, 100, 100);
							args.graphic=form->graphic;
							args.msg=Message::Paint;
							form->ProcessMsg(args);
							break;
						case KeyPress:
							xlib::XDestroyWindow(((Linux_Form*)form)->display, ((Linux_Form*)form)->window);
							break;
						case DestroyNotify:
							done = true;
							break;
						case ClientMessage:
							break;
						}
					}
				}
			}
		};
		FormManager* FormManager::singleton = new Linux_Manager();
    }
}

#endif