#include <cstdint>
#include <emlabcpp/protocol/converter.h>

#pragma once

namespace emlabcpp::drivers::ina219
{

enum registers : uint8_t
{
        CONFIGURATION_REGISTER = 0x00,
        SHUNT_VOLTAGE_REGISTER = 0x01,
        BUS_VOLTAGE_REGISTER   = 0x02,
        POWER_REGISTER         = 0x03,
        CURRENT_REGISTER       = 0x04,
        CALIBRATION_REGISTER   = 0x05
};

enum bus_voltage_range : uint8_t
{
        BUS_VOLTAGE_16V = 0,
        BUS_VOLTAGE_32V = 1
};

enum pga_gain : uint8_t
{
        PGA_GAIN_1_0 = 0b00,  // +- 40mV
        PGA_GAIN_1_2 = 0b01,  // +- 80mV
        PGA_GAIN_1_4 = 0b10,  // +- 160mV
        PGA_GAIN_1_8 = 0b11   // +- 320mV
};

enum adc_resolution_averaging : uint8_t
{
        ADC_09_BIT_RESOLUTION   = 0b0000,  // 84 us
        ADC_10_BIT_RESOLUTION   = 0b0001,  // 148 us
        ADC_11_BIT_RESOLUTION   = 0b0010,  // 278 us
        ADC_12_BIT_RESOLUTION   = 0b0011,  // 532 us
        ADC_09_BIT_RESOLUTION_2 = 0b0100,  // 84 us
        ADC_10_BIT_RESOLUTION_2 = 0b0101,  // 148 us
        ADC_11_BIT_RESOLUTION_2 = 0b0110,  // 278 us
        ADC_12_BIT_RESOLUTION_2 = 0b0111,  // 532 us
        ADC_12_BIT_RESOLUTION_3 = 0b1000,  // 532 us
        ADC_002_AVERAGING       = 0b1001,  // 1.06 ms
        ADC_004_AVERAGING       = 0b1010,  // 2.13 ms
        ADC_008_AVERAGING       = 0b1011,  // 4.26 ms
        ADC_016_AVERAGING       = 0b1100,  // 8.51 ms
        ADC_032_AVERAGING       = 0b1101,  // 17.02 ms
        ADC_064_AVERAGING       = 0b1110,  // 34.05 ms
        ADC_128_AVERAGING       = 0b1111   // 69.10 ms
};

enum operating_mode : uint8_t
{
        POWER_DOWN               = 0b000,
        SHUNT_VOLTAGE_TRIGGERED  = 0b001,
        BUS_VOLTAGE_TRIGGERED    = 0b010,
        SHUNT_AND_BUS_TRIGGERED  = 0b011,
        ADC_OFF                  = 0b100,
        SHUNT_VOLTAGE_CONTINUOUS = 0b101,
        BUS_VOLTAGE_CONTINUOUS   = 0b110,
        SHUNT_AND_BUS_CONTINUOUS = 0b111
};

#pragma pack( push, 1 )
struct config
{
        bool                     reset : 1;
        bool                     unused : 1;
        bus_voltage_range        bus_vol : 1;
        pga_gain                 pga : 2;
        adc_resolution_averaging bus_adc : 4;
        adc_resolution_averaging shunt_adc : 4;
        operating_mode           mode : 3;
};
#pragma pack( pop )

}  // namespace emlabcpp::drivers::ina219

namespace emlabcpp::protocol
{

template <>
struct proto_traits< drivers::ina219::config >
{
        using value_type                      = drivers::ina219::config;
        static constexpr std::size_t max_size = sizeof( value_type );
        static constexpr std::size_t min_size = sizeof( value_type );
};

template < std::endian Endianess >
struct converter< drivers::ina219::config, Endianess >
{
        using value_type                      = drivers::ina219::config;
        using traits                          = proto_traits< drivers::ina219::config >;
        static constexpr std::size_t max_size = traits::max_size;
        static constexpr std::size_t min_size = traits::min_size;
        using size_type                       = bounded< std::size_t, min_size, max_size >;

        static constexpr size_type
        serialize_at( std::span< uint8_t, max_size > buffer, const drivers::ina219::config& val )
        {
                uint16_t res = static_cast< uint16_t >(
                    val.reset << 15 | val.unused << 14 | val.bus_vol << 13 | val.pga << 11 |
                    val.bus_adc << 7 | val.shunt_adc << 3 | val.mode );

                buffer[0] = static_cast< uint8_t >( res << 8 );
                buffer[1] = static_cast< uint8_t >( res & 0xFF );

                return size_type{};
        }

        static constexpr conversion_result
        deserialize( const std::span< const uint8_t >& buffer, value_type& value )
        {
                if ( buffer.size() < max_size ) {
                        return { 0, &SIZE_ERR };
                }

                uint16_t inpt = static_cast< uint16_t >( buffer[0] << 8 | buffer[1] );

                value.reset   = inpt >> 15;
                value.unused  = inpt >> 14;
                value.bus_vol = static_cast< emlabcpp::drivers::ina219::bus_voltage_range >(
                    ( inpt >> 13 ) & 0b1 );
                value.pga =
                    static_cast< emlabcpp::drivers::ina219::pga_gain >( ( inpt >> 11 ) & 0b11 );
                value.bus_adc = static_cast< emlabcpp::drivers::ina219::adc_resolution_averaging >(
                    ( inpt >> 7 ) & 0b1111 );
                value.shunt_adc =
                    static_cast< emlabcpp::drivers::ina219::adc_resolution_averaging >(
                        ( inpt >> 3 ) & 0b1111 );
                value.mode =
                    static_cast< emlabcpp::drivers::ina219::operating_mode >( inpt & 0xb111 );

                return conversion_result{ max_size };
        }
};
}  // namespace emlabcpp::protocol
