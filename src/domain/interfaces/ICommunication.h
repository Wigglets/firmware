//
// Created by ub on 12/2/25.
//

#ifndef ICOMMUNICATION_H
#define ICOMMUNICATION_H

#pragma once
#include <string>

class ICommunication {
public:
    virtual ~ICommunication() {}
    virtual void init();
    virtual void sendMessage(const std::string& msg);
    virtual bool receiveMessage(std::string& msg);
};

#endif //ICOMMUNICATION_H
