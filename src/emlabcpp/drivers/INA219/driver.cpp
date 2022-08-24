#include "emlabcpp/drivers/INA219/driver.h"

#include "emlabcpp/protocol/register_handler.h"

namespace emlabcpp::protocol
{
template < std::endian Endianess >
struct converter< drivers::ina219::config, Endianess >
  : memcpy_converter< drivers::ina219::config, Endianess >
{
        static_assert( sizeof( drivers::ina219::config ) == 2 );
};
}  // namespace emlabcpp::protocol

namespace emlabcpp::drivers::ina219
{

using handler = protocol::register_handler< regmap >;

driver::driver( uint8_t address )
  : address_( address )
{
}

void driver::query( registers reg, i2c_interface& iface )
{
        std::array< uint8_t, 2 > data = { reg, 0 };
        iface.write( address_, std::span{ data.data(), 1 } );
        if ( !iface.read( address_, data ) ) {
                // TODO: report error
                return;
        }
        store_read( reg, data );
}

bool driver::store_read( uint8_t addr, std::span< const uint8_t > data )
{
        std::optional opt_err = handler::insert(
            map_, static_cast< registers >( addr ), view_n( data.data(), data.size() ) );
        return opt_err.has_value();
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

bool driver::set_config( config cfg, i2c_interface& iface )
{
        bool succeeded = write(
            CONFIGURATION_REGISTER, handler::serialize< CONFIGURATION_REGISTER >( cfg ), iface );
        if ( succeeded ) {
                map_.set_val< CONFIGURATION_REGISTER >( cfg );
        }
        return succeeded;
}
bool driver::set_calibration( uint16_t calib, i2c_interface& iface )
{
        bool succeeded = write(
            CALIBRATION_REGISTER, handler::serialize< CALIBRATION_REGISTER >( calib ), iface );
        if ( succeeded ) {
                map_.set_val< CALIBRATION_REGISTER >( calib );
        }
        return succeeded;
}
bool driver::write( registers reg, protocol::message< 2 > data, i2c_interface& iface )
{
        std::array< uint8_t, 3 > buff = { reg, data[0], data[1] };
        return iface.write( address_, buff );
}
}  // namespace emlabcpp::drivers::ina219
