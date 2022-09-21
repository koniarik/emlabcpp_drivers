#include "emlabcpp/drivers/INA219/driver.h"
#include "emlabcpp/drivers/INA219/rpc.h"

#pragma once

namespace emlabcpp::drivers::ina219
{

struct rpc_pack
{
        driver           ina;
        rpc_wrapper_impl rpc_impl;
        rpc_wrapper      rpc_wrap;

        rpc_pack( uint8_t addr, i2c_owning_async_interface& i2c )
          : ina( addr )
          , rpc_impl( ina, i2c )
          , rpc_wrap( rpc_impl )
        {
        }

        rpc_pack( const rpc_pack& )            = delete;
        rpc_pack( rpc_pack&& )                 = delete;
        rpc_pack& operator=( const rpc_pack& ) = delete;
        rpc_pack& operator=( rpc_pack&& )      = delete;
};

}  // namespace emlabcpp::drivers::ina219
