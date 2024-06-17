//
// Created by dudu233 on 2024/6/17.
//

#include "file.h"
// 文件类构造函数
file::file(string &fileName ) {
    this->name = fileName;
    // 尝试打开文件
    fileHandle.open(name.c_str(), ios::in | ios::out); // 打开文件用于读写
    if (!fileHandle.is_open()) {
        // 如果文件打开失败，处理错误
        cerr << "无法打开文件：" << name << endl;
        // 可以抛出异常或返回错误代码，取决于错误处理策略
        throw runtime_error("文件打开失败");
    }
    this->createTime = chrono::system_clock::now();
    updateTime();
    this->masterPrivilege = 7; // 给予文件所有者所有权限
    this->otherPrivilege = 1; // 其他用户无权限
    this->point = -1; // 假设-1表示文件尚未分配存储位置
    this->size = 0; // 初始文件大小为0；
}
file::~file(){
    // 析构函数中关闭文件句柄
    if (fileHandle.is_open()) {
        fileHandle.close();
}
}
bool file::updateTime(){
    this->modifyTime = chrono::system_clock::now();
    return true;
}
// 设置用户权限
bool file::setMasterPrivilege(char privilege) {
    this->masterPrivilege = privilege;
    return true;
}
 bool file::setOtherPrivilege(char privilege) {
    this->otherPrivilege =privilege;
    return true;
}
// 删除文件
bool file::deleteFile() {
    fileHandle.close(); // 确保文件句柄已关闭
    // 删除文件
    if (remove(name.c_str()) == 0) {
        return true; // 文件删除成功
    } else {
        return false; // 文件删除失败
    }
}
// 读取文件内容的方法
string file::readFile() {
    string content;
    string line;
    while (getline(fileHandle, line)) {
        content += line + "\n"; // 将每行添加到content中，并用换行符分隔
    }
    return content;
}

