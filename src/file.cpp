//
// Created by dudu233 on 2024/6/17.
//

#include "file.h"
#include "data.h"
// 文件类构造函数
file::file(string &fileName ) {
    this->name = fileName;
    this->createTime = chrono::system_clock::now();
    updateTime();
    this->masterPrivilege = 7; // 给予文件所有者所有权限
    this->otherPrivilege = 1; // 其他用户无权限
    this->point = -1; // 假设-1表示文件尚未分配存储位置
    this->size = 0; // 初始文件大小为0；
}

file::file(const file &file) {
    this->name=file.name;
    this->size = file.size;
    this->createTime = file.createTime;
    this->modifyTime=file.modifyTime;
    this->masterPrivilege = file.masterPrivilege;
    this->otherPrivilege = file.otherPrivilege;
    this->point = file.point;

}

file::~file(){

}
// 更新文件修改时间
bool file::updateTime() {
    this->modifyTime = chrono::system_clock::now();
    return true;
}
// 设置用户权限
bool file::setMasterPrivilege(char privilege) {
    this->masterPrivilege = privilege;
    return true;
}
bool file::setOtherPrivilege(char privilege) {
    this->otherPrivilege = privilege;
    return true;
}
// 读取文件内容的方法
string file::readFile() {
    if (this->point == -1LL || this->size == 0) {
        // 如果文件没有分配位置或者大小为0，返回空字符串
        return string();
    } // 分配足够大小的缓冲区来存储文件内容
    char *buffer = new char[size];   // 从数据文件中读取内容
    char *readBuf = read(point, size); // 如果读取失败，释放缓冲区并返回空字符串
    if (!readBuf) {
        delete[] buffer;
        return readBuf;
    }  // 将读取的内容复制到字符串缓冲区
}

//bool file::deleteFile() {
//    if (point != -1 && size > 0) {
//        // 清空文件内容（可选，取决于文件系统的实现）
//        char *emptyData = new char[size];
//        for (long long i = 0; i < size; ++i) {
//            emptyData[i] = 0;
//        }
//        write(point, emptyData); // 写入空数据覆盖原有内容
//        delete[] emptyData;
//        point = -1; // 重置文件的位置和大小
//        size = 0;
//    }
//    // 重置文件的创建和修改时间
//    createTime = chrono::system_clock::now();
//    modifyTime = chrono::system_clock::now();
//    return true;
//}

