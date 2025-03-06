# QT界面的文件管理系统

> 这是一个课程设计的项目。
> 

---

## 介绍


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

然后再运行：
``` bash
cmake -B build
cmake --build build
```
编译好后的 **exe** 文件在 `build/` ，运行该程序即可。

---