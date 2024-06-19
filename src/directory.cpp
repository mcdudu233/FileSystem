//
// Created by dudu233 on 2024/6/17.
//

#include "directory.h"

// 根目录
directory dir_root(".", "..", user_root);

directory::directory() {
}

directory::directory(string name, string father, user master) {
    this->name = name;
    this->master = master;
    this->masterPrivilege = 7;// 文件所有者有全部权限
    this->otherPrivilege = 1; //其他用户仅有读取权限
    this->createTime = chrono::system_clock::now();
    this->modifyTime = chrono::system_clock::now();
    this->father = father;
}

directory::directory(const directory &dir) : name(dir.name),
                                             master(dir.master),
                                             masterPrivilege(dir.masterPrivilege),
                                             otherPrivilege(dir.otherPrivilege),
                                             createTime(dir.createTime),
                                             modifyTime(dir.modifyTime),
                                             father(dir.father),
                                             directories(dir.directories),// 默认复制子目录
                                             files(dir.files)             // 默认复制文件
{}

directory::~directory() {
    // 释放资源
}
//判断所有者是否有读取权限
bool directory::hasMasterPrivilege_read( char masterPrivilege) {
    if(this->masterPrivilege==1||3||5||7)
        return true;
}
// 判断所有者是否有写入权限
bool directory::hasMasterPrivilege_write( char masterPrivilege){
    if(this->masterPrivilege==2||3||6||7)
        return true;
}
// 判断所有者是否执行入权
bool directory::hasMasterPrivilege_execute(char masterPrivilege) {
    if(this->masterPrivilege==4||5||6||7)
        return true;
}
//判断所有者是否具有读取权
bool directory::hasOtherPrivilege_read( char masterPrivilege) {
    if(this->masterPrivilege==1||3||5||7)
        return true;
}// 判断其他用户是否有写入权限
bool directory::hasOtherPrivilege_write( char masterPrivilege){
    if(this->masterPrivilege==2||3||6||7)
        return true;
}
//判断其他用户是否具有执行权
bool directory::hasOtherPrivilege_execute(char masterPrivilege) {
    if(this->masterPrivilege==4||5||6||7)
        return true;
}
//获取目录名
string directory::getName(){
    return this->name;
}
// 设置用户名
bool directory::setName(string name){
    this->name=name;
    return true;
}
// 添加文件
bool directory::addFile(file file) {
    this->files.push_back(file);
    modifyTime = chrono::system_clock::now();// 更新修改时间
    return true;                             // 假设文件添加总是成功
}
// 添加目录
bool directory::addDirectory(directory dir) {
    this->directories.push_back(dir);
    modifyTime = chrono::system_clock::now();// 更新修改时间
    return true;                             // 假设目录添加总是成功
}

// 获取所属用户
user directory::getUser() {
    return this->master;
}

// 获取所有者权限
char directory::getMasterPrivilege() {
    return this->masterPrivilege;
}

// 获取其他人权限
char directory::getOtherPrivilege() {
    return this->otherPrivilege;
}
// 获取所有子目录
vector<directory> directory::getDirectories() {
    return directories;
}
// 获取目录下的文件
vector<file> directory::getFiles(){
    return files;
}
//序列化
void directory::serialize(fstream &out) const {
    size_t nameLength = name.size();
    out.write(reinterpret_cast<const char *>(&nameLength), sizeof(nameLength));
    out.write(name.c_str(), nameLength);

    master.serialize(out);

    out.write(reinterpret_cast<const char *>(&masterPrivilege), sizeof(masterPrivilege));
    out.write(reinterpret_cast<const char *>(&otherPrivilege), sizeof(otherPrivilege));

    auto createTimeTimeT = chrono::system_clock::to_time_t(createTime);
    auto modifyTimeTimeT = chrono::system_clock::to_time_t(modifyTime);
    out.write(reinterpret_cast<const char *>(&createTimeTimeT), sizeof(createTimeTimeT));
    out.write(reinterpret_cast<const char *>(&modifyTimeTimeT), sizeof(modifyTimeTimeT));

    size_t fatherLength = father.size();
    out.write(reinterpret_cast<const char *>(&fatherLength), sizeof(fatherLength));
    out.write(father.c_str(), fatherLength);

    size_t directoriesSize = directories.size();
    out.write(reinterpret_cast<const char *>(&directoriesSize), sizeof(directoriesSize));
    for (const auto &dir: directories) {
        dir.serialize(out);
    }

    size_t filesSize = files.size();
    out.write(reinterpret_cast<const char *>(&filesSize), sizeof(filesSize));
    for (const auto &f: files) {
        f.serialize(out);
    }
}
// 反序列化
void directory::deserialize(fstream &in) {
    size_t nameLength;
    in.read(reinterpret_cast<char *>(&nameLength), sizeof(nameLength));
    name.resize(nameLength);
    in.read(&name[0], nameLength);

    master.deserialize(in);

    in.read(reinterpret_cast<char *>(&masterPrivilege), sizeof(masterPrivilege));
    in.read(reinterpret_cast<char *>(&otherPrivilege), sizeof(otherPrivilege));

    time_t createTimeTimeT;
    time_t modifyTimeTimeT;
    in.read(reinterpret_cast<char *>(&createTimeTimeT), sizeof(createTimeTimeT));
    in.read(reinterpret_cast<char *>(&modifyTimeTimeT), sizeof(modifyTimeTimeT));
    createTime = chrono::system_clock::from_time_t(createTimeTimeT);
    modifyTime = chrono::system_clock::from_time_t(modifyTimeTimeT);

    size_t fatherLength;
    in.read(reinterpret_cast<char *>(&fatherLength), sizeof(fatherLength));
    father.resize(fatherLength);
    in.read(&father[0], fatherLength);

    size_t directoriesSize;
    in.read(reinterpret_cast<char *>(&directoriesSize), sizeof(directoriesSize));
    directories.resize(directoriesSize);
    for (auto &dir: directories) {
        dir.deserialize(in);
    }

    size_t filesSize;
    in.read(reinterpret_cast<char *>(&filesSize), sizeof(filesSize));
    files.resize(filesSize);
    for (auto &f: files) {
        f.deserialize(in);
    }
}