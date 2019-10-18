#ifndef KAT_ANIMATION
#define KAT_ANIMATION
#include<vector>
#include<ctime>
#include"Helper.hpp"
#include"PlatformAPI.hpp"
namespace Kat
{
    // namespace Easing
    // {
    //     template<typename T>
    //     class EasingFunction
    //     {
    //     public:
    //         virtual T Easing(T x)=0;
    //     };

    //     template<typename T>
    //     class Liner:public EasingFunction<T>
    //     {
    //     public:
    //         T Easing(T x,T min,T max)
    //         {
    //             return 
    //         }
    //     };

    // }

    namespace Animation
    {
        class IAnimation
        {
        public:
            virtual void Play() = 0;
        };

        template<typename T>
        class Liner:public IAnimation
        {
            Property<T> property;
            T last_value;
            T begin,end;
            time_t duringTime,beginTime;
            CrossPlatform::Timer* timer;
        public:
            void Liner(Property<T> property,T begin,T end,time_t duringTime)
            {
                this->property=property;
                this->begin=begin;
                this->now=begin;
                this->duringTime=duringTime;
                this->last_value=property;
            }

            void Play()override
            {
                timer=CrossPlatform::FormManager::singleton->CreateTimer(500);
                beginTime=time(NULL);
                timer->Tick=[&]{
                    time_t now = time(NULL);
                    if(beginTime+duringTime>now)
                    {
                        property = last_value+(now-beingTime/duringTime)*(end-begin);
                    }
                    else
                    {
                        property = end;
                        timer->UnEnable();
                    }
                }
                timer->Enable();
            }

        };

        template<typename T>
        class Switches:public IAnimation
        {
            T value;
            Property<T> property;
        public:
            Switches(Property<T> property,T value)
            {
                this->value=value;
                this->property=property;
            }

            void Play()override
            {
                property=value;
            }
        };

        class boolAnimation:public IAnimation
        {
        public:
            Property<int>* target;

            void Play()override
            {
                
            }

        };
    }
    using namespace Animation;
}

#endif