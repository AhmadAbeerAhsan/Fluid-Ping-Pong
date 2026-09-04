#include "LocalPlayerGameEventWindow.hpp"

LocalPlayerGameEventWindow::LocalPlayerGameEventWindow(/* args */)
{
}

LocalPlayerGameEventWindow::~LocalPlayerGameEventWindow()
{
}

void LocalPlayerGameEventWindow::Push(GameEventData &e)
{
    m_widow[ m_write_ptr % lpegew_size ] = e;
    m_write_ptr++;
}

void LocalPlayerGameEventWindow::UpdateLag(GameEventData &e, int time_count_now)
{
    if (e.m_time_stamp_now_ms <= m_latest_observered_timestamp_ms)
    {
        return;
    }
    
    for ( const GameEventData& m_e : m_widow )
    {
        if (AreTwins(m_e, e))
        {
            m_latest_observered_timestamp_ms = e.m_time_stamp_now_ms;
            m_lag = (time_count_now - m_latest_observered_timestamp_ms)/2;
            return;
        }
    }
}
