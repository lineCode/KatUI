// 	class Window :public Element
// 	{
// 		friend Kat;
// #ifdef _WIN32
// 		friend CrossPlatform::Win32_WndManger;
// #endif
// #ifdef __linux__
// 		friend CrossPlatform::Linux_X_WndManager;
// #endif
// 		friend Rectangle;

// 		CrossPlatform::IWindowHandler* handler = nullptr;
// 		CrossPlatform::IGraphic* graphic = nullptr;

// 		Args ProcessMsg(Args args)override
// 		{
// 			switch (args.msg)
// 			{
// 			case Message::Resize:
// 				graphic->Resize(Width, Height);
// 				break;
// 			case Message::Paint:
// 				graphic->Begin();
// 				graphic->Clear(Background);
// 				if (Content != nullptr)
// 					((Element*)Content)->foreach([&](Element* element) {element->ProcessMsg(args);});

// 				graphic->End();
// 				break;
// 			}
// 			return args;
// 		}


// 		void Paint(CrossPlatform::IGraphic* graphic)override
// 		{
// 			print("window paint")
// 		}
// 	public:
// 		Property<Color> Background = Property<Color>(callback, Color(1, 1, 1));

// 		Window(std::string title, int x, int y, int width, int height)
// 		{
// 			this->owner = this;

// 			handler = CrossPlatform::WindowManager->Create(title, x, y, width, height, this);

// 			Left.get = [&] {return handler->GetRect().left;};
// 			Width.get = [&] {return handler->GetRect().right;};
// 			Height.get = [&] {return handler->GetRect().bottom;};
// 			Width.get = [&] {return handler->GetRect().right;};

// 			Left.set = [&](int value) {handler->SetWindowRect(Rect(value, (int)Top, (int)Width, (int)Height));};
// 			Top.set = [&](int value) {handler->SetWindowRect(Rect((int)Left, value, (int)Width, (int)Height));};
// 			Width.set = [&](int value) {handler->SetWindowRect(Rect((int)Left, (int)Top, value, (int)Height));};
// 			Height.set = [&](int value) {handler->SetWindowRect(Rect((int)Left, (int)Top, (int)Width, value));};

// 			graphic = CrossPlatform::WindowManager->CreateGraphic(handler);
// 			//callback = [&] {};
// 		}

// 		void Show()
// 		{
// 			handler->Show();
// 		}

// 		~Window()
// 		{
// 			CrossPlatform::WindowManager->Delete(handler);
// 		}
// 	};