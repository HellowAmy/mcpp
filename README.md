# C++入门编程代码规范


在无编程经验初学者的视角，编写C++11为基础的入门教程案例，重点讲述代码在 **编程规范** 方面的内容，让初学者结合AI的反馈编写程序

## 目录
#### 目录开始
[0.前言](./doc/0.前言.md)  
[1.准备编译环境](./doc/1.准备编译环境.md)  
[2.第一个标准程序](./doc/2.第一个标准程序.md)  
  
#### 目录结束
  

## 启动测试
#### 脚本启动
在example目录下执行脚本，参数是目录名词，存在01/02/03...等目录，对应解释文档在doc目录下对应序号

~~~
./run.sh 01
~~~


## 编译需求
编译环境需求，要求等于或大于下列版本

~~~
1.Linux 2.36
2.VSCode
3.G++ 12.0
4.CMake 4.0
5.Python (可选)
~~~

如果CMake版本过小请尝试修改CMakeLists.txt文件

~~~
cmake_minimum_required(VERSION 4.0)
改为
cmake_minimum_required(VERSION 3.20)
~~~

#### 一键启动
可以使用VSCode一键运行，存在tasks.json和launch.json文件，如果文件未更新，请在bash目录下执行脚本更新配置文件，随后在VSCode中可见

~~~
python3 update_launch.py
~~~


