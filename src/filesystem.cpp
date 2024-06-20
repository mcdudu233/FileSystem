//
// Created by dudu233 on 2024/6/17.
//

#include "filesystem.h"

void split(string str, vector<string> &v, string spacer) {
    int pos1, pos2;
    int len = spacer.length();//记录分隔符的长度
    pos1 = 0;
    pos2 = str.find(spacer);
    while (pos2 != string::npos) {
        v.push_back(str.substr(pos1, pos2 - pos1));
        pos1 = pos2 + len;
        pos2 = str.find(spacer, pos1);// 从str的pos1位置开始搜寻spacer
    }
    if (pos1 != str.length())//分割最后一个部分
        v.push_back(str.substr(pos1));
}

filesystem::filesystem(const string &name, int space, int block) {
    this->path = "./";
    this->name = name;
    this->space_size = space;
    this->block_size = block;
    this->users.push_back(user_root);
    this->tree = dir_root;

    setSpaceSize(space);
    setBlockSize(block);
    // 初始化文件系统数据
    if (existData(name)) {
        if (!initData(name)) {
            cerr << "Fail to init data." << endl;
        }
        // 然后加载文件系统数据
        setPosition(0);
        deserialize(*getData());
    } else {
        if (!initData(name)) {
            cerr << "Fail to init data." << endl;
        }
        // 建立初始的目录和文件
        tree.addDirectory(directory("root", ".", 0));
    }
}

filesystem::~filesystem() {
    // 保存文件系统数据
    setPosition(0);
    serialize(*getData());
    closeData();
}

void filesystem::serialize(fstream &out) const {
    size_t nameLength = name.size();
    out.write(reinterpret_cast<const char *>(&nameLength), sizeof(nameLength));
    out.write(name.c_str(), nameLength);

    out.write(reinterpret_cast<const char *>(&space_size), sizeof(space_size));
    out.write(reinterpret_cast<const char *>(&block_size), sizeof(block_size));

    size_t usersSize = users.size();
    out.write(reinterpret_cast<const char *>(&usersSize), sizeof(usersSize));
    for (const auto &u: users) {
        u.serialize(out);
    }

    tree.serialize(out);

    out.close();
}

void filesystem::deserialize(fstream &in) {
    size_t nameLength;
    in.read(reinterpret_cast<char *>(&nameLength), sizeof(nameLength));
    name.resize(nameLength);
    in.read(&name[0], nameLength);

    in.read(reinterpret_cast<char *>(&space_size), sizeof(space_size));
    in.read(reinterpret_cast<char *>(&block_size), sizeof(block_size));

    size_t usersSize;
    in.read(reinterpret_cast<char *>(&usersSize), sizeof(usersSize));
    users.resize(usersSize);
    for (auto &u: users) {
        u.deserialize(in);
    }

    tree.deserialize(in);

    in.close();
}

vector<List> filesystem::ls(string path) {

    return vector<List>();
}
