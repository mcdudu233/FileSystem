# QT界面的文件管理系统

![](https://oss.mcso.top/wordpress/2025/03/20250306024548617.png)

> 这是一个课程设计的项目。
> 用C++实现的开源文件系统项目！3689行代码详解FCB结构设计、目录权限管理、数据序列化与Qt图形界面开发。可二次开发的文件系统源码，学习如何通过位示图法实现磁盘空间管理！

---

## 项目结构

本项目基于 `CMake` 构建。

`form`：窗口设计文件

`include`：头文件

`res`：资源文件

`src`：源代码

---

## 构建

### 使用 CLion

本项目是用 **CLion** 写的，可以通过 **CLion** 打开。直接点击右上角的绿色箭头运行即可。

### 使用 CMake

以下命令在项目文件夹下运行，因此先进入项目文件夹`cd filesystem`。

修改本目录下的`QT_PATH`文件，填入你的`QT`路径，如：

```vim
H:\QT\6.7.0\mingw_64
```

然后再运行：
``` bash
mkdir build
cd build
cmake -G "MinGW Makefiles" ..
cmake --build .
```
编译好后的 **exe** 文件在 `build/` ，运行该程序即可。

注意可能需要以下库：

```bash
cp "C:\Qt\6.7.2\mingw_64\bin\libgcc_s_seh-1.dll" ".\"
cp "C:\Qt\6.7.2\mingw_64\bin\libstdc++-6.dll" ".\"
cp "C:\Qt\6.7.2\mingw_64\bin\libwinpthread-1.dll" ".\"
```

---