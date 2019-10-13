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


        

        class Linux_X_WndManager;
		class Linux_X_Handler;
		class Linux_X_Graphic:public IGraphic
		{
			friend Linux_X_WndManager;
			Linux_X_Graphic()=default;
		public:
			void Begin(){}
			void Clear(DataHelper::Color color){}
			void FillRectangle(Rect rect, DataHelper::Color* color){}
			void Resize(int width, int height){}
			void End(){}
		};

		class Linux_X_WndHandler :public IWindowHandler
		{
			friend Linux_X_WndManager;
			xlib::Window window = 0;
			xlib::Display* display = nullptr;
			int screen = 0;
		public:
			Rect GetRect()override
			{
				XWindowAttributes attributes;
				XGetWindowAttributes(display, window, &attributes);
				
				return Rect(attributes.x,attributes.y,attributes.width,attributes.height);
			}
			void SetWindowRect(Rect rect)override
			{
				XMoveResizeWindow(display,window,rect.left,rect.top,rect.right,rect.bottom);
			}
			void Show()override
			{
				XMapWindow(display, window);
			}
			void Minimize()
			{
				XSetWindowAttributes
			}
			void Maximize()
			{

			}
			string GetTitle()
			{

			}
			void SetTitle(string title)
			{

			}

		};

		class Linux_X_WndManager :public IWindowManager
		{
			
		public:
			Window* Find(IWindowHandler* handler)override
			{

			}
			int Count()override
			{

			}

			IWindowHandler* Create(std::string title, int x, int y, int width, int height, Window* window)override
			{
				Linux_X_WndHandler* handler = new Linux_X_WndHandler();
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
				handler->window=wnd;
				handler->display=display;
				handler->screen=screen_number;
				collection.insert(std::make_pair(handler,window));
				return handler;
			}
			void Delete(IWindowHandler* handler)override
			{
				IWindowHandler* target = nullptr;
				Linux_X_WndHandler* h = (Linux_X_WndHandler*)handler;
				for (auto iter = collection.begin();iter != collection.end();iter++)
				{
					Linux_X_WndHandler* tmp = (Linux_X_WndHandler*)iter->first;
					if (tmp->window == h->window)
					{
						target = iter->first;
						break;
					}
				}
				xlib::XDestroyWindow(h->display,h->window);
				collection.erase(target);
				print(collection.size());
			}

			IGraphic* CreateGraphic(IWindowHandler* handler)override
			{
				return new Linux_X_Graphic();
			}
			void MainLoop()override;
		};
		IWindowManager* WindowManager = new Linux_X_WndManager();
    }
}

#endif

// void Linux_X_WndManager::MainLoop()
// 		{
// 			bool done = false;
// 			for(auto iter=collection.begin();iter!=collection.end();iter++)	
// 			{
// 				xlib::XEvent event;
// 				Linux_X_WndHandler* handler = (Linux_X_WndHandler*)(iter->first);
// 				Window* window = iter->second;
// 				while (!done) 
// 				{
// 					xlib::XNextEvent(handler->display,&event);
// 					switch(event.type)
// 					{
// 						case Expose:
// 						xlib::XFillRectangle(handler->display,handler->window, DefaultGC(handler->display,handler->screen), 20, 20, 100, 100);
// 						window->ProcessMsg(KatUI::Args(Message::Paint,window->graphic));
// 						break;
// 						case KeyPress:
// 						xlib::XDestroyWindow(handler->display, handler->window);
// 						break;
// 						case DestroyNotify:
// 						done = true;
// 						break;
// 						case ClientMessage:
// 						break;
// 					}
// 				}


// 			}	