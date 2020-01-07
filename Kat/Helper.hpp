#ifndef KAT_HELPER
#define KAT_HELPER
#include<functional>
#include<stdexcept>
namespace Kat
{
	typedef std::function<void()> Event;

	// 垃圾MSVC在这加了这个函数没法过编译！
	////使用此函数调用function<T(...)>避免因目标为空抛出错误
	//template<typename Result, typename... ParamList>
	//void call(std::function<Result(ParamList...)> target, ParamList ... list)
	//{
	//	if (target)target(list...);
	//};
	
	//template<typename T>
	//class Delegate
	//{

	//};

    /*
    * 模拟C#的属性
    * 使用lambda表达式传入Getter和Setter设置器
    */
	template<typename T>
	class Property
	{
	public:
		typedef std::function<void(T*)>  Setter;
		typedef std::function<T*()>   Getter;
	private:
		Getter  getter;
		Setter  setter;
	public:
		Property(Getter getter, Setter setter) :getter(getter), setter(setter) {};

		operator T() { return *getter(); }
		void operator =(T value) { setter(new T(value)); }
		void operator =(T* value) { setter(value); }
		T* operator->() { return getter(); }
		T operator*(){ return *getter(); }
		bool operator!=(T value) { return value != *getter(); }
		bool operator!=(T* pointer){ return pointer != getter(); }
		bool operator==(T value) { return value == *getter(); }
		bool operator==(T* pointer) { return pointer == getter(); }
	};

	/*
	*	颜色类
	*	任意通道变化时触发Change
	*/
	class Color
	{
		Event Change;
		float *red = new float(0),*green = new float(0),*blue = new float(0),*alpha = new float(0);
	public:
		Property<float> a = Property<float>(
			[&]() { return alpha; },
			[&](float* value) {/* alpha = value; if(Change)Change();*/ }
		);

		Property<float> r = Property<float>(
			[&]()			{  printf("%f\n",*red); return red; },
			[&](float* value){ /*red = value; if(Change)Change();*/ }
		);

		Property<float> g = Property<float>(
			[&]()			{ return green;},
			[&](float* value){ /*green = value; if(Change)Change();*/ }
		);

		Property<float> b = Property<float>(
			[&]()			{ return blue; },
			[&](float* value){ /*blue = value; if(Change)Change();*/ }
		);

		Color(float red, float green, float blue, float alpha = 1.0)
		{
			*(this->alpha) = alpha; 
			*(this->red) = red;
			*(this->green) = green;
			*(this->blue) = blue;
		}
		Color(long code)
		{
			//旧的颜色计算代码 总是出现莫名其妙的错误 留着以后看看
			//printf("0x%06X\n",code);
			//int byte_r = code / 0x010000;
			//int byte_g = (code - byte_r * 0x010000) / 0x000100;
			//int byte_b = (code - byte_r * 0x010000 - byte_g * 0x000100) / 0x000001;
			//*this->alpha = 1;
			//*this->red = byte_r / 255;
			//*this->green = byte_g / 255;
			//*this->blue = byte_b / 255;

			int byteR = (code >> 16);
			int byteG = (code >> 8) - byteR * 256;
			*(this->blue) = (code - byteR * 256 * 256 - byteG * 256)/255;
			*(this->red) = byteR / 255;
			*(this->green) = byteG / 255;
			*(this->alpha) = 1;
			
		}
	};

	class Point
	{
	public:
		int x,y;
		Point()=default;
		Point(int x,int y):x(x),y(y){};
	};

	class Size
	{
	public:
		int width = 0,height = 0;
	};

	class Corner
	{
	public:
		int leftTop = 0,leftBottom = 0,rightTop = 0, rightBottom = 0; 
		Corner()=default;
		Corner(int value):leftTop(value), leftBottom(value), rightTop(value), rightBottom(value) {};
		Corner(int leftTop,int leftBottom,int rightTop,int rightBottom):
			leftTop(leftTop),leftBottom(leftBottom),rightTop(rightTop),rightBottom(rightBottom){};
	};

	class Rect
	{
	public:
		float x,y,h,w;
		Rect()=default;
		Rect(float x, float y, float h, float w):x(x),y(y),h(h),w(w){};
	};

	class Margin
	{
	public:
		float left = 0, top = 0, right = 0, bottom = 0;
		Margin()=default;
		Margin(float left, float top, float right, float bottom):left(left),top(top),right(right),bottom(bottom){};
	};

	class Graphic;
	class Message
	{
	public:
		enum Enum { Processed, MouseEnter, MouseExit, MouseMove, MouseDown,MouseUp, Resize, Paint, Destory}content = Enum::Processed;
		enum class Sender{ None, LeftBtn, RightBtn, MidBtn}sender = Sender::None;
		Graphic* graphic = nullptr;
		Point* mouse = nullptr;
		Message()=default;
		Message(Enum value):content(value){};
		Message(Enum value,Sender sender):content(value),sender(sender){};
	};
}

#endif