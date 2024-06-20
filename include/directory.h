//
// Created by dudu233 on 2024/6/17.
//

#ifndef FILESYSTEM_DIRECTORY_H
#define FILESYSTEM_DIRECTORY_H

#include "chrono"
#include "ctime"
#include "file.h"
#include "string"
#include "user.h"
#include "vector"

using std::vector;

class directory {
private:
    /* 基本信息 */
    string name;                  // *关键字：目录名
    string father;                // 父目录的目录名
    vector<directory> directories;// 目录下的子目录
    vector<file> files;           // 目录下的文件
    /* 存取控制 */
    int master = user_root.getUid();// 所属用户
    char masterPrivilege;           // 所有者权限
    char otherPrivilege;            // 其他人权限
    /* 使用信息 */
    chrono::system_clock::time_point createTime;// 创建时间
    chrono::system_clock::time_point modifyTime;// 修改时间

public:
    directory();
    directory(string name, string father, int master);
    directory(const directory &dir);
    ~directory();

public:
    string getName();             // 获取目录名
    bool setName(string name);    // 设置用户名
    string getFather();           // 获取父目录名
    bool setFather(string father);// 设置父目录名

    /* 目录操作 */
    vector<directory> getDirectories();// 获取所有子目录
    vector<file> getFiles();           // 获取所有文件
    bool addFile(file file);           // 新增文件
    bool addDirectory(directory dir);  // 新增目录

    /* 存取控制 */
    int getUser();                                        // 获取所属用户
    bool setUser(int uid);                                // 设置所属用户
    char getMasterPrivilege();                            // 获取所有者权限
    char getOtherPrivilege();                             // 获取其他用户权限
    bool hasMasterPrivilege_read(char masterPrivilege);   // 判断所有者是否有读取权限
    bool hasMasterPrivilege_write(char masterPrivilege);  // 判断所有者是否有写入权限
    bool hasMasterPrivilege_execute(char masterPrivilege);// 判断所有者是否有执行权限
    bool hasOtherPrivilege_read(char otherPrivilege);     //判断其他用户是否有读取权限
    bool hasOtherPrivilege_write(char otherPrivilege);    //判断其他用户是否有写入权限
    bool hasOtherPrivilege_execute(char masterPrivilege); //判断其他用户是否有执行权限

    /* 使用信息 */

    /* 序列化 */
    void serialize(fstream &out) const;// 序列化
    void deserialize(fstream &in);     // 反序列化
};

// 根目录
extern directory dir_root;

#endif//FILESYSTEM_DIRECTORY_H
