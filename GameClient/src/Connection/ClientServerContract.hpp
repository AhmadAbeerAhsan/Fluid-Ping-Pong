#pragma once

#include <array>  
#include <string_view>
#include <charconv>
 
constexpr size_t buf_size{128};
constexpr size_t match_name_buf_size{32};
constexpr int server_udp_port{13};
constexpr int server_tcp_port{14};

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

inline int ParseIntegerKnownRange(const std::array<char, buf_size>& buf, size_t start, size_t range_size, std::size_t len)
{
    if (start + range_size > len)
        return -1;

    int id = 0;
    auto result = std::from_chars(buf.data() + start, buf.data() + start + range_size, id);

    if (result.ec != std::errc() || result.ptr != buf.data() + start + range_size)
        return -1;

    return id;
}

inline int ParseIntegerTillDeliminator(const std::array<char, buf_size>& buf, size_t start, std::size_t len, char deliminator)
{
    size_t digits_end{start};
    bool deliminator_found{false};
    while (digits_end < len)
    {
        if (buf[digits_end] == deliminator)
        {
            deliminator_found = true;
            break;
        }
        ++digits_end;
    }
    
    int id = 0;
    auto result = std::from_chars(buf.data() + start, buf.data() + digits_end, id);

    if (result.ec != std::errc() || result.ptr != buf.data() + digits_end || !deliminator_found)
        return -1;

    return id;
}

inline int ParseStringTillDeliminator(
    const std::array<char, buf_size>& buf,
    std::size_t start,
    std::size_t len,
    std::array<char, match_name_buf_size>& match_name_buf
)
{
    std::size_t field_end{start};
    bool deliminator_found{false};
    while (field_end < len)
    {
        if (buf[field_end] == contract(Action::Deliminator))
        {
            deliminator_found = true;
            break;
        }
        ++field_end;
    }

    if (!deliminator_found)
        return -1;

    std::size_t field_len{field_end - start};

    // -1 to leave room for the null terminator below
    if (field_len >= match_name_buf_size)
        return -1; // name too long for the destination buffer

    std::memcpy(match_name_buf.data(), buf.data() + start, field_len);
    match_name_buf[field_len] = '\0';

    return static_cast<int>(field_len);
}