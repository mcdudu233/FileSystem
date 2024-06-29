//
// Created by dudu233 on 2024/6/17.
//

#include "file.h"
#include "data.h"


file::file() {
}

// 文件类构造函数
file::file(const string &fileName) {
    this->name = fileName;
    this->createTime = chrono::system_clock::now();
    updateTime();
    this->masterPrivilege = 7;// 给予文件所有者所有权限
    this->otherPrivilege = 1; // 其他用户无权限
    this->size = 0;           // 初始文件大小为0
}

file::file(const file &f)
    : name(f.name),
      size(f.size),
      master(f.master),
      masterPrivilege(f.masterPrivilege),
      otherPrivilege(f.otherPrivilege),
      createTime(f.createTime),
      modifyTime(f.modifyTime),
      point(f.point) {}

file::~file() {
}

bool file::operator==(const file &other) {
    if (this->name == other.name && this->size == other.size && this->createTime == other.createTime && this->modifyTime == other.modifyTime && this->master == other.master) {
        return true;
    } else {
        return false;
    }
}

// 获取用户名
string file::getName() {
    return this->name;
}

// 设置用户名
bool file::setName(string name) {
    this->name = name;
    return true;
}

// 获取文件大小
int file::getSize() {
    return this->size;
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

bool file::hasMasterPrivilege_read(char masterPrivilege) {// 判断所有者是否有读取权限
    if (this->masterPrivilege == 1)
        return true;
    if (this->masterPrivilege == 3)
        return true;
    if (this->masterPrivilege == 5)
        return true;
    if (this->masterPrivilege == 7)
        return true;
}

bool file::hasMasterPrivilege_write(char masterPrivilege) {// 判断所有者是否有写入权
    if (this->masterPrivilege == 2)
        return true;
    if (this->masterPrivilege == 3)
        return true;
    if (this->masterPrivilege == 6)
        return true;
    if (this->masterPrivilege == 7)
        return true;
}

bool file::hasMasterPrivilege_execute(char masterPrivilege) {//判断所有者是否具有写入权
    if (this->masterPrivilege == 4)
        return true;
    if (this->masterPrivilege == 5)
        return true;
    if (this->masterPrivilege == 6)
        return true;
    if (this->masterPrivilege == 7)
        return true;
}

bool file::hasOtherPrivilege_read(char masterPrivilege) {// 判断其他用户是否有读取权限
    if (this->masterPrivilege == 1)
        return true;
    if (this->masterPrivilege == 3)
        return true;
    if (this->masterPrivilege == 5)
        return true;
    if (this->masterPrivilege == 7)
        return true;
}

bool file::hasOtherPrivilege_write(char masterPrivilege) {// 判断其他用户是否有写入权
    if (this->masterPrivilege == 2)
        return true;
    if (this->masterPrivilege == 3)
        return true;
    if (this->masterPrivilege == 6)
        return true;
    if (this->masterPrivilege == 7)
        return true;
}

bool file::hasOtherPrivilege_execute(char masterPrivilege) {//判断其他用户是否具有执行权
    if (this->masterPrivilege == 4)
        return true;
    if (this->masterPrivilege == 5)
        return true;
    if (this->masterPrivilege == 6)
        return true;
    if (this->masterPrivilege == 7)
        return true;
}

// 获取用户
int file::getUser() {
    return this->master;
}

// 设置用户
bool file::setUser(int uid) {
    this->master = uid;
    return true;
}

bool file::clearFile() {
    if (this->point.empty() || this->size == 0) {
        return false;
    }
    // 清空所有块
    for (auto p: point) {
        releaseBlock(p);
    }
    point.erase(point.begin(), point.end());
    size = 0;
    return true;
}

// 读取文件内容的方法
char *file::readFile() {
    if (this->point.empty() || this->size == 0) {
        return nullptr;
    }
    char *buff = new char[point.size() * getBlockSize()];
    // 读取每一块数据
    for (int i = 0; i < point.size(); i++) {
        char *tmp = readBlock(point[i]);
        memcpy(buff + i * getBlockSize(), tmp, getBlockSize());
        delete[] tmp;
    }
    return buff;
}
//  获取创建时间
chrono::system_clock::time_point file::getCreateTime() {
    return this->createTime;
}
//  获取修改时间
chrono::system_clock::time_point file::getModifyTime() {
    return this->modifyTime;
}
// 写入文件
bool file::writeFile(char *data, int size) {
    int block = size / getBlockSize() + 1;// 要保存在多少块里
    // 清空重写
    clearFile();
    for (int i = 0; i < block; i++) {
        int b = availableBlock();
        useBlock(b);
        // 如果获取不到空闲块 则分区已满
        if (b == -1) {
            return false;
        }
        point.push_back(b);

        char *tmp = new char[getBlockSize()];
        if (size - i * getBlockSize() < 512) {
            memcpy(tmp, data + i * getBlockSize(), size - i * getBlockSize());
        } else {
            memcpy(tmp, data + i * getBlockSize(), getBlockSize());
        }
        writeBlock(b, tmp, getBlockSize());
    }
    this->size = size;
    return true;
}

void file::serialize(fstream &out) const {
    size_t nameLength = name.size();
    out.write(reinterpret_cast<const char *>(&nameLength), sizeof(nameLength));
    out.write(name.c_str(), nameLength);
    size_t pointSize = point.size();
    out.write(reinterpret_cast<const char *>(&pointSize), sizeof(pointSize));
    for (const auto p: point) {
        out.write(reinterpret_cast<const char *>(&p), sizeof(p));
    }

    out.write(reinterpret_cast<const char *>(&master), sizeof(master));
    out.write(reinterpret_cast<const char *>(&masterPrivilege), sizeof(masterPrivilege));
    out.write(reinterpret_cast<const char *>(&otherPrivilege), sizeof(otherPrivilege));

    out.write(reinterpret_cast<const char *>(&size), sizeof(size));
    auto createTimeTimeT = chrono::system_clock::to_time_t(createTime);
    auto modifyTimeTimeT = chrono::system_clock::to_time_t(modifyTime);
    out.write(reinterpret_cast<const char *>(&createTimeTimeT), sizeof(createTimeTimeT));
    out.write(reinterpret_cast<const char *>(&modifyTimeTimeT), sizeof(modifyTimeTimeT));
}

void file::deserialize(fstream &in) {
    size_t nameLength;
    in.read(reinterpret_cast<char *>(&nameLength), sizeof(nameLength));
    name.resize(nameLength);
    in.read(&name[0], nameLength);
    size_t pointSize;
    in.read(reinterpret_cast<char *>(&pointSize), sizeof(pointSize));
    point.resize(pointSize);
    for (auto p: point) {
        in.read(reinterpret_cast<char *>(&p), sizeof(p));
    }

    in.read(reinterpret_cast<char *>(&master), sizeof(master));
    in.read(reinterpret_cast<char *>(&masterPrivilege), sizeof(masterPrivilege));
    in.read(reinterpret_cast<char *>(&otherPrivilege), sizeof(otherPrivilege));

    in.read(reinterpret_cast<char *>(&size), sizeof(size));
    time_t createTimeTimeT;
    time_t modifyTimeTimeT;
    in.read(reinterpret_cast<char *>(&createTimeTimeT), sizeof(createTimeTimeT));
    in.read(reinterpret_cast<char *>(&modifyTimeTimeT), sizeof(modifyTimeTimeT));
    createTime = chrono::system_clock::from_time_t(createTimeTimeT);
    modifyTime = chrono::system_clock::from_time_t(modifyTimeTimeT);
}
