//
// Created by dudu233 on 2024/6/17.
//

#ifndef FILESYSTEM_FILE_H
#define FILESYSTEM_FILE_H

#include "chrono"
#include "ctime"
#include "iostream"
#include "string"
#include "user.h"
#include "vector"

namespace chrono = std::chrono;
using std::cerr;
using std::cout;
using std::endl;
using std::ifstream;
using std::ios;
using std::ofstream;
using std::runtime_error;
using std::vector;

class file {
private:
    /* 基本信息 */
    string name;      // *关键字：文件名
    vector<int> point;// 所有数据块的指针
    /* 存取控制 */
    int master = user_root.getUid();// 所属用户
    char masterPrivilege;           // 主用户权限
    char otherPrivilege;            // 其他用户权限
    /* 使用信息 */
    int size;                                   // 大小
    chrono::system_clock::time_point createTime;// 创建时间
    chrono::system_clock::time_point modifyTime;// 修改时间

private:
    bool updateTime();// 更新文件修改时间

public:
    file();
    file(string &fileName);
    file(const file &file);
    ~file();

public:
    string getName();         // 获取用户名
    bool setName(string name);// 设置用户名
    int getSize();            //获取文件大小

    /* 文件操作 */
    char *readFile();          // 读取的文件内容
    bool writeFile(char *data);// 写入文件内容

    /* 存取控制 */
    int getUser();                                        // 获取用户
    bool setUser(int uid);                                // 设置用户
    bool setMasterPrivilege(char masterPrivilege);        // 设置文件所有者权限
    bool setOtherPrivilege(char otherPrivilege);          // 设置其他用户的权限
    bool hasMasterPrivilege_read(char masterPrivilege);   // 判断所有者是否有读取权限
    bool hasMasterPrivilege_write(char masterPrivilege);  // 判断所有者是否有写入权限
    bool hasMasterPrivilege_execute(char masterPrivilege);// 判断所有者是否有执行权限
    bool hasOtherPrivilege_read(char otherPrivilege);     // 判断其他用户是否有读取权限
    bool hasOtherPrivilege_write(char otherPrivilege);    // 判断其他用户是否有写入权限
    bool hasOtherPrivilege_execute(char otherPrivilege);  // 判断其他用户是否有执行权限

    /* 使用信息 */

    /* 序列化 */
    void serialize(fstream &out) const;// 序列化
    void deserialize(fstream &in);     // 反序列化
};


#endif//FILESYSTEM_FILE_H
