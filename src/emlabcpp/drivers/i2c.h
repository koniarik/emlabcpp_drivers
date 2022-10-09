#include "emlabcpp/protocol/message.h"
#include "emlabcpp/static_function.h"
#include "emlabcpp/static_vector.h"

#include <array>
#include <cstdint>
#include <emlabcpp/experimental/request_reply.h>
#include <span>

#pragma once

namespace emlabcpp::drivers
{

static constexpr std::size_t driver_callback_size = 48;

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

struct i2c_read_reg_query
{
        uint8_t addr;
        uint8_t reg;
        uint8_t size;
};

struct i2c_write_reg_query
{
        uint8_t                    addr;
        uint8_t                    reg;
        std::span< const uint8_t > data;
};

template < std::size_t N >
struct i2c_write_reg_blob
{
        uint8_t                addr;
        uint8_t                reg;
        protocol::message< N > data;

        i2c_write_reg_query get_query()
        {
                return { .addr = addr, .reg = reg, .data = data };
        }
};

using i2c_coroutine = request_reply<
    std::span< const uint8_t >,
    std::variant< std::monostate, i2c_read_reg_query, i2c_write_reg_query > >;
using i2c_coroutine_handle = typename i2c_coroutine::handle;

class i2c_awaiter_interface
{
public:
        virtual bool store_read( uint8_t reg, std::span< const uint8_t > data ) = 0;
        virtual ~i2c_awaiter_interface()                                        = default;
};

class i2c_read_awaiter
{
        i2c_awaiter_interface& driver;
        i2c_read_reg_query     q;
        i2c_coroutine_handle   h = {};

public:
        i2c_read_awaiter( i2c_awaiter_interface& dr, i2c_read_reg_query rq )
          : driver( dr )
          , q( rq )
        {
        }

        constexpr bool await_ready()
        {
                return false;
        }

        void await_suspend( i2c_coroutine_handle handle )
        {
                h                  = handle;
                h.promise().output = q;
        }

        [[nodiscard]] bool await_resume()
        {
                return driver.store_read( q.reg, *h.promise().input );
        }
};

template < std::size_t N >
struct i2c_write_awaiter
{
        i2c_write_reg_blob< N > blob;

        constexpr bool await_ready()
        {
                return false;
        }

        void await_suspend( i2c_coroutine_handle handle )
        {
                handle.promise().output = blob.get_query();
        }

        void await_resume()
        {
        }
};

struct i2c_noop_awaiter
{
        constexpr bool await_ready()
        {
                return false;
        }

        void await_suspend( i2c_coroutine_handle handle )
        {
                handle.promise().output = std::monostate{};
        }

        void await_resume()
        {
        }
};

}  // namespace emlabcpp::drivers
