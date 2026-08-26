#pragma once

#include <string>

class State
{
private:
    std::string m_username{};
public:
    State();

    std::string Username(){ return m_username; };
    void SetUsername(std::string username){ m_username = username; };
};
