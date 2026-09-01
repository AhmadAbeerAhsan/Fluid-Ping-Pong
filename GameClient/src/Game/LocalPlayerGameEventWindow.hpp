#pragma once

#include "../Connection/ClientServerContract.hpp"

constexpr size_t lpegew_size{5};
class LocalPlayerGameEventWindow
{
private:
    std::array<GameEventData, lpegew_size> m_widow{};
    size_t m_write_ptr{0};
    int m_latest_observered_timestamp_ms{0};
    int m_lag{0};
public:
    LocalPlayerGameEventWindow(/* args */);
    ~LocalPlayerGameEventWindow();

    void Push(GameEventData& e);
    void UpdateLag(GameEventData& e, int time_count_now);
    int Lag(){ return m_lag; };
};