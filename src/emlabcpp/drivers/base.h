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
        uint8_t                    address;
        std::array< std::byte, N > data;
};

}  // namespace emlabcpp::drivers
