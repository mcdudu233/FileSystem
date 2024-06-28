//
// Created by dudu233 on 2024/6/17.
//

#ifndef FILESYSTEM_FILESYSTEM_H
#define FILESYSTEM_FILESYSTEM_H

#include "data.h"
#include "directory.h"
#include "file.h"
#include "user.h"
#include "utility"

// 返回的目录和文件列表
typedef struct {
    string name;
    int size;
    string type;
    string modifiedTime;
} List;
// 列表的表项
#define ListSize 4

class filesystem {
private:
    vector<string> current;// 当前所在的路径
    string name;           // 文件系统数据的文件名
    int space_size;        // 空间大小
    int block_size;        // 块大小
    int block_data;        // 开始存数据部分的块地址

    vector<user> users;// 所有用户
    directory tree;    // 根目录

    vector<bool> available;// 空闲盘块 位视图法

private:
    static string concat(vector<string> v);                                       // 拼接路径
    static void split(const string &str, vector<string> &v, const string &spacer);// 用于分割路径
    bool getAbsolutePath(string path, vector<string> &v);                         // 根据路径获得绝对路径

public:
    filesystem(const string &name, int space = 0, int block = 0);
    ~filesystem();
    void serialize(fstream &out) const;
    void deserialize(fstream &in);

public:
    /* 基本 */
    string getCurrentPath();                  // 获取当前所在的路径
    directory *getTree();                     // 获取树形目录
    directory *getFatherByName(directory dir);// 根据目录找到父目录
    directory *getFatherByName(file f);       // 根据文件找到父目录
    directory *findParentDirectory(directory *current, directory &target);
    directory *findParentDirectory(directory *current, file &target);
    /* 命令 */
    bool ls(vector<List> &v);                                                    // 列出当前文件夹下的文件
    bool ls(string path, vector<List> &v);                                       // 列出某个文件夹下的文件
    int disk(bool left);                                                         // 获取磁盘容量(left为true时返回剩余容量)
    bool cd(string path);                                                        // 跳转到某个文件夹
    bool rm(file f);                                                             // 删除文件
    bool rm(directory d);                                                        // 删除目录
    int useradd(string name, string password = "", bool super = false);          // 新增用户
    bool usercrg(int uid, string name, string password = "", bool super = false);// 修改用户信息
};

#endif//FILESYSTEM_FILESYSTEM_H
