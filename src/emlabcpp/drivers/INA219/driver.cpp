#include "emlabcpp/drivers/INA219/driver.h"

#include "emlabcpp/protocol/register_handler.h"

namespace emlabcpp::drivers::ina219
{

using handler = protocol::register_handler< regmap >;

driver::driver( uint8_t address )
  : address_( address )
{
}

i2c_read_reg_query driver::query( registers reg )
{
        return {
            .addr = address_,
            .reg  = reg,
            .size = 2,
        };
}

bool driver::store_read( uint8_t addr, std::span< const uint8_t > data )
{
        std::optional opt_err = handler::insert(
            map_, static_cast< registers >( addr ), view_n( data.data(), data.size() ) );
        return !opt_err.has_value();
}

float driver::get_current() const
{
        // TODO: 2s complement?
        return map_.get_val< CURRENT_REGISTER >() * 0.001f;
}

float driver::get_power() const
{
        return map_.get_val< POWER_REGISTER >() * 0.020f;
}

uint16_t driver::get_calibration() const
{
        return map_.get_val< CALIBRATION_REGISTER >();
}

float driver::get_bus_voltage() const
{
        uint16_t raw = map_.get_val< BUS_VOLTAGE_REGISTER >();
        return static_cast< float >( raw >> 3 ) * 0.004f;
}
bool driver::is_conversion_ready() const
{
        uint16_t raw = map_.get_val< BUS_VOLTAGE_REGISTER >();
        return raw & 0b0010;
}
float driver::get_shunt_voltage() const
{
        // TODO: 2s complement?

        return map_.get_val< SHUNT_VOLTAGE_REGISTER >() * 0.000010f;
}
config driver::get_config() const
{
        return map_.get_val< CONFIGURATION_REGISTER >();
}

i2c_write_reg_blob< 2 > driver::set_config( config cfg )
{
        map_.set_val< CONFIGURATION_REGISTER >( cfg );
        return i2c_write_reg_blob< 2 >{
            .addr = address_,
            .reg  = CONFIGURATION_REGISTER,
            .data = handler::serialize< CONFIGURATION_REGISTER >( cfg ) };
}

i2c_write_reg_blob< 2 > driver::set_calib( uint16_t val )
{
        map_.set_val< CALIBRATION_REGISTER >( val );
        return i2c_write_reg_blob< 2 >{
            .addr = address_,
            .reg  = CALIBRATION_REGISTER,
            .data = handler::serialize< CALIBRATION_REGISTER >( val ) };
}

}  // namespace emlabcpp::drivers::ina219
