#pragma once

#include <array>  
#include <string_view>
#include <charconv>
#include <string>
#include <format>

constexpr size_t conn_buf_size{128};
constexpr size_t match_name_buf_size{32};
constexpr int server_udp_port{13};
constexpr int server_tcp_port{14};

using MatchNameBuf  = std::array<char, match_name_buf_size>;
using ConnectionBuf = std::array<char, conn_buf_size>;

enum class Action { Connect, Join, Create, Leave, ListSessions, MatchId, MatchEvent, Deliminator, EndDeliminator, Success, Error, Count }; // Count = number of values ()  
 
// constexpr array of (enum, string) pairs  
constexpr std::array<std::pair<Action, char>, static_cast<size_t>(Action::Count)> client_server_contract {{
    {Action::Connect,       'C'},
    {Action::Create,        'K'}, 
    {Action::Join,          'J'},  
    {Action::Leave,         'L'},
    {Action::ListSessions,  'S'},  
    {Action::MatchId,       'M'},  
    {Action::MatchEvent,    'E'},
    {Action::Deliminator,   '|'},
    {Action::EndDeliminator,'$'},
    {Action::Success,       'Z'},
    {Action::Error,         '#'},
}};  
 
// constexpr function to convert enum to string (C++14+)  
constexpr char contract(Action c) {  
    for (const auto& pair : client_server_contract) {  
        if (pair.first == c) {  
            return pair.second;  
        }  
    }  
    return '#'; // Handle invalid values  
}

inline int ParseIntegerKnownRange(
    const ConnectionBuf& buf,
    size_t start,
    size_t range_size,
    std::size_t len
)
{
    if (start + range_size > len)
        return -1;

    int id = 0;
    auto result = std::from_chars(buf.data() + start, buf.data() + start + range_size, id);

    if (result.ec != std::errc() || result.ptr != buf.data() + start + range_size)
        return -1;

    return id;
}

inline int ParseIntegerTillDeliminator(
    const ConnectionBuf& buf,
    size_t& start_next,
    std::size_t len,
    char deliminator
)
{
    size_t digits_end{start_next};
    bool deliminator_found{false};
    while (digits_end < len)
    {
        if (buf[digits_end] == deliminator)
        {
            deliminator_found = true;
            if(start_next == digits_end)
                return -1;
            break;
        }
        ++digits_end;
    }
    
    int id = 0;
    auto result = std::from_chars(buf.data() + start_next, buf.data() + digits_end, id);

    if (result.ec != std::errc() || result.ptr != buf.data() + digits_end || !deliminator_found)
        return -1;

    start_next = digits_end + 1;
    return id;
}

inline int ParseMatchNameTillDeliminator(
    const ConnectionBuf& buf,
    size_t& start_next,
    std::size_t len,
    MatchNameBuf& match_name_buf,
    char deliminator
)
{
    std::size_t field_end{start_next};
    bool deliminator_found{false};
    while (field_end < len)
    {
        if (buf[field_end] == deliminator)
        {
            deliminator_found = true;
            if(start_next == field_end)
                return -1;
            break;
        }
        ++field_end;
    }

    if (!deliminator_found)
        return -1;

    std::size_t field_len{field_end - start_next};

    // -1 to leave room for the null terminator below
    if (field_len >= match_name_buf_size)
        return -1; // name too long for the destination buffer

    std::memcpy(match_name_buf.data(), buf.data() + start_next, field_len);
    match_name_buf[field_len] = '\0';

    start_next = field_end + 1;

    return static_cast<int>(field_len);
}

inline int ParseConnectionMessageTillDeliminator(
    const ConnectionBuf& buf,
    size_t& start_next,
    std::size_t len,
    ConnectionBuf& dst_connectionBuf_buf,
    char deliminator
)
{
    std::size_t field_end{start_next};
    bool deliminator_found{false};
    while (field_end < len)
    {
        if (buf[field_end] == deliminator)
        {
            deliminator_found = true;
            if(start_next == field_end)
                return -1;
            break;
        }
        ++field_end;
    }

    if (!deliminator_found)
        return -1;

    std::size_t field_len{field_end - start_next};

    // -1 to leave room for the null terminator below
    if (field_len >= conn_buf_size)
        return -1; // name too long for the destination buffer

    std::memcpy(dst_connectionBuf_buf.data(), buf.data() + start_next, field_len);
    dst_connectionBuf_buf[field_len] = '\0';

    start_next = field_end + 1;

    return static_cast<int>(field_len);
}

class GameSessionData
{
private:
    size_t m_match_name_len;
    MatchNameBuf m_match_name_buf;
    int match_id{0};
    int player_count{0};
    int red_score{0};
    int green_score{0};
public:
    GameSessionData(const int new_id, const std::array<char, match_name_buf_size>& match_name_buf, size_t match_name_len) :
        match_id(new_id), m_match_name_buf(match_name_buf), m_match_name_len(match_name_len)
    {
    }
    GameSessionData(const ConnectionBuf &buf, size_t buf_len)
    {
        DecodeBuffer(buf, buf_len);
    }
    GameSessionData(){}

    void IncreamentPlayerCount(){player_count++;}
    void DecreamentPlayerCount(){player_count--;}
    int PlayerCount(){return player_count;}
    const char* MatchName(){ return m_match_name_buf.data(); }

    std::string EncodeBuffer() const
    {
        return std::format(
            "{}{}{}{}{}{}{}{}{}{}{}{}{}{}",
            contract(Action::Success),      contract(Action::Deliminator),
            contract(Action::ListSessions), contract(Action::Deliminator),
            match_id,                       contract(Action::Deliminator),
            m_match_name_buf.data(),        contract(Action::Deliminator),
            player_count,                   contract(Action::Deliminator),
            red_score,                      contract(Action::Deliminator),
            green_score,                    contract(Action::EndDeliminator)
        );
    }
    void DecodeBuffer(const ConnectionBuf &buf, size_t buf_len)
    {
        size_t next_start{4};
        match_id = ParseIntegerTillDeliminator(buf, next_start, buf_len, contract(Action::Deliminator));
        m_match_name_len = ParseMatchNameTillDeliminator(buf, next_start, buf_len, m_match_name_buf, contract(Action::Deliminator));
        player_count = ParseIntegerTillDeliminator(buf, next_start, buf_len, contract(Action::Deliminator));
        red_score = ParseIntegerTillDeliminator(buf, next_start, buf_len, contract(Action::Deliminator));
        green_score = ParseIntegerTillDeliminator(buf, next_start, buf_len, contract(Action::EndDeliminator));
    }
};

class GameEventData
{
public:
    enum PlayerType : int
    {
        Red = 0,
        Green = 1
    };
    int match_id{0};
    int player_type{0};
    int red_score{0};
    int green_score{0};
    int player_pos{0};
    int player_vel{0};
    int time_stamp_sec{0};

    GameEventData() {}

    GameEventData(const ConnectionBuf& buf, size_t buf_len)
    {
        DecodeBuffer(buf, buf_len);
    }

    std::string EncodeBuffer() const
    {
        return std::format(
            "{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}",
            contract(Action::Success),   contract(Action::Deliminator),
            contract(Action::MatchEvent),contract(Action::Deliminator),
            match_id,                    contract(Action::Deliminator),
            player_type,                 contract(Action::Deliminator),
            red_score,                   contract(Action::Deliminator),
            green_score,                 contract(Action::Deliminator),
            player_pos,                  contract(Action::Deliminator),
            player_vel,                  contract(Action::Deliminator),
            time_stamp_sec,              contract(Action::EndDeliminator)
        );
    }

    void DecodeBuffer(const ConnectionBuf& buf, size_t buf_len)
    {
        size_t next_start{4};
        match_id       = ParseIntegerTillDeliminator(buf, next_start, buf_len, contract(Action::Deliminator));
        player_type    = ParseIntegerTillDeliminator(buf, next_start, buf_len, contract(Action::Deliminator));
        red_score      = ParseIntegerTillDeliminator(buf, next_start, buf_len, contract(Action::Deliminator));
        green_score    = ParseIntegerTillDeliminator(buf, next_start, buf_len, contract(Action::Deliminator));
        player_pos     = ParseIntegerTillDeliminator(buf, next_start, buf_len, contract(Action::Deliminator));
        player_vel     = ParseIntegerTillDeliminator(buf, next_start, buf_len, contract(Action::Deliminator));
        time_stamp_sec = ParseIntegerTillDeliminator(buf, next_start, buf_len, contract(Action::EndDeliminator));
    }
};

class ErrorData
{
private:
    size_t m_error_message_len{0};
    ConnectionBuf m_error_message_buf;

public:
    ErrorData() {}

    ErrorData(const std::string& error_message)
    {
        m_error_message_len = std::min(error_message.size(), m_error_message_buf.size());
        std::copy_n(error_message.data(), m_error_message_len, m_error_message_buf.data());
    }

    ErrorData(const ConnectionBuf& buf, size_t buf_len)
    {
        DecodeBuffer(buf, buf_len);
    }

    std::string EncodeBuffer() const
    {
        return std::format(
            "{}{}{}{}{}",
            contract(Action::Error), contract(Action::Deliminator),
            std::string_view(m_error_message_buf.data(), m_error_message_len),
            contract(Action::EndDeliminator), ""
        );
    }

    void DecodeBuffer(const ConnectionBuf& buf, size_t buf_len)
    {
        size_t next_start{2};
        m_error_message_len = ParseConnectionMessageTillDeliminator(
            buf, next_start, buf_len, m_error_message_buf, contract(Action::EndDeliminator)
        );
    }
};