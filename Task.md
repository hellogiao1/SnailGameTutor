### 修改

##### 2022/9/15 12:00

1、通过角色去检测NPC进行交互（其他游戏也是通过角色检测，射线还是盒子？）
（游戏中和物品交互是通过角色碰撞体去检测；和NPC交互是通过摄像机射线检测）
2、UI控件的绑定通过GetFromName，并加指针保护
3、传入角色和NPC指针进去进行数据的读取，以及区分 枚举
4、传入结构体开销问题
5、设计“任务”结构，实现进度功能

##### 2022/9/20 18:00

1、控件一般都是用完就扔的，不需要用对象池存起来，否则会有很多问题（我自己也感受到了）（Addchild的时候会把UI的construct函数再调用一遍）
2、调整显示按钮关闭任务界面，以手游的模式开发，和NPC交互通过UI按钮绑定
3、扩展：3D位置的转换为2D的UI，比如NPC头上的提示，可以根据角色的位置和摄像机朝向来改变UI生成的位置和大小
4、修改检测机制，只在自己客户端执行，不在网络上执行
5、Tick换成Timer

##### 2022/9/26 11:00

1、延伸任务系统（击杀NPC、拾取物品、走到指定地点）
击杀NPC：角色攻击、添加伤害通知、减少血量至死亡，通知任务系统更新

问题：角色身上的capsule为什么会自动适应骨骼大小，我通过查代码CapsuleComponent->InitCapsuleSize(34.0f, 88.0f);

<img src="Task.assets/133087336985449552.png" alt="img" style="zoom:50%;" />

##### 2022/10/8 国庆回来

<img src="Task.assets/133096700416362585.png" alt="img" style="zoom:75%;" />

###### 修改

1. 给动画蒙太奇添加中断以及结束委托函数通知

   <details><summary>思路</summary>
   学习如何写出蓝图中的PlayMontage的回调函数

2. 修改伤害判定为每个玩家

---



## 笔记

* 为什么TArray<>中，自己给自己加友元？
  SharedPtr在特化的时候，即编译时，对于不同的实参类型生成不同的类。
  如果不这样，由此模板生成的特定类型的类无法互相访问其他类型的类。

TWeakPtr，~~智能指针~~

<u>MVC模式</u>：
UI显示，不建议UI中去调用其他类的逻辑，但是在实际开发中，会把具体数据传入进去给UI进行相关逻辑判断

ue4服务器[^1]

[^1]:三种状态：客户端、模拟端、服务端

**创建的UE4变量指针都需要加上UPROPERY宏，让系统管理，否则会被系统错误的清理掉！！！**

DoesContainerMatchTagQuery()

[GAS技能系统介绍（一）同步](https://zhuanlan.zhihu.com/p/472109168)

复习：
RPC网络，官方文档阅读（模拟端，都触发了逻辑）
游戏编程模式复习

<details><summary><font color=darkered>override：参数类型，返回值、const修饰符都需要相同，会进行检查</font></summary>
    报错：cannot convert 'this' pointer from 'const UAS01' to 'UAS01 &'；这是因为函数被声明了为const的，
C++在调用类成员函数时会隐式的传递this指针，而将函数声明为const即相当于this指针被声明为const的，而这个被声明为const的this指针在print函数中调用getStr()时又会被传入，
这里问题就出现了，getStr()函数并没有被声明为const的，那么这里面进行参数传递的时候就会发生类型不匹配的问题，也就是这里报的错误。
</details>

c++变量后面加上“：1”位限定符，为什么要这样做呢，通过查询百度得知而且不能取地址
uint32 变量和bool变量

<font face="黑体" color=green size=5>修改Percentage变量</font>，会实时改动，这是因为通过TAttribute绑定的

### UE动画学习

unreal 动画系统学习：
1、controlrig，重定向、



<center><font color=green>动画通知、武器绑定</center>

连招系统：
按下攻击键，存储当前的状态到队列里面，并排除相同的蒙太奇，避免同一时间重复播放相同的蒙太奇；
这里排除相同的蒙太奇，可以用列表去判断；