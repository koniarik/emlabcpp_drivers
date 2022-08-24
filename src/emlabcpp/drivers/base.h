#include "emlabcpp/protocol/message.h"

#include <array>
#include <cstdint>
#include <span>

namespace emlabcpp::drivers
{

struct i2c_interface
{
        virtual bool read( uint8_t addr, std::span< uint8_t > data )        = 0;
        virtual bool write( uint8_t addr, std::span< const uint8_t > data ) = 0;
        virtual ~i2c_interface()                                            = default;
};

}  // namespace emlabcpp::drivers
