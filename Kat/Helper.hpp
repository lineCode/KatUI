#ifndef KAT_HELPER
#define KAT_HELPER
#include<functional>

namespace Kat
{
	namespace CrossPlatform
	{
		class Graphic;
	}
	enum Direction { Horizontal, Vertical };
	enum Message 
	{ 
		Processed, 
		MouseEnter, 
		MouseExit, 
		MouseMove, 
		MouseDown,
		MouseUp, 
		Resize, 
		Paint 
	};
	enum ButtonType {left,middle,right};
	typedef std::function<void()> ChangeEvent;
	
	namespace Animation
	{
		class IAnimation;
	}
	
	class Style
    {
        std::list<Animation::IAnimation*> normal;
        std::list<Animation::IAnimation*> hightlight;
        std::list<Animation::IAnimation*> pressed;
        std::list<Animation::IAnimation*> selected;
        std::list<Animation::IAnimation*> appear;
        std::list<Animation::IAnimation*> disappear;
    };

	// template<typename T>
	// class IEnumerable
	// {
	// 	virtual void foreach(std::function<void(T)>) = 0;
	// };

    template<typename T>
	class Property
	{
		std::shared_ptr<T>      t = std::make_shared<T>();
		std::shared_ptr<bool>   inEvent = std::make_shared<bool>(false);

		ChangeEvent OnChange;
		std::function<T()>      get;
		std::function<void(T)>  set;

	public:
		Property() = default;
		Property(ChangeEvent function, T value)
		{
			t = std::make_shared<T>(value);
			OnChange = function;
		}
		Property(std::function<T()> getter, std::function<void(T)> setter)
		{
			get = getter;
			set = setter;
		}
		Property(std::function<T()> getter, std::function<void(T)> setter, T value)
		{
			get = getter;
			set = setter;
			t = std::make_shared<T>(value);
		}
		Property(const Property& value)
		{
			t = value.t;
			get = value.get;
			set = value.set;
			inEvent = value.inEvent;
		}
		T& operator->()
		{
			return *t;
		}
		operator T()
		{
			if (get && !*inEvent)
			{
				(*inEvent) = true;
				T tmp = get();
				(*inEvent) = false;
				return tmp;
			}
			else
				return *t;
		}
		void operator =(T value)
		{
			if (set && !*inEvent)
			{
				(*inEvent) = true;
				set(value);
				(*inEvent) = false;
			}
			else
				*t = value;

			if (OnChange) OnChange();
		}
		bool operator==(T value)
		{
			return value == (*t);
		}
	};

	class Point
	{
	public:
		Point()=default;
		Point(int X, int Y) :x(X), y(Y) {};
		int x = 0, y = 0;
	};

	class Args
	{
	public:
		Message msg;
		CrossPlatform::Graphic* graphic;
		Point mouse;
		ButtonType button;
	};
	

    class Rect
		{
		public:
			Rect() = default;
			Rect(int Left, int Top, int Right, int Bottom) :left(Left), top(Top), right(Right), bottom(Bottom) {};
			int left = 0, top = 0, right = 0, bottom = 0;
		};

		class Marign
		{
		public:
			Marign() = default;
			Marign(int Left, int Top, int Right, int Bottom) :left(Left), top(Top), right(Right), bottom(Bottom) {}
			int left = 0, top = 0, right = 0, bottom = 0;

		};

		class Color
		{
			ChangeEvent OnChange;
			ChangeEvent callback = [&] {if (OnChange)OnChange();};
		public:
			Property<float> a = Property<float>(callback, 0);
			Property<float> r = Property<float>(callback, 0);
			Property<float> g = Property<float>(callback, 0);
			Property<float> b = Property<float>(callback, 0);

			Color() = default;
			Color(float red, float green, float blue, float alpha = 1.0)
			{
				a = alpha;r = red;g = green;b = blue;
			}
			Color(long code)
			{
				int byte_r = code / 0x010000;
				int byte_g = (code - byte_r * 0x010000) / 0x000100;
				int byte_b = (code - byte_r * 0x010000 - byte_g * 0x000100) / 0x000001;

				r = byte_r / 255;
				g = byte_g / 255;
				b = byte_b / 255;
			}
		};


}

#endif