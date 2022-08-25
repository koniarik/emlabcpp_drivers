#include "emlabcpp/experimental/static_function.h"
#include "emlabcpp/protocol/message.h"

#include <array>
#include <cstdint>
#include <span>

namespace emlabcpp::drivers
{

struct i2c_interface
{
        virtual bool read(
            uint8_t              addr,
            std::span< uint8_t > data,
            static_function< bool( i2c_interface& ), 16 > ) = 0;
        virtual bool write(
            uint8_t                    addr,
            std::span< const uint8_t > data,
            static_function< bool( i2c_interface& ), 16 > ) = 0;
        virtual ~i2c_interface()                            = 0;
};

}  // namespace emlabcpp::drivers
