#ifndef KAT_WIN32
#define KAT_WIN32
#include<string>
#include<windef.h>
#include<winuser.h>
#include<d2d1.h>
#include<d2d1helper.h>
#include<d2d1.h>
#include<Gdiplus.h>
#include<d2d1helper.h>
#pragma comment(lib,"d2d1.lib")
#pragma comment(lib,"Gdiplus.lib")

#include"Element.hpp"

namespace Kat
{
    namespace CrossPlatform
    {
       
		class Win32_Graphic;
        class Win32_WManager;
        class Win32_Form:public Kat::Form
        {
			friend FormManager;
			friend Win32_WManager;
			friend Win32_Graphic;
            friend Win32_WManager;

			HWND hwnd = 0;
			Graphic* graphic = nullptr;
			std::function<void()> callback = [&]{
				Args args;
				args.graphic=graphic;
				args.msg=Message::Paint;
				ProcessMsg(args);//--------------------------------------- something wrong here
			};

        public:

			Property<Color> Background = Property<Color>(callback, Color(1, 1, 1));

            void Show()override
            {
				ShowWindow(hwnd,SW_SHOW);
            }
            void Hide()override
            {
				ShowWindow(hwnd,SW_HIDE);
            }
			void Maximize()override
			{
				ShowWindow(hwnd,SW_MAXIMIZE);
			}
            void Minimize()override
            {
				ShowWindow(hwnd,SW_MINIMIZE);
            }
		private:
			void Paint(CrossPlatform::Graphic* graphic)override
			{
				graphic->Clear(Background);
			}

        };

		class Win32_Timer:public Timer
		{
			friend Win32_WManager;
			static int count;
			int id;
			HWND hwnd;
			Win32_Timer(HWND hwnd)
			{
				this->hwnd=hwnd;
				id=++Win32_Timer::count;
			}
		public:
			void Enable()override
			{
				SetTimer(hwnd, id, interval, NULL);
			}
			void UnEnable()override
			{
				KillTimer(hwnd,id);
			}
		};
		int Win32_Timer::count=0;

		class Win32_Graphic :public Graphic
		{
			friend Win32_WManager;
			static ID2D1Factory* factory;
			ID2D1HwndRenderTarget* target = nullptr;
			Win32_Graphic(Kat::Form* form);
			D2D1_RECT_F& get_RECTF(Marign marign)
			{
				return D2D1::RectF(offset.left+marign.left,
									offset.top+marign.top, 
									width-marign.right-marign.left-offset.right-offset.left,
									height-marign.top-marign.bottom-offset.top-offset.bottom);
			}
		public:
			void Begin()override
			{
				target->BeginDraw();
			}
			void FillRectangle(Marign marign, Color* color)override
			{
				ID2D1SolidColorBrush* brush;
				target->CreateSolidColorBrush(D2D1::ColorF(color->r, color->g, color->b, color->a), &brush);
				target->FillRectangle(get_RECTF(marign), brush);
			}
			void DrawRectangle(Marign marign,Color* color,float strokeWidth=1.0)override
			{
				ID2D1SolidColorBrush* brush;
				target->CreateSolidColorBrush(D2D1::ColorF(color->r, color->g, color->b, color->a), &brush);
				target->DrawRectangle(get_RECTF(marign), brush,strokeWidth);
			}
			void FillEllipse(Marign marign,Color* color)override
			{
				ID2D1SolidColorBrush* brush;
				D2D1_ELLIPSE ellipse;
				auto rect = get_RECTF(marign);
				ellipse.point=D2D1::Point2F((rect.right-rect.left)/2,(rect.bottom-rect.top)/2);
				ellipse.radiusX=(rect.right-rect.left)/2;
				ellipse.radiusY=(rect.bottom-rect.top)/2;
				target->CreateSolidColorBrush(D2D1::ColorF(color->r, color->g, color->b, color->a), &brush);
				target->FillEllipse(&ellipse, brush);
			}
			void DrawEllipse(Marign marign,Color* color,float strokeWidth=1.0)override
			{
				ID2D1SolidColorBrush* brush;
				D2D1_ELLIPSE ellipse;
				auto rect = get_RECTF(marign);
				ellipse.point=D2D1::Point2F((rect.right-rect.left)/2,(rect.bottom-rect.top)/2);
				ellipse.radiusX=(rect.right-rect.left)/2;
				ellipse.radiusY=(rect.bottom-rect.top)/2;
				target->CreateSolidColorBrush(D2D1::ColorF(color->r, color->g, color->b, color->a), &brush);
				target->DrawEllipse(&ellipse, brush,strokeWidth);
			}
			void Clear(Color color)override
			{
				target->Clear(D2D1::ColorF(color.r, color.g, color.b, color.a));
			}
			void Resize(int width, int height)override
			{
				this->width=width;
				this->height=height;
				target->Resize(D2D1::SizeU(width, height));
			}
			void End()override
			{
				target->EndDraw();
			}

		};

        class Win32_WManager:public FormManager
        {
            //friend Win32_WManager;
            Win32_Form* GetWin32Form(HWND hwnd)
            {
                Win32_Form* target = nullptr;
                for(auto iter=forms.begin();iter!=forms.end();iter++)
                {
                    if(((Win32_Form*)*iter)->hwnd==hwnd)
                    {
                        target=(Win32_Form*)*iter;
                        break;
                    }
                }
				return target;
            }
        public:

            static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

			Graphic* CreateGraphic(Kat::Form* form)override
            {
				return new Win32_Graphic(form);
            }

			Timer* CreateTimer(int interval)override
			{
				return new Win32_Timer(((Win32_Form*)forms.front())->hwnd);
			}

			Kat::Form* Create(std::string title, int x, int y, int width, int height)override
            {
                WNDCLASSEXA WndClass;
				char CLASS_NAME[] = "KatWindow";
				if (forms.size() == 0)
				{
					WndClass.cbSize = sizeof(WNDCLASSEXA);
					WndClass.style = CS_VREDRAW | CS_HREDRAW;
					WndClass.lpfnWndProc = (WNDPROC)WndProc;
					WndClass.cbClsExtra = 0;
					WndClass.cbWndExtra = 0;
					WndClass.hInstance = GetModuleHandle(NULL);
					WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
					WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
					WndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW);
					WndClass.lpszClassName = CLASS_NAME;
					WndClass.lpszMenuName = NULL;
					WndClass.hIconSm = NULL;

					int ret = RegisterClassExA(&WndClass);
				}
				HWND hwnd = CreateWindowExA(WS_EX_APPWINDOW,
					CLASS_NAME,
					title.c_str(),
					WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SIZEBOX,
					x, y, width, height,
					NULL, NULL, GetModuleHandle(NULL), NULL);
                Win32_Form* form = new Win32_Form();
                form->hwnd=hwnd;
				form->graphic = CreateGraphic(form);
				form->owner=form;
				forms.push_back(form);
				return form;
            }
			void Delete(Kat::Form* form)override
            {
                forms.remove(form);
            }
            void MainLoop(Kat::Form* form)override
            {
				MSG msg;
				while (GetMessage(&msg, NULL, 0, 0) && forms.size() != 0)
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
            }
            Rect GetFormRect(Kat::Form* form)override
            {
                LPRECT rect = new RECT();
				int result = GetClientRect(((Win32_Form*)form)->hwnd, rect);
				return Rect(rect->left, rect->top, rect->right, rect->bottom);
            }
            void SetFormRect(Kat::Form* form,Rect rect)override
            {
                SetWindowPos(((Win32_Form*)form)->hwnd, HWND_TOPMOST, rect.left, rect.top, rect.right, rect.bottom, SWP_SHOWWINDOW);
            }


        };

		FormManager* FormManager::singleton = new Win32_WManager();

		ID2D1Factory* Win32_Graphic::factory = nullptr;
		
		Win32_Graphic::Win32_Graphic(Kat::Form* form)
		{
			if (factory == nullptr)
			D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);
			Rect rect = FormManager::singleton->GetFormRect(form);
			factory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
											D2D1::HwndRenderTargetProperties(((Win32_Form*)form)->hwnd, D2D1::SizeU(rect.right, rect.bottom)),
											&target);
		}

        LRESULT CALLBACK Win32_WManager::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
        {
            Win32_Form* target = ((Win32_WManager*)FormManager::singleton)->GetWin32Form(hWnd);
			Args args;
			if(target!=nullptr)
			{
				switch (message)
				{
				case WM_TIMER:
					CrossPlatform::FormManager::singleton->timers[(int)wParam]->Tick();
					break;
				case WM_PAINT:
					PAINTSTRUCT ps;
					BeginPaint(hWnd, &ps);
					target->graphic->Begin();
					args.msg=Message::Paint;
					args.graphic=target->graphic;
					Rect rect = CrossPlatform::FormManager::singleton->GetFormRect(target);
					target->foreach([&](Element* element) {
						args.graphic->offset = element->GetRelativeMarign(target);
						element->ProcessMsg(args);
						});
					target->graphic->End();
					EndPaint(hWnd, &ps);
					break;
				case WM_SIZE:
					auto tmp = CrossPlatform::FormManager::singleton->GetFormRect(target);
					target->graphic->Resize(tmp.right,tmp.bottom);
					args.graphic=target->graphic;
					args.msg=Message::Resize;
					target->ProcessMsg(args);
					break;
				case WM_MOUSEMOVE:
					FormManager::singleton->mouse.x=LOWORD(lParam);
					FormManager::singleton->mouse.y=HIWORD(lParam);
					args.msg=Message::MouseMove;
					args.mouse=FormManager::singleton->mouse;
					target->foreach([&](Element* element){element->ProcessMsg(args);});
					break;
				case WM_LBUTTONDOWN:
				case WM_LBUTTONUP:
					args.button=ButtonType::left;
					break;
				case WM_RBUTTONDOWN:
				case WM_RBUTTONUP:
					args.button=ButtonType::right;
					break;
				case WM_MBUTTONDOWN:
				case WM_MBUTTONUP:
					args.button=ButtonType::middle;
					break;
				case WM_DESTROY:
					CrossPlatform::FormManager::singleton->Delete(target);
					PostQuitMessage(0);
					break;
				default:
					return DefWindowProc(hWnd, message, wParam, lParam);
				}

				switch(message)
				{
				case WM_LBUTTONDOWN:
				case WM_MBUTTONDOWN:
				case WM_RBUTTONDOWN:
					args.msg=MouseDown;
					target->foreach([&](Element* element){element->ProcessMsg(args);});
					break;
				case WM_LBUTTONUP:
				case WM_MBUTTONUP:
				case WM_RBUTTONUP:
					args.msg=MouseUp;
					target->foreach([&](Element* element){element->ProcessMsg(args);});
					break;
				}

			}
			else
			{
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
            return 0;
        }
    }
}

#endif