#include "emlabcpp/drivers/INA219/driver.h"

#include <gtest/gtest.h>

namespace emlabcpp::drivers
{

TEST( INA219, basic )
{
        ina219::driver d{0x42};

        auto store = [&]( uint8_t addr, std::initializer_list< uint8_t > data ) {
                d.store_read( addr, std::span{ std::data( data ), data.size() } );
        };

        store( 0x00, { 0x01, 0x9F } );
        store( 0x01, { 0x07, 0xD0 } );
        store( 0x02, { 0x5D, 0x98 } );
        store( 0x05, { 0x50, 0x00 } );
        store( 0x04, { 0x27, 0x10 } );
        store( 0x03, { 0x17, 0x66 } );

        EXPECT_FLOAT_EQ( d.get_shunt_voltage(), 0.02f );
        EXPECT_FLOAT_EQ( d.get_bus_voltage(), 11.98f );
        EXPECT_FLOAT_EQ( d.get_calibration(), 20480u );
        EXPECT_FLOAT_EQ( d.get_current(), 10.0 );
        EXPECT_FLOAT_EQ( d.get_power(), 119.8 );

        pretty_printer pp{};
        pp << d;
        std::cout << pp.str();
}

}  // namespace emlabcpp::drivers
