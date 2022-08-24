#include "drivers/INA219/driver.h"

#include "emlabcpp/protocol/register_handler.h"

namespace emlabcpp::drivers::ina219
{

using handler = protocol::register_handler< regmap >;

read_request driver::make_request( registers reg )
{
        return { .address = reg, .count = 2 };
}

bool driver::store_read( uint8_t addr, std::span< std::byte > data )
{
        handler::insert( map_, addr, data );
}

int16_t driver::get_current() const
{
        // TODO: 2s complement?
        return map_.get_val< CURRENT_REGISTER >();
}

uint16_t driver::get_power() const
{
        return map_.get_val< POWER_REGISTER >();
}

uint16_t driver::get_calibration() const
{
        calibration_reg reg = map_.get_val< CALIBRATION_REGISTER >();
        return reg.value;
}

uint16_t driver::get_bus_voltage() const;
{
        bus_voltage_reg reg = map_.get_val< BUS_VOLTAGE_REGISTER >();
        return reg.value;
}
bool driver::is_conversion_ready() const
{
        bus_voltage_reg reg = map_.get_val< BUS_VOLTAGE_REGISTER >();
        return reg.conversion_ready;
}
int16_t driver::get_shunt_voltage() const
{
        // TODO: 2s complement?

        return map_.get_val< SHUNT_VOLTAGE_REGISTER >();
}
config driver::get_config() const
{
        return map_.get_val< CONFIGURATION_REGISTER >();
}

write_request< 2 > driver::set_config( config cfg )
{
        map_.set_val< CONFIGURATION_REGISTER >( cfg );
}
write_request< 2 > driver::set_calibration( uint16_t calib )
{
        map_.set_cal< CALIBRATION_REGISTER >( calib );
}
}  // namespace emlabcpp::drivers::ina219
