 
 
 # 问题与解决情况日志

## 2020-1-5:

- [ ] 布局组件中Parent属性存在bug
- [ ] 为了适应XML布局，Widget的childs应更改为只能容纳一个子组件，需要
- [ ] MSVC编译的程序中，存在窗口最大化和恢复大小时布局因组件因大小限制出错的问题
- [ ] 许多传递参数的地方（尤其是消息、绘制和布局计算中）存在严重的内存泄漏问题
- [ ] 在vs中使用Release模式下，Ellipse的Backgound.r可能会变成1（图形变为红色）
目前已知使用以下操作会复现此bug：
    ```c++
        Form* form = createForm("lalalal",0,50,800,500);
        auto r1 = new Kat::Ellipse(Color(0, 1, 1, 0), 0x0000FF);
        form->add(new Layout(400, 0, 0, 0), new Kat::Ellipse(Color(0, 0, 0, 0.4F), 0x00FF00));
        auto l = new Layout(100,100,100,100);
        auto b = new Kat::RoundRectangle(10,Color(0, 1, 1, 0), 0xFF0000);
        b->add(l,r1);
        form->add(new Layout(0),b);
        startUp(form);
    ```
    当创建r1的操作移动到第一次调用form->add之后时（即添加出现问题的Ellipse），bug消失