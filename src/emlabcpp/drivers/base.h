#include "emlabcpp/protocol/message.h"

#include <array>
#include <cstdint>

namespace emlabcpp::drivers
{

struct [[nodiscard]] read_request
{
        uint8_t address;
        uint8_t count;
};

template < std::size_t N >
struct [[nodiscard]] write_request
{
        uint8_t                address;
        protocol::message< N > data;
};

}  // namespace emlabcpp::drivers
