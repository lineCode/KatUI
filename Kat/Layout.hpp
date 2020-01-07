#ifndef KAT_LAYOUT
#define KAT_LAYOUT
#include"Helper.hpp"
namespace Kat
{
    class Widget;

    /*
    * 布局的最基本类
    * parent是父元素的指针
    * x是父元素左端到元素左端的距离
    * y是父元素顶端到元素顶端的距离
    * w是元素的宽度
    * H是元素的高度
    */
	class Layout
	{
	protected:
		friend Widget;
		Widget* owner = nullptr;
		Event Change;
		float paddingW = 0,paddingH = 0;
		float left = 0, top = 0, right = 0, bottom = 0;
		float maxW = 0, minW = 0, maxH = 0, minH = 0;
		float *thresholdW = nullptr,*thresholdH = nullptr;
		void checkXAxis()
		{
			if (owner != nullptr && (maxW != 0 ||  minW != 0 ))
			{
				Rect rect = *this;
				if (maxW != 0)
				{
					if (rect.w > maxW)
					{
						paddingW += (rect.w - maxW)/2;
					}
					else if(paddingW > 0 && (rect.w > minW||minW == 0))
					{
						paddingW -=(maxW - rect.w)/2;
						if(paddingW<0)paddingW=0;
					}
				}
				if (minW != 0)
				{
					if (rect.w < minW)
					{
						paddingW -= (minW - rect.w)/2;
					}
					else if (paddingW < 0 && (rect.w < maxW||maxW == 0))
					{
						paddingW += (rect.w - minW)/2;
						if(paddingW>0)paddingW = 0;
					}
				}
			}
		}

		void checkYAxis()
		{
			if (owner != nullptr && (maxH != 0 || minH != 0))
			{
				Rect rect = *this;
				if (maxH != 0)
				{
					if (rect.h > maxH)
					{
						paddingH += (rect.h - maxH) / 2;
					}
					else if (paddingH > 0 && (rect.h > minH || minH == 0))
					{
						paddingH -= (maxH - rect.h) / 2;
						if (paddingH < 0)paddingH = 0;
					}
				}
				if (minH != 0)
				{
					if (rect.h < minH)
					{
						paddingH -= (minH - rect.h) / 2;
					}
					else if (paddingH < 0 && (rect.h < maxH || maxH == 0))
					{
						paddingH += (rect.h - minH) / 2;
						if (paddingH > 0)paddingH = 0;
					}
				}
			}
		}

	public:
		Layout()=default;

		Layout(float distance)
		{
			this->left = distance;
			this->top = distance;
			this->right = distance;
			this->bottom = distance;
		}

		Layout(float left, float top, float right, float bottom)
		{
			this->left = left;
			this->top = top;
			this->right = right;
			this->bottom = bottom;
		};

		Property<float> Left = Property<float>(
			[&]() { return new float(left + paddingW);  },
			[&](float* value) { left = *value; if(Change)Change(); }
		);

		Property<float> Top = Property<float>(
			[&]() { return new float(top + paddingH);  },
			[&](float* value) { top = *value; if (Change)Change(); }
		);

		Property<float> Right = Property<float>(
			[&]() { return new float(right + paddingW); },
			[&](float* value) { right = *value; if (Change)Change(); }
		);

		Property<float> Bottom = Property<float>(
			[&]() { return new float(bottom + paddingH); },
			[&](float* value) { bottom = *value; if (Change)Change(); }
		);

		Property<float> MinWidth = Property<float>(
			[&]() { return &minW;},
			[&]( float* value ){ minW = *value; checkXAxis(); if (Change)Change(); }
		);

		Property<float> MaxWidth = Property<float>(
			[&]() { return &maxW; },
			[&](float* value) { maxW = *value; checkXAxis(); if (Change)Change(); }
		);

		Property<float> MinHeight = Property<float>(
			[&]() { return &minH; },
			[&](float* value) { minH = *value; checkYAxis(); if (Change)Change(); }
		);

		Property<float> MaxHeight = Property<float>(
			[&]() { return &maxH; },
			[&](float* value) { maxH = *value; checkYAxis(); if (Change)Change(); }
		);

		//operator Margin()
		//{
		//	auto p = this;
		//	Margin ret;
		//	do
		//	{
		//		ret.left += p->left;
		//		ret.top += p->top;
		//		ret.right += p->right;
		//		ret.bottom += p->bottom;
		//		p = p->parent;
		//	}while(p!=nullptr);
		//	return ret;
		//}
		
		virtual operator Rect();

	};



	class Squre:public Layout
	{
		float distance = 0;
	public:
		Squre(float dist);
		//virtual operator Rect()override;
	};



  //  class Layout
  //  {
  //  protected:
  //      friend Margin;
  //      Widget* owner = nullptr; //布局的拥有者
  //      Layout* parent = nullptr;
  //      int x = 0,y = 0,w = 0,h = 0;
  //  public:
		//Layout()=default;
		//Layout(int x, int y, int width, int height):x(x),y(y),w(width),h(height){};

  //      int getXDistanceToRoot()
  //      {
  //          int ret = x;
  //          auto p = parent;
  //          while(p != nullptr)
  //          {
  //              ret+=p->x;
  //          }
  //          return ret;
  //      }

  //      int getYDistanceToRoot()
  //      {
  //          int ret = y;
  //          auto p = parent;
  //          while(p != nullptr)
  //          {
  //              ret+=p->y;
  //          }
  //          return ret;
  //      }

  //      operator Rect()
  //      {
  //          Rect ret(this->y,this->x,w,h);
  //          auto p = parent;
  //          while(p != nullptr)
  //          {
  //              ret.y+=p->y;
  //              ret.x+=p->x;
  //          }
  //          return ret;
  //      }

  //  };

  //  /*
  //  *  布局边距类
  //  *  Left、Top、Right、Bottom均为对应的边到父布局对应的边的距离
  //  */
  //  class Margin:public Layout
  //  {
  //  public:
  //      Property<int> Left = Property<int>(
  //          [&]()           { return &this->x;  },
  //          [&](int value)  { this->x = value; }
  //      );

  //      Property<int> Top = Property<int>(
  //          [&]()           { return &this->y;  },
  //          [&](int value)  { this->y = value; }
  //      );

  //      Property<int> Right = Property<int>(
  //          [&]()           { return new int(parent->w - this->x - this->w); },
  //          [&](int value)  { this->w = parent->w - this->x - value; }
  //      );

  //      Property<int> Bottom = Property<int>(
  //          [&]()           { return new int(parent->h - this->y - this->h); },
  //          [&](int value)  { this->h = parent->h - this->y - value; }
  //      );

		//Margin(int left, int top, int right, int bottom)
		//{
		//	this->x = left;
		//	this->y = top;
		//	this->w = right - left;
		//	this->h = bottom - top;
		//}

		//operator Rect()
		//{
		//	Rect ret(this->y, this->x, 0, 0);
		//	auto p = parent;
		//	while (p != nullptr)
		//	{
		//		ret.y += p->y;
		//		ret.x += p->x;
		//		ret.w -= ret.w
		//	}
		//	auto Rect = FormManager::singleton->GetFormRect((Form*)p);
		//	return ret;
		//}

    //};


}
#endif