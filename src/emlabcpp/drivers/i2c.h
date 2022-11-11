#include "emlabcpp/protocol/handler.h"
#include "emlabcpp/protocol/message.h"
#include "emlabcpp/static_function.h"
#include "emlabcpp/static_vector.h"

#include <array>
#include <cstdint>
#include <emlabcpp/experimental/coroutine/request_reply.h>
#include <span>

#pragma once

namespace emlabcpp::drivers
{

struct i2c_write_query
{
        uint8_t                    addr;
        std::span< const uint8_t > data;
};

struct i2c_read_query
{
        uint8_t addr;
        uint8_t size;
};

template < std::size_t N >
struct i2c_write_blob
{
        uint8_t                addr;
        protocol::message< N > data;

        i2c_write_query get_query()
        {
                return { .addr = addr, .data = data };
        }
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

using i2c_coroutine_var = std::variant<
    std::monostate,
    i2c_read_reg_query,
    i2c_write_reg_query,
    i2c_write_query,
    i2c_read_query >;
struct i2c_reply
{
        uint32_t                   time;
        std::span< const uint8_t > data = {};
};
using i2c_coroutine        = request_reply< i2c_coroutine_var, i2c_reply >;
using i2c_coroutine_handle = typename i2c_coroutine::handle;

class i2c_awaiter_interface
{
public:
        virtual bool store_read( uint8_t reg, std::span< const uint8_t > data ) = 0;
        virtual ~i2c_awaiter_interface()                                        = default;
};

template < typename QueryType >
class i2c_read_awaiter_base
{
        i2c_awaiter_interface& driver;
        QueryType              q;
        i2c_coroutine_handle   h = {};

public:
        i2c_read_awaiter_base( i2c_awaiter_interface& dr, QueryType rq )
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
                h                   = handle;
                h.promise().request = q;
        }

        [[nodiscard]] bool await_resume()
        {
                return driver.store_read( q.reg, h.promise().reply->data );
        }
};

using i2c_read_awaiter     = i2c_read_awaiter_base< i2c_read_query >;
using i2c_read_reg_awaiter = i2c_read_awaiter_base< i2c_read_reg_query >;

template < typename BlobType >
struct i2c_write_awaiter_base
{
        BlobType blob;

        constexpr bool await_ready()
        {
                return false;
        }

        void await_suspend( i2c_coroutine_handle handle )
        {
                handle.promise().request = blob.get_query();
        }

        void await_resume()
        {
        }
};

template < std::size_t N >
using i2c_write_awaiter = i2c_write_awaiter_base< i2c_write_blob< N > >;
template < std::size_t N >
using i2c_write_reg_awaiter = i2c_write_awaiter_base< i2c_write_reg_blob< N > >;

struct i2c_noop_awaiter
{
        constexpr bool await_ready()
        {
                return false;
        }

        void await_suspend( i2c_coroutine_handle handle )
        {
                handle.promise().request = std::monostate{};
        }

        void await_resume()
        {
        }
};

template < std::endian Endianess, typename... Args >
auto i2c_write_reg( uint8_t addr, uint8_t reg, const Args&... args )
{
        using proto                       = protocol::handler< std::tuple< Args... > >;
        using message                     = typename proto::message_type;
        static constexpr std::size_t size = message::capacity;
        return i2c_write_reg_awaiter< size >{ i2c_write_reg_blob< size >{
            .addr = addr, .reg = reg, .data = proto::serialize( args... ) } };
}

}  // namespace emlabcpp::drivers
