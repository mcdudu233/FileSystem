//
// Created by dudu233 on 2024/6/17.
//

#ifndef FILESYSTEM_DATA_H
#define FILESYSTEM_DATA_H

#include "filesystem"
#include "filesystem.h"
#include "fstream"
#include "iostream"
#include "vector"

using std::cerr;
using std::cout;
using std::endl;
using std::fstream;
using std::ifstream;
using std::ios;
using std::ofstream;
using std::string;
using std::vector;
namespace fs = std::filesystem;

// 文件系统数据的存放路径
#define DATA_PATH "./"
#define DATA_SUFFIX ".hwh.xb.fs"

bool initData(const string &name);
bool closeData();
bool existData(const string &name);
bool setAvailable(vector<bool> *v, int start);
int getSpaceSize();
bool setSpaceSize(int size);
int getBlockSize();
bool setBlockSize(int size);
bool setPosition(long long block);          // 设置读写指针位置
char *read(long long size);                 // 读取
char *read(long long block, long long size);// 读取
bool write(char *data);                     // 写入
bool write(long long block, char *data);    // 写入
fstream *getData();                         // 获取file

/* 文件数据读写方法 */
bool hasData(int block);                        // 判断块是否已经有数据了
int availableData(int block);                   // 获取空闲块
bool useData(int block);                        // 使用空闲块
bool releaseData(int block);                    // 释放已经使用的块
char *readData(long long block, long long size);// 读取
bool writeData(long long block, char *data);    // 写入

#endif//FILESYSTEM_DATA_H
