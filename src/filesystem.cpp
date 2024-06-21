//
// Created by dudu233 on 2024/6/17.
//

#include "filesystem.h"

filesystem::filesystem(const string &name, int size, int block) {
    this->current.emplace_back(".");
    this->name = name;
    this->space_size = size;
    this->block_size = block;
    this->users.push_back(user_root);
    this->tree = dir_root;

    // 默认10倍空闲分区表的空间储存结构
    this->block_data = 10 * (size / block) / block;
    if (block_data < this->space_size * 0.02 / block) {
        // 采用2%的空间储存结构
        this->block_data = (int) (this->space_size * 0.02 / block);
    }


    setSpaceSize(size);
    setBlockSize(block);
    // 初始化文件系统数据
    setAvailable(&available, block_data);
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
        // 建立空闲分区表
        available.assign(space_size / block_size, false);
        for (int i = 0; i < block_data; i++) {
            available[i] = true;
        }
        // 建立初始的目录和文件
        tree.addDirectory(directory("root", ".", 0));
        file welcome("welcome.txt");// 欢迎文件
        string tmp = "welcome to use file system made by xb and hwh!";
        welcome.writeFile(tmp.data(), tmp.length());
        tree.addFile(welcome);
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
    out.write(reinterpret_cast<const char *>(&block_data), sizeof(block_data));

    size_t usersSize = users.size();
    out.write(reinterpret_cast<const char *>(&usersSize), sizeof(usersSize));
    for (const auto &u: users) {
        u.serialize(out);
    }

    tree.serialize(out);

    size_t availableSize = available.size();
    out.write(reinterpret_cast<const char *>(&availableSize), sizeof(availableSize));
    for (const auto a: available) {
        out.write(reinterpret_cast<const char *>(&a), sizeof(a));
    }

    out.close();
}

void filesystem::deserialize(fstream &in) {
    size_t nameLength;
    in.read(reinterpret_cast<char *>(&nameLength), sizeof(nameLength));
    name.resize(nameLength);
    in.read(&name[0], nameLength);

    in.read(reinterpret_cast<char *>(&space_size), sizeof(space_size));
    in.read(reinterpret_cast<char *>(&block_size), sizeof(block_size));
    in.read(reinterpret_cast<char *>(&block_data), sizeof(block_data));

    size_t usersSize;
    in.read(reinterpret_cast<char *>(&usersSize), sizeof(usersSize));
    users.resize(usersSize);
    for (auto &u: users) {
        u.deserialize(in);
    }

    tree.deserialize(in);

    size_t availableSize;
    in.read(reinterpret_cast<char *>(&availableSize), sizeof(availableSize));
    available.resize(availableSize);
    for (auto a: available) {
        in.read(reinterpret_cast<char *>(&a), sizeof(a));
    }

    in.close();
}

string filesystem::getCurrentPath() {
    return concat(this->current);
}

bool filesystem::ls(vector<List> &v) {
    return ls(getCurrentPath(), v);
}

bool filesystem::ls(string path, vector<List> &v) {
    vector<string> names;
    // 找不到路径则退出
    if (!getAbsolutePath(std::move(path), names)) {
        return false;
    }
    directory *tmp = &tree;
    for (int i = 0; i < names.size() - 1; i++) {
        if (tmp->hasDirectory(tree.getName())) {
            tmp = tmp->getDirectory(tree.getName());
        } else {
            return false;
        }
    }
    // 找到所有文件和子目录
    for (auto dir: tmp->getDirectories()) {
        List l = {dir.getName()};
        v.push_back(l);
    }
    for (auto file: tmp->getFiles()) {
        List l = {file.getName()};
        v.push_back(l);
    }
    return true;
}

bool filesystem::cd(string path) {
    vector<string> names;
    if (!getAbsolutePath(std::move(path), names)) {
        return false;
    }
    this->current = names;
    return true;
}

int filesystem::useradd(string name, string password, bool super) {
    user usr(users.size(), name, password, super);
    users.push_back(usr);
    return usr.getUid();
}

bool filesystem::usercrg(int uid, string name, string password, bool super) {
    for (auto u: users) {
        if (u.getUid() == uid) {
            u.setName(name);
            u.setPassword(password);
            u.setSuper(super);
            return true;
        }
    }
    return false;
}

/* 静态方法 工具类 */
void filesystem::split(const string &str, vector<string> &v, const string &spacer) {
    int pos1, pos2;
    int len = spacer.length();//记录分隔符的长度
    pos1 = 0;
    pos2 = str.find(spacer);
    while (pos2 != string::npos) {
        v.push_back(str.substr(pos1, pos2 - pos1));
        pos1 = pos2 + len;
        pos2 = str.find(spacer, pos1);// 从str的pos1位置开始搜寻spacer
    }
    //分割最后一个部分
    if (pos1 != str.length()) {
        v.push_back(str.substr(pos1));
    }
}

string filesystem::concat(vector<string> v) {
    string tmp;
    for (auto t: v) {
        tmp += t + "/";
    }
    tmp = tmp.substr(0, tmp.size() - 1);
    return tmp;
}

bool filesystem::getAbsolutePath(string path, vector<string> &v) {
    vector<string> names;
    split(path, names, "/");
    // 为空则有问题
    if (names.empty()) {
        return false;
    }
    // 判断是相对路径还是绝对路径
    if (names[0] == ".") {
        // 为绝对路径
        v = names;
    } else if (names[0] == "..") {
        // 多级目录返回
        int i = 0;
        while (names[i] == "..") {
            i++;
        }
        if (i > current.size() - 1) {
            return false;
        }
        // 路径拼接
        names.erase(names.begin(), names.begin() + i);
        names.insert(names.begin(), current.begin(), current.begin() + current.size() - i);
        v = names;
    } else {
        names.insert(names.begin(), current.begin(), current.end());
        v = names;
    }
    return true;
}
