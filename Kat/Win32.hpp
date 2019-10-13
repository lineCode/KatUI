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
				ProcessMsg(Args(Message::Paint,graphic));
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
			void ProcessMsg(Args args)override
			{
				Rect rect = FormManager::singleton->GetFormRect(this);
				switch (args.msg)
				{
				case Message::Resize:
					args.graphic->Resize(rect.right, rect.bottom);
					break;
				case Message::Paint:
					args.graphic->Begin();
					args.graphic->Clear(Background);
					if (Content != nullptr)
						(*Content).foreach([&](Element* element) {element->ProcessMsg(args);});

					args.graphic->End();
					break;
				}
			}
        };

		class Win32_Graphic :public Graphic
		{
			friend Win32_WManager;
			static ID2D1Factory* factory;
			ID2D1HwndRenderTarget* target = nullptr;
			Win32_Graphic(Kat::Form* form);
		public:
			void Begin()override
			{
				target->BeginDraw();
			}
			void FillRectangle(Rect rect, Color* color)override
			{
				ID2D1SolidColorBrush* brush;
				target->CreateSolidColorBrush(D2D1::ColorF(color->r, color->g, color->b, color->a), &brush);
				target->FillRectangle(D2D1::RectF(rect.left, rect.top, rect.right, rect.bottom), brush);
			}
			void Clear(Color color)override
			{
				target->Clear(D2D1::ColorF(color.r, color.g, color.b, color.a));
			}
			void Resize(int width, int height)override
			{
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
			if(target!=nullptr)
			{
				switch (message)
				{
				case WM_PAINT:
					PAINTSTRUCT ps;
					BeginPaint(hWnd, &ps);
					target->ProcessMsg(Args(Message::Paint,target->graphic));
					EndPaint(hWnd, &ps);
					break;
				case WM_SIZE:
					target->ProcessMsg(Args(Message::Resize,target->graphic));
					break;
				case WM_DESTROY:
					CrossPlatform::FormManager::singleton->Delete(target);
					PostQuitMessage(0);
					break;
				default:
					return DefWindowProc(hWnd, message, wParam, lParam);
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