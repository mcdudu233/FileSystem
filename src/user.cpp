//
// Created by dudu234 on 2024/6/17.
//

#include "user.h"

user user_root("root", "root", true);

user::user() {
}

user::user(std::string name, std::string password, bool superuser) {
    this->name = name;
    this->password = password;
    this->superuser = superuser;
}

user::~user() {}

bool user::changePassword(std::string password) {
    this->password = password;
    return true;
}

int user::getUid() {
    return this->uid;
}

void user::serialize(fstream &out) const {
    size_t nameLength = name.size();
    out.write(reinterpret_cast<const char *>(&nameLength), sizeof(nameLength));
    out.write(name.c_str(), nameLength);

    size_t passwordLength = password.size();
    out.write(reinterpret_cast<const char *>(&passwordLength), sizeof(passwordLength));
    out.write(password.c_str(), passwordLength);

    out.write(reinterpret_cast<const char *>(&superuser), sizeof(superuser));
    out.write(reinterpret_cast<const char *>(&uid), sizeof(uid));
}

void user::deserialize(fstream &in) {
    size_t nameLength;
    in.read(reinterpret_cast<char *>(&nameLength), sizeof(nameLength));
    name.resize(nameLength);
    in.read(&name[0], nameLength);

    size_t passwordLength;
    in.read(reinterpret_cast<char *>(&passwordLength), sizeof(passwordLength));
    password.resize(passwordLength);
    in.read(&password[0], passwordLength);

    in.read(reinterpret_cast<char *>(&superuser), sizeof(superuser));
    in.read(reinterpret_cast<char *>(&uid), sizeof(uid));
}
