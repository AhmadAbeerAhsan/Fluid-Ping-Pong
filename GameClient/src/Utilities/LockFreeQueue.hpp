#pragma once

#include <thread>
#include <format>

#include <iostream>
#include <functional>

template<typename T>
class Node
{
public:
    Node(){};

    T data;
    std::shared_ptr<Node<T>> Next;
};

template<typename T>
class LockFreeQueue
{
private:
    std::atomic<int> packets{0};
    std::shared_ptr<Node<T>> to_read_node;
    std::shared_ptr<Node<T>> to_write_node;

public:
    LockFreeQueue(/* args */);
    ~LockFreeQueue();

    void Push(T& t);
    bool Read(T& destination);
};

template <typename T>
inline LockFreeQueue<T>::LockFreeQueue()
{
    to_read_node = std::make_shared<Node<T>>();
    to_write_node = to_read_node;
}

template <typename T>
inline LockFreeQueue<T>::~LockFreeQueue()
{
}

template <typename T>
inline void LockFreeQueue<T>::Push(T &t)
{
    to_write_node->data = t;
    to_write_node->Next = std::make_shared<Node<T>>();
    to_write_node = to_write_node->Next;
    packets++;
}

template <typename T>
inline bool LockFreeQueue<T>::Read(T &destination)
{
    int packet = packets.load();

    if (packet > 0)
    {
        destination = to_read_node->data;
        to_read_node = to_read_node->Next;
        packets--;
        return true;
    }
    
    return false;
}
