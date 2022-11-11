#include "emlabcpp/drivers/INA219/base.h"
#include "emlabcpp/drivers/INA219/map.h"
#include "emlabcpp/drivers/i2c.h"
#include "emlabcpp/protocol/streams.h"

#include <span>

#pragma once

namespace emlabcpp::drivers::ina219
{

class driver : public i2c_awaiter_interface
{
public:
        driver( uint8_t address );

        i2c_read_reg_query query( registers reg );

        bool store_read( uint8_t addr, std::span< const uint8_t > data );

        float    get_current() const;
        float    get_power() const;
        uint16_t get_calibration() const;
        float    get_bus_voltage() const;
        bool     is_conversion_ready() const;
        float    get_shunt_voltage() const;
        config   get_config() const;

        i2c_write_reg_blob< 2 > set_config( config );
        i2c_write_reg_blob< 2 > set_calib( uint16_t );

        template < ostreamlike Stream >
        friend auto& operator<<( Stream& os, const driver& d );

private:
        uint8_t address_;
        regmap  map_;
};

template < ostreamlike Stream >
auto& operator<<( Stream& os, const driver& d )
{
        os << "ina219 driver: " << '\n';
        os << d.map_;

        return os;
}

}  // namespace emlabcpp::drivers::ina219
