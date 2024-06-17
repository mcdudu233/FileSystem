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

bool initData();
bool closeData();

#endif//FILESYSTEM_DATA_H
