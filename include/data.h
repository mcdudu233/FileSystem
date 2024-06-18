//
// Created by dudu233 on 2024/6/17.
//

#ifndef FILESYSTEM_DATA_H
#define FILESYSTEM_DATA_H

#include "filesystem"
#include "fstream"
#include "iostream"

using std::cerr;
using std::cout;
using std::endl;
using std::fstream;
using std::ifstream;
using std::ios;
using std::ofstream;
using std::string;
namespace fs = std::filesystem;

// 文件系统数据的存放路径
#define DATA_PATH "./"
#define DATA_SUFFIX ".hwh.xb.fs"

bool initData(string name);
bool closeData();
bool existData(string name);
int getSpaceSize();
bool setSpaceSize(int size);
int getBlockSize();
bool setBlockSize(int size);
bool setPosition(long long block);          // 设置读写指针位置
char *read(long long size);                 // 读取
char *read(long long block, long long size);// 读取
bool write(char *data);                     // 写入
bool write(long long block, char *data);    // 写入

#endif//FILESYSTEM_DATA_H
