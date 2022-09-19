#include "emlabcpp/drivers/INA219/base.h"
#include "emlabcpp/drivers/i2c.h"

#include <emlabcpp/experimental/rpc.h>

namespace emlabcpp::drivers::ina219
{

struct driver;

enum rpc_ids : uint8_t
{
        QUERY               = 0x01,
        GET_CURRENT         = 0x02,
        GET_POWER           = 0x03,
        GET_CALIBRATION     = 0x04,
        GET_BUS_VOLTAGE     = 0x05,
        IS_CONVERSION_READY = 0x06,
        GET_SHUNT_VOLTAGE   = 0x07,
        GET_CONFIG          = 0x08,
        SET_CONFIG          = 0x09,
        SET_CALIBRATION     = 0x0A
};

class rpc_wrapper_impl
{
        driver&                     driver_;
        i2c_owning_async_interface& i2c_;

public:
        rpc_wrapper_impl( driver& driver, i2c_owning_async_interface& i2c );

        void query( registers );

        float    get_current() const;
        float    get_power() const;
        uint16_t get_calibration() const;
        float    get_bus_voltage() const;
        bool     is_conversion_ready() const;
        float    get_shunt_voltage() const;
        config   get_config() const;

        bool set_config( config );
        bool set_calibration( uint16_t );
};

using rpc_wrapper = rpc::class_wrapper<
    rpc_wrapper_impl,
    rpc::derive< QUERY, &rpc_wrapper_impl::query >,
    rpc::derive< GET_CURRENT, &rpc_wrapper_impl::get_current >,
    rpc::derive< GET_POWER, &rpc_wrapper_impl::get_power >,
    rpc::derive< GET_CALIBRATION, &rpc_wrapper_impl::get_calibration >,
    rpc::derive< GET_BUS_VOLTAGE, &rpc_wrapper_impl::get_bus_voltage >,
    rpc::derive< IS_CONVERSION_READY, &rpc_wrapper_impl::is_conversion_ready >,
    rpc::derive< GET_SHUNT_VOLTAGE, &rpc_wrapper_impl::get_shunt_voltage >,
    rpc::derive< GET_CONFIG, &rpc_wrapper_impl::get_config >,
    rpc::derive< SET_CONFIG, &rpc_wrapper_impl::set_config >,
    rpc::derive< SET_CALIBRATION, &rpc_wrapper_impl::set_calibration > >;

using rpc_traits = rpc::wrapper_traits< rpc_wrapper >;

using request_group = typename rpc_traits::request_group;
using reply_group   = typename rpc_traits::reply_group;

using request_message_type = typename rpc_traits::request_message_type;
using reply_message_type   = typename rpc_traits::reply_message_type;

}  // namespace emlabcpp::drivers::ina219
