
#include "emlabcpp/drivers/INA219/rpc.h"

#include "emlabcpp/drivers/INA219/driver.h"

namespace emlabcpp::drivers::ina219
{

rpc_wrapper_impl::rpc_wrapper_impl( driver& driver, i2c_owning_async_interface& i2c )
  : driver_( driver )
  , i2c_( i2c )
{
}

void rpc_wrapper_impl::query( registers regs )
{
        driver_.query( regs, i2c_ );
}

float rpc_wrapper_impl::get_current() const
{
        return driver_.get_current();
}
float rpc_wrapper_impl::get_power() const
{
        return driver_.get_power();
}
uint16_t rpc_wrapper_impl::get_calibration() const
{
        return driver_.get_calibration();
}
float rpc_wrapper_impl::get_bus_voltage() const
{
        return driver_.get_bus_voltage();
}
bool rpc_wrapper_impl::is_conversion_ready() const
{
        return driver_.is_conversion_ready();
}
float rpc_wrapper_impl::get_shunt_voltage() const
{
        return driver_.get_shunt_voltage();
}
config rpc_wrapper_impl::get_config() const
{
        return driver_.get_config();
}

bool rpc_wrapper_impl::set_config( config c )
{
        return driver_.set_config( c, i2c_ );
}
bool rpc_wrapper_impl::set_calibration( uint16_t c )
{
        return driver_.set_calibration( c, i2c_ );
}

}  // namespace emlabcpp::drivers::ina219
