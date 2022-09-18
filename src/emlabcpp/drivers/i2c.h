#include "emlabcpp/protocol/message.h"
#include "emlabcpp/static_function.h"

#include <array>
#include <cstdint>
#include <span>

#pragma once

namespace emlabcpp::drivers
{

static constexpr std::size_t driver_callback_size = 32;

struct i2c_owning_async_interface;

using i2c_owning_async_read_callback = static_function<
    bool( std::span< uint8_t >, i2c_owning_async_interface& ),
    driver_callback_size >;
using i2c_owning_async_write_callback =
    static_function< bool( i2c_owning_async_interface& ), driver_callback_size >;

struct i2c_owning_async_interface
{
        virtual bool read( uint8_t addr, std::size_t size, i2c_owning_async_read_callback ) = 0;
        virtual bool
        write( uint8_t addr, std::span< const uint8_t > data, i2c_owning_async_write_callback ) = 0;
        virtual ~i2c_owning_async_interface() = default;
};

}  // namespace emlabcpp::drivers
