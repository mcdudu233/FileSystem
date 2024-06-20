//
// Created by dudu233 on 2024/6/17.
//

#ifndef FILESYSTEM_FILESYSTEM_H
#define FILESYSTEM_FILESYSTEM_H

#include "data.h"
#include "directory.h"
#include "file.h"
#include "user.h"

// 返回的目录和文件列表
typedef struct {
    string name;
} List;

class filesystem {
private:
    vector<string> current;// 当前所在的路径
    string name;           // 文件系统数据的文件名
    int space_size;        // 空间大小
    int block_size;        // 块大小

    vector<user> users;// 所有用户
    directory tree;    // 根目录

private:
    static string concat(vector<string> v);                                       // 拼接路径
    static void split(const string &str, vector<string> &v, const string &spacer);// 用于分割路径
    bool getAbsolutePath(string path, vector<string> &v);                         // 根据路径获得绝对路径

public:
    filesystem(const string &name, int space, int block);
    ~filesystem();
    void serialize(fstream &out) const;
    void deserialize(fstream &in);

public:
    /* 基本 */
    string getCurrentPath();// 获取当前所在的路径
    /* 命令 */
    bool ls(vector<List> &v);             // 列出当前文件夹下的文件
    bool ls(string path, vector<List> &v);// 列出某个文件夹下的文件
    bool cd(string path);                 // 跳转到某个文件夹
};

#endif//FILESYSTEM_FILESYSTEM_H
