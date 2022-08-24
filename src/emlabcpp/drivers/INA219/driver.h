
#include "emlabcpp/drivers/base.h"

namespace emlabcpp::drivers::ina219
{
class driver
{
        read_request make_request( registers reg );
        bool         store_read( uint8_t addr, std::span< std::byte > data );

        int16_t  get_current() const;
        uint16_t get_power() const;
        uint16_t get_calibration() const;
        uint16_t get_bus_voltage() const;
        bool     is_conversion_ready() const;
        int16_t  get_shunt_voltage() const;
        config   get_config() const;

        write_request< 2 > set_config( config );
        write_request< 2 > set_calibration( uint16_t );

private:
        regmap map_;
};

}  // namespace emlabcpp::drivers::ina219
