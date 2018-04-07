#ifndef DEFINE_H
#define DEFINE_H

enum Code {
    C2S_LOGIN = 1,
    C2S_REGISTER,
    C2S_GET_FRIENDS_LIST,
    C2S_ADD_FRIEND,
    C2S_REMOVE_FRIEND,
    C2S_SEND_MSG,

    S2C_LOGIN = 1000,  // result 0成功 1用户id错误 2密码错误
    S2C_REGISTER,   // result 0成功 1注册失败
    S2C_GET_FRIENDS_LIST,  // result 0成功 1获取好友列表失败
    S2C_ADD_FRIEND,  // result 0成功 1已经是好友
    S2C_REMOVE_FRIEND,  // result 0成功 1好友不存在
    S2C_SEND_MSG,  // result 0成功 1发送失败
};

#endif // DEFINE_H
