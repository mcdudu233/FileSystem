//
// Created by dudu234 on 2024/6/17.
//

#include "user.h"

user user_root(0, "root", "", true);

user::user() {
}

// 构造函数
user::user(int uid, string name, string password, bool superuser) : uid(uid), name(name), superuser(superuser) {
    setPassword(std::move(password));
}
// 析构函数
user::~user() {}

bool user::operator==(const user &other) {
    if (this->uid == other.uid) {
        return true;
    }
    return false;
}

string MD5(const char *mStr);

string user::getPassword() {
    return this->password;
}
// 检查密码是否一致
bool user::checkPassword(string password) {
    string md5 = MD5(password.c_str());
    if (md5 == password || password.empty()) {
        return true;
    } else {
        return false;
    }
}
// 设置密码
bool user::setPassword(string password) {
    if (password.empty()) {
        this->password = "";
    } else {
        this->password = MD5(password.c_str());
    }
    return true;
}
//  获取uid
int user::getUid() {
    return this->uid;
}
// 获取用户名
string user::getName() {
    return this->name;
}
// 设置用户名
bool user::setName(string name) {
    this->name = name;
    return true;
}
// 判断是否为超级用户
bool user::getSuper() {
    return this->superuser;
}
// 设置是否为超级用户
bool user::setSuper(bool super) {
    this->superuser = super;
    return true;
}
// 序列化
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
// 反序列化
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

/* MD5加密函数实现 */
#include <Windows.h>
#include <cmath>
inline unsigned int F(unsigned int X, unsigned int Y, unsigned int Z) {
    return (X & Y) | ((~X) & Z);
}
inline unsigned int G(unsigned int X, unsigned int Y, unsigned int Z) {
    return (X & Z) | (Y & (~Z));
}
inline unsigned int H(unsigned int X, unsigned int Y, unsigned int Z) {
    return X ^ Y ^ Z;
}
inline unsigned int I(unsigned int X, unsigned int Y, unsigned int Z) {
    return Y ^ (X | (~Z));
}
void ROL(unsigned int &s, unsigned short cx) {
    if (cx > 32) cx %= 32;
    s = (s << cx) | (s >> (32 - cx));
    return;
}
void ltob(unsigned int &i) {
    unsigned int tmp = i;//保存副本
    byte *psour = (byte *) &tmp, *pdes = (byte *) &i;
    pdes += 3;//调整指针，准备左右调转
    for (short i = 3; i >= 0; --i) {
        CopyMemory(pdes - i, psour + i, 1);
    }
    return;
}
void AccLoop(unsigned short label, unsigned int *lGroup, void *M) {
    unsigned int *i1, *i2, *i3, *i4, TAcc, tmpi = 0;                             //定义:4个指针； T表累加器； 局部变量
    typedef unsigned int (*clac)(unsigned int X, unsigned int Y, unsigned int Z);//定义函数类型
    const unsigned int rolarray[4][4] = {
            {7, 12, 17, 22},
            {5, 9, 14, 20},
            {4, 11, 16, 23},
            {6, 10, 15, 21}};//循环左移-位数表
    const unsigned short mN[4][16] = {
            {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
            {1, 6, 11, 0, 5, 10, 15, 4, 9, 14, 3, 8, 13, 2, 7, 12},
            {5, 8, 11, 14, 1, 4, 7, 10, 13, 0, 3, 6, 9, 12, 15, 2},
            {0, 7, 14, 5, 12, 3, 10, 1, 8, 15, 6, 13, 4, 11, 2, 9}};//数据坐标表
    const unsigned int *pM = static_cast<unsigned int *>(M);        //转换类型为32位的Uint
    TAcc = ((label - 1) * 16) + 1;                                  //根据第几轮循环初始化T表累加器
    clac clacArr[4] = {F, G, H, I};                                 //定义并初始化计算函数指针数组

    /*一轮循环开始（16组->16次）*/
    for (short i = 0; i < 16; ++i) {
        /*进行指针自变换*/
        i1 = lGroup + ((0 + i) % 4);
        i2 = lGroup + ((3 + i) % 4);
        i3 = lGroup + ((2 + i) % 4);
        i4 = lGroup + ((1 + i) % 4);

        /*第一步计算开始: A+F(B,C,D)+M[i]+T[i+1] 注:第一步中直接计算T表*/
        tmpi = (*i1 + clacArr[label - 1](*i2, *i3, *i4) + pM[(mN[label - 1][i])] + (unsigned int) (0x100000000UL * abs(sin((double) (TAcc + i)))));
        ROL(tmpi, rolarray[label - 1][i % 4]);//第二步:循环左移
        *i1 = *i2 + tmpi;                     //第三步:相加并赋值到种子
    }
    return;
}
string intToHex(unsigned int number) {
    std::stringstream ss;
    ss << std::hex << number;
    return ss.str();
}
string MD5(const char *mStr) {
    unsigned int mLen = strlen(mStr);//计算字符串长度
    if (mLen < 0) return 0;
    unsigned int FillSize = 448 - ((mLen * 8) % 512);   //计算需填充的bit数
    unsigned int FSbyte = FillSize / 8;                 //以字节表示的填充数
    unsigned int BuffLen = mLen + 8 + FSbyte;           //缓冲区长度或者说填充后的长度
    unsigned char *md5Buff = new unsigned char[BuffLen];//分配缓冲区
    CopyMemory(md5Buff, mStr, mLen);                    //复制字符串到缓冲区

    /*数据填充开始*/
    md5Buff[mLen] = 0x80;                           //第一个bit填充1
    ZeroMemory(&md5Buff[mLen + 1], FSbyte - 1);     //其它bit填充0，另一可用函数为FillMemory
    unsigned long long lenBit = mLen * 8ULL;        //计算字符串长度，准备填充
    CopyMemory(&md5Buff[mLen + FSbyte], &lenBit, 8);//填充长度
    /*数据填充结束*/

    /*运算开始*/
    unsigned int LoopNumber = BuffLen / 64;                                      //以16个字为一分组，计算分组数量
    unsigned int A = 0x67452301, B = 0x0EFCDAB89, C = 0x98BADCFE, D = 0x10325476;//初始4个种子，小端类型
    unsigned int *lGroup = new unsigned int[4]{A, D, C, B};                      //种子副本数组,并作为返回值返回
    for (unsigned int Bcount = 0; Bcount < LoopNumber; ++Bcount)                 //分组大循环开始
    {
        /*进入4次计算的小循环*/
        for (unsigned short Lcount = 0; Lcount < 4;) {
            AccLoop(++Lcount, lGroup, &md5Buff[Bcount * 64]);
        }
        /*数据相加作为下一轮的种子或者最终输出*/
        A = (lGroup[0] += A);
        B = (lGroup[3] += B);
        C = (lGroup[2] += C);
        D = (lGroup[1] += D);
    }
    /*转换内存中的布局后才能正常显示*/
    ltob(lGroup[0]);
    ltob(lGroup[1]);
    ltob(lGroup[2]);
    ltob(lGroup[3]);
    delete[] md5Buff;//清除内存并返回
    return intToHex(lGroup[0]) + intToHex(lGroup[1]) + intToHex(lGroup[2]) + intToHex(lGroup[3]);
}