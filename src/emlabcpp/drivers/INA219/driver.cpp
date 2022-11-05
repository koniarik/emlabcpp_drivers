#include "emlabcpp/drivers/INA219/driver.h"

#include "emlabcpp/protocol/register_handler.h"

namespace emlabcpp::drivers::ina219
{

using handler = protocol::register_handler< regmap >;

driver::driver( uint8_t address )
  : address_( address )
{
}

bool driver::query(
    registers                     reg,
    i2c_owning_async_interface&   iface,
    callback< void( registers ) > resp )
{
        std::array< uint8_t, 1 > data = { reg };
        return iface.write( address_, data, [this, reg, resp]( i2c_owning_async_interface& iface ) {
                return iface.read(
                    address_,
                    2,
                    [this, reg, resp]( std::span< uint8_t > data, i2c_owning_async_interface& ) {
                            store_read( reg, data );
                            resp( reg );
                            return true;
                    } );
        } );
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

bool driver::set_config( config cfg, i2c_owning_async_interface& iface, callback< void() > cb )
{
        return write(
            CONFIGURATION_REGISTER,
            handler::serialize< CONFIGURATION_REGISTER >( cfg ),
            iface,
            [this, cfg, cb]( i2c_owning_async_interface& ) {
                    map_.set_val< CONFIGURATION_REGISTER >( cfg );
                    cb();
                    return true;
            } );
}
bool driver::set_calibration(
    uint16_t                    calib,
    i2c_owning_async_interface& iface,
    callback< void() >          cb )
{
        return write(
            CALIBRATION_REGISTER,
            handler::serialize< CALIBRATION_REGISTER >( calib ),
            iface,
            [this, calib, cb]( i2c_owning_async_interface& ) {
                    map_.set_val< CALIBRATION_REGISTER >( calib );
                    cb();
                    return true;
            } );
}
bool driver::write(
    registers                       reg,
    protocol::message< 2 >          data,
    i2c_owning_async_interface&     iface,
    i2c_owning_async_write_callback cb )
{
        std::array< uint8_t, 3 > buff = { reg, data[0], data[1] };
        return iface.write( address_, buff, cb );
}
}  // namespace emlabcpp::drivers::ina219
