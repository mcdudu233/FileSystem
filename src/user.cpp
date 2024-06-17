//
// Created by dudu234 on 2024/6/17.
//

#include "user.h"

user user_root("root", "root", true);

user::user() {}

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
