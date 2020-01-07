# KatUI
## 这是一个~~随时都有可能弃坑的~~具有组件化、可扩展、适应性布局、跨平台(Win/Linux)特点的轻量级的RMGUI框架

[问题与解决情况日志](log.md)

# 简单演示：创建一个窗口并显示
```c++
    #include"Kat/KatUI.hpp"
    using namespace Kat;
    int main()
    {
        Form* form = createForm("MyWindow",0,50,800,500);
        startUp(form);
    }

```

# ~~从WPF抄来的~~组件化
![](https://raw.githubusercontent.com/Yoorkin/KatUI/master/Resource/iconBtn.png)

如上图，控件`IconButton`由`RoundRectangle`、`Image`、`Text`组成:
```c++
    class IconButton:public Widget
    {
        Text* mtext;
        Image* mimage;
    public:
        //文字属性，供调用者设置按钮的标题
        Property<std::string> text = Property<std::string>(
            [&]()   { return mtext; },
            [&](std::string value){mtext.text = value; }
        );
        //图标属性，供调用者设置按钮的图标
        Property<Image> image = Property<Image>(
            [&]()   { return mimage;},
            [&](Image* value){ mimage = *value; }
        );
        IconButton(std::string text,Image* img)
        {
            this->mtext = new Text(text);
            this->mimage  = img;
            Column column = new Column();
            column.add(mimage);
            column.add(mtext);
            add(new Layout(0),new RoundRectangle(0xFF0000,0xFF00FF,10));//管他什么颜色啦这里就演示一下
            add(new Layout(10),column);
        }
    }
```
这就是一个封装好的、可以重复使用的IconButton控件。下面来创建一个带图标的“发送”按钮并添加到窗体中：

```c++
    //省略创建窗口form的代码
    std::string path = /*小飞机图标的文件路径*/;
    form->add(new Layout(10,10,50,100),new IconButton("send",LoadImage(path)));
```

# ~~鸡肋的~~跨平台
KatUI抹平了不同平台的GUI接口差异，只要按照~~基本法~~正确的姿势调用KatUI,所有的GUI部分代码是可以跨Linux/Windows平台使用的。

# ~~令人抓狂的~~适应性布局
抛弃`定义控件的x、y、width、height`这种写死了GUI布局的布局方式

# ~~鸽出来的~~轻量级
得益于c++的面向对象机制，目前为止所有代码仅1400多行，其中还包括几百行注释和空行

