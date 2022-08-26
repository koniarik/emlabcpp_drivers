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
        driver( uint8_t address );

        void query( registers reg, i2c_owning_async_interface& );
        bool store_read( uint8_t addr, std::span< const uint8_t > data );

        float    get_current() const;
        float    get_power() const;
        uint16_t get_calibration() const;
        float    get_bus_voltage() const;
        bool     is_conversion_ready() const;
        float    get_shunt_voltage() const;
        config   get_config() const;

        bool set_config( config, i2c_owning_async_interface& );
        bool set_calibration( uint16_t, i2c_owning_async_interface& );

        template < ostreamlike Stream >
        friend auto& operator<<( Stream& os, const driver& d );

private:
        bool write(
            registers reg,
            protocol::message< 2 >,
            i2c_owning_async_interface&,
            i2c_owning_async_write_callback );

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
