#include "emlabcpp/drivers/INA219/base.h"
#include "emlabcpp/drivers/INA219/map.h"
#include "emlabcpp/drivers/base.h"
#include "emlabcpp/protocol/streams.h"

#include <span>

namespace emlabcpp::drivers::ina219
{
class driver
{
public:
        read_request make_request( registers reg );
        bool         store_read( uint8_t addr, std::span< const uint8_t > data );

        float    get_current() const;
        float    get_power() const;
        uint16_t get_calibration() const;
        float    get_bus_voltage() const;
        bool     is_conversion_ready() const;
        float    get_shunt_voltage() const;
        config   get_config() const;

        write_request< 2 > set_config( config );
        write_request< 2 > set_calibration( uint16_t );

        template < ostreamlike Stream >
        friend auto& operator<<( Stream& os, const driver& d );

private:
        regmap map_;
};

template < ostreamlike Stream >
auto& operator<<( Stream& os, const driver& d )
{
        os << "ina219 driver: " << '\n';
        os << d.map_;

        return os;
}

}  // namespace emlabcpp::drivers::ina219
