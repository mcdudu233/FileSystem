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
    this->user_current = nullptr;

    if (existData(name)) {
        if (!initData(name)) {
            cerr << "Fail to init data." << endl;
        }
        // 然后加载文件系统数据
        deserialize(getData());
        setSpaceSize(this->space_size);
        setBlockSize(this->block_size);
        setAvailable(&available, this->block_data);
    } else {
        // 默认10倍空闲分区表的空间储存结构
        this->block_data = 10 * (size / block) / block;
        if (block_data < this->space_size * 0.02 / block) {
            // 采用2%的空间储存结构
            this->block_data = (int) (this->space_size * 0.02 / block);
        }
        setSpaceSize(size);
        setBlockSize(block);
        // 初始化数据
        if (!initData(name)) {
            cerr << "Fail to init data." << endl;
        }
        // 建立空闲分区表
        available.assign(space_size / block_size, false);
        for (int i = 0; i < block_data; i++) {
            available[i] = true;
        }
        setAvailable(&available, block_data);
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
    serializeRewrite();
    serialize(getData());
    closeData();
}

void filesystem::serializeRewrite() const {
    for (int i = 0; i < block_data; i++) {
        char *tmp = new char[block_size]{0};
        writeBlock(i, tmp, block_size);
    }
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
    for (int i = 0; i < availableSize; i++) {
        bool value;
        in.read(reinterpret_cast<char *>(&value), sizeof(value));
        available[i] = value;
    }
}

string filesystem::getCurrentPath() {
    return concat(this->current);
}

directory *filesystem::getTree() {
    return &this->tree;
}

directory *filesystem::getFatherByName(directory dir) {
    return findParentDirectory(&tree, dir);
}

// 递归查找目录的父目录
directory *filesystem::findParentDirectory(directory *current, directory &target) {
    // 检查当前目录的子目录中是否有目标目录
    for (directory &child: *current->getDirectories()) {
        if (child == target) {
            return current;
        }
        // 递归检查子目录
        directory *found = findParentDirectory(&child, target);
        if (found != nullptr) {
            return found;
        }
    }
    return nullptr;
}

directory *filesystem::getFatherByName(file f) {
    return findParentDirectory(&tree, f);
}

// 递归查找包含指定文件的父目录
directory *filesystem::findParentDirectory(directory *current, file &target) {
    // 检查当前目录是否包含目标文件
    for (file &childFile: *current->getFiles()) {
        if (childFile == target) {
            return current;
        }
    }

    // 递归检查子目录
    for (directory &childDir: *current->getDirectories()) {
        directory *found = findParentDirectory(&childDir, target);
        if (found != nullptr) {
            return found;
        }
    }
    return nullptr;
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
    for (auto dir: *tmp->getDirectories()) {
        List l = {dir.getName(), 0, "目录", "time"};
        v.push_back(l);
    }
    for (auto file: *tmp->getFiles()) {
        List l = {file.getName(), file.getSize(), "文件", "time"};
        v.push_back(l);
    }
    return true;
}

int filesystem::disk(bool left) {
    if (left) {
        int sum = 0;
        for (int i = block_data; i < available.size(); i++) {
            if (available[i]) {
                sum++;
            }
        }
        return sum * block_size;
    } else {
        return (available.size() - block_data) * block_size;
    }
}

bool filesystem::cd(string path) {
    vector<string> names;
    if (!getAbsolutePath(std::move(path), names)) {
        return false;
    }
    this->current = names;
    return true;
}

bool filesystem::mkdir(directory d, const string &dname) {
    if (hasSameName(d, dname)) {
        return false;
    }
    directory dir(dname, d.getName(), d.getUser());
    getDirectoryByDirectory(d)->addDirectory(dir);
    return true;
}

bool filesystem::rm(file f) {
    directory *father = getFatherByName(f);
    if (father->removeFile(f.getName())) {
        return true;
    }
    return false;
}

bool filesystem::rm(directory d) {
    directory *father = getFatherByName(d);
    if (father->removeDirectory(d.getName())) {
        return true;
    }
    return false;
}

vector<user> filesystem::usrs() {
    return this->users;
}

bool filesystem::userdel(int uid) {
    for (int i = 0; i < users.size(); i++) {
        if (users[i].getUid() == uid) {
            users.erase(users.begin() + i);
            return true;
        }
    }
    return false;
}

int filesystem::useradd(string name, string password, bool super) {
    // 判断是否已经有该用户了
    for (auto u: this->users) {
        if (u.getName() == name) {
            return -1;
        }
    }
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

user filesystem::userbyid(int uid) {
    for (auto u: users) {
        if (u.getUid() == uid) {
            return u;
        }
    }
    return {};
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

directory *filesystem::getDirectoryByDirectory(const directory &dir) {
    return findDirectory(&tree, dir);
}

directory *filesystem::findDirectory(directory *current, const directory &target) {
    if (*current == target) {
        return current;
    }
    for (auto &subDir: *current->getDirectories()) {
        directory *result = findDirectory(&subDir, target);
        if (result) {
            return result;
        }
    }
    return nullptr;
}

bool filesystem::hasSameName(directory dir, string name) {
    for (auto d: *dir.getDirectories()) {
        if (d.getName() == name) {
            return true;
        }
    }
    for (auto f: *dir.getFiles()) {
        if (f.getName() == name) {
            return true;
        }
    }
    return false;
}