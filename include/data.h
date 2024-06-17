//
// Created by dudu233 on 2024/6/17.
//

#ifndef FILESYSTEM_DATA_H
#define FILESYSTEM_DATA_H

#include "filesystem"
#include "fstream"
#include "iostream"
using namespace std;
namespace fs = std::filesystem;

// 文件系统数据的存放路径
#define DATA_PATH "./data"

// 文件系统块的大小
extern int block_size;

bool initData();
bool closeData();
bool setPosition(long long block);          // 设置读写指针位置
char *read(long long size);                 // 读取
char *read(long long block, long long size);// 读取
bool write(char *data);                     // 写入
bool write(long long block, char *data);    // 写入

#endif//FILESYSTEM_DATA_H
