#pragma once


#include <array>
#include <thread>
#include <format>

#include "ClientServerContract.hpp"

constexpr int swsrsize{50};

template<typename T>
class SWSRSlidingWindow
{
private:
    int write_ptr{0};
    std::array<T, swsrsize> m_window{};
    std::atomic<int> packets_ptr{0};
    std::atomic<int> read_ptr{0};

    void loopAddThenMod(std::atomic<int>& atom, int add, int size)
    {
        int expected{atom.load()};
        int desired {(expected + add) % size};
        while (!atom.compare_exchange_strong(expected, desired))
        {
            desired = (expected + add) % size;
        }
    }
public:
    SWSRSlidingWindow(){}

    void Push(T& a)
    {
        m_window[write_ptr++] = a;
        packets_ptr++;
        write_ptr%=swsrsize;
        int read = read_ptr.load();
        if (write_ptr == read)
        {
            packets_ptr--;
            loopAddThenMod(read_ptr, 1, swsrsize);
        }
    }
    bool Read(T& destination)
    {
        int packet = packets_ptr.load();
        if(packet > 0)
        {
            int read = read_ptr.load();
            destination = m_window[read];
            packets_ptr--;
            loopAddThenMod(read_ptr, 1, swsrsize);
            return true;
        }
        return false;
    }
};
