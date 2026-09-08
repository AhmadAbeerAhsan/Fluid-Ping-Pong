#pragma once


#include <array>
#include <thread>
#include <format>

#include <iostream>
#include <functional>

constexpr int swsrsize{50};

template<typename T>
class SWSRSlidingWindow
{
private:
    std::array<T, swsrsize> m_window{};
    std::atomic<int> write_ptr{0};
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
        int write = write_ptr++;
        m_window[write%swsrsize] = a;
        write++;
        int read = read_ptr.load();
        if (write%swsrsize == read%swsrsize)
        {
            read_ptr++;
        }
        std::cout << std::format(
            "{}: Write:{}, Read:{}\n","SWSRSlidingWindow::Push", write, read
        );
    }

    bool Read(T& destination)
    {
        int read = read_ptr.load();
        int write = write_ptr.load();

        if(write - read > 0)
        {
            destination = m_window[read%swsrsize];
            read_ptr++;
            return true;
        }
        return false;
    }
};
