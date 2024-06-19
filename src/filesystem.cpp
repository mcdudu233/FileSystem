//
// Created by dudu233 on 2024/6/17.
//

#include "filesystem.h"

filesystem::filesystem(const string &name, int space, int block) {
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
        tree.addDirectory(directory("root", ".", user_root));
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