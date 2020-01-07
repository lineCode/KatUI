#ifndef KAT_WIN32
#define KAT_WIN32
#include<string>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
//#include<windef.h>
//#include<winuser.h>
#include<d2d1.h>
#include<d2d1helper.h>
#include<d2d1.h>
#include<Gdiplus.h>
#include<d2d1helper.h>
#include<stdio.h>
#include<exception>
#pragma comment(lib,"d2d1.lib")
#pragma comment(lib,"Gdiplus.lib")

#include"Helper.hpp"
#include"Layout.hpp"
#include"Widget.hpp"
#include"PlatformAPI.hpp"

namespace Kat
{
     
		class Win32_Graphic;
        class Win32_WManager;
        class Win32_Form:public Form
        {
			friend FormManager;
			friend Win32_WManager;
			friend Win32_Graphic;
            friend Win32_WManager;

			HWND hwnd = 0;

			std::function<void()> callback = [&]{
				Message msg;
				msg.graphic=graphic;
				msg.content=Message::Paint;
				ProcessMsg(msg);//--------------------------------------- something wrong here
			};

        public:

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
			void Paint(Graphic* graphic)override
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
			D2D1_RECT_F get_RECTF(Rect rect)
			{
				return D2D1::RectF(rect.x,rect.y,rect.w + rect.x,rect.h + rect.y);
			}
			ID2D1SolidColorBrush* getBrush(Color* color)
			{
				ID2D1SolidColorBrush* brush = nullptr;
				printf("%f,%f,%f,%f\n", (float)color->r, (float)color->g, (float)color->b, (float)color->a);
				target->CreateSolidColorBrush(D2D1::ColorF(color->r,color->g,color->b,color->a), &brush);
				if(brush == nullptr)throw std::runtime_error("Direct2D错误：画刷申请失败");
				return brush;
			}
			void release(ID2D1SolidColorBrush* brush)
			{
				if(brush)brush->Release();
				brush = nullptr;
			}
		public:
			void Begin()override
			{
				target->BeginDraw();
			}
			void FillRectangle(Rect rect, Color color)override
			{
				auto brush = getBrush(&color);
				target->FillRectangle(get_RECTF(rect), brush);
				release(brush);
			}
			void DrawRectangle(Rect rect,Color color,float strokeWidth=1.0)override
			{
				auto brush = getBrush(&color);
				target->DrawRectangle(get_RECTF(rect), brush,strokeWidth);
				release(brush);
			}
			void FillRoundRectangle(Rect rect, Color color, float corner)
			{
				auto brush = getBrush(&color);
				target->FillRoundedRectangle(D2D1::RoundedRect(get_RECTF(rect),corner,corner),brush);
				release(brush);
			}
			void DrawRoundRectangle(Rect rect, Color color, float corner, float strokeWidth = 1.0)
			{
				auto brush = getBrush(&color);
				target->DrawRoundedRectangle(D2D1::RoundedRect(get_RECTF(rect), corner, corner), brush);
				release(brush);
			}
			void FillEllipse(Rect rect,Color color)override
			{
				auto brush = getBrush(&color);
				D2D1_ELLIPSE ellipse;
				ellipse.point = D2D1::Point2F(rect.x + rect.w / 2, rect.y + rect.h / 2);
				ellipse.radiusX = rect.w / 2;
				ellipse.radiusY = rect.h / 2;
				target->FillEllipse(&ellipse, brush);
				release(brush);
			}
			void DrawEllipse(Rect rect,Color color,float strokeWidth=1.0)override
			{
				auto brush = getBrush(&color);
				D2D1_ELLIPSE ellipse;
				ellipse.point = D2D1::Point2F(rect.x + rect.w / 2, rect.y + rect.h / 2);
				ellipse.radiusX = rect.w / 2;
				ellipse.radiusY = rect.h / 2;
				target->DrawEllipse(&ellipse, brush, strokeWidth);
				release(brush);
			}
			void DrawString(Rect rect, Color color, std::string text)override
			{
				auto brush = getBrush(&color);
				//target->DrawTextA
				release(brush);
			}
			void Clear(Color color)override
			{
				target->Clear(D2D1::ColorF(color.r,color.g,color.b,color.a));
				/*printf("R:%f G:%f B:%f A:%f\n",(float)color.r, (float)color.g, (float)color.b, (float)color.a);*/
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
				return Rect(rect->left, rect->top, rect->bottom, rect->right);
            }
            void SetFormRect(Kat::Form* form,Rect rect)override
            {
                SetWindowPos(((Win32_Form*)form)->hwnd, HWND_TOPMOST, rect.x, rect.y, rect.w, rect.h, SWP_SHOWWINDOW);
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
											D2D1::HwndRenderTargetProperties(((Win32_Form*)form)->hwnd, D2D1::SizeU(rect.w, rect.h)),
											&target);
		}

    	LRESULT CALLBACK Win32_WManager::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
        {
            Win32_Form* target = ((Win32_WManager*)FormManager::singleton)->GetWin32Form(hWnd);
			Rect tmp,rect;
			Message args;
			if(target!=nullptr)
			{
				switch (message)
				{
				case WM_TIMER:
					FormManager::singleton->timers[(int)wParam]->Tick();
					break;
				case WM_SIZE:
					target->ProcessMsg(Message::Resize);
					break;
				case WM_PAINT:
					PAINTSTRUCT ps;
					BeginPaint(hWnd, &ps);
					target->ProcessMsg(Message::Paint);
					EndPaint(hWnd, &ps);
					break;
				case WM_MOUSEMOVE:
					args.mouse = new Point(LOWORD(lParam),HIWORD(lParam));
					args.content=Message::MouseMove;
					target->ProcessMsg(args);
					break;
				case WM_LBUTTONDOWN:
				case WM_LBUTTONUP:
					args.sender = Message::Sender::LeftBtn;
					break;
				case WM_RBUTTONDOWN:
				case WM_RBUTTONUP:
					args.sender = Message::Sender::RightBtn;
					break;
				case WM_MBUTTONDOWN:
				case WM_MBUTTONUP:
					args.sender = Message::Sender::MidBtn;
					break;
				case WM_DESTROY:
					target->ProcessMsg(Message::Destory);
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
					args.content=Message::MouseDown;
					target->ProcessMsg(args);
					break;
				case WM_LBUTTONUP:
				case WM_MBUTTONUP:
				case WM_RBUTTONUP:
					args.content=Message::MouseUp;
					target->ProcessMsg(args);
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

#endif