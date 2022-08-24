#include <cstdint>

#pragma once

namespace emlabcpp::drivers::ina219
{

enum registers
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
}

enum pga_gain : uint8_t
{
	PGA_GAIN_1_0 = 0b00, // +- 40mV
	PGA_GAIN_1_2 = 0b01, // +- 80mV
	PGA_GAIN_1_4 = 0b10, // +- 160mV
	PGA_GAIN_1_8 = 0b11  // +- 320mV
}

enum adc_resolution_averaging : uint8_t
{
	ADC_09_BIT_RESOLUTION = 0b0000, // 84 us
	ADC_10_BIT_RESOLUTION = 0b0001, // 148 us
	ADC_11_BIT_RESOLUTION = 0b0010, // 278 us
	ADC_12_BIT_RESOLUTION = 0b0011, // 532 us
	ADC_002_AVERAGING 		= 0b1001, // 1.06 ms
	ADC_004_AVERAGING 		=	0b1010, // 2.13 ms
	ADC_008_AVERAGING			=	0b1011, // 4.26 ms
	ADC_016_AVERAGING			=	0b1100, // 8.51 ms
	ADC_032_AVERAGING			=	0b1101, // 17.02 ms
	ADC_064_AVERAGING			=	0b1110, // 34.05 ms
	ADC_128_AVERAGING 		= 0b1111 // 69.10 ms
}

enum operating_mode : uint8_t
{
	POWER_DOWN 								= 0b000,
	SHUNT_VOLTAGE_TRIGGERED		= 0b001,
	BUS_VOLTAGE_TRIGGERED			= 0b010,
	SHUNT_AND_BUS_TRIGGERED		= 0b011,
	ADC_OFF 									= 0b100,
	SHUNT_VOLTAGE_CONTINUOUS 	= 0b101,
	BUS_VOLTAGE_CONTINUOUS 		= 0b110,
	SHUNT_AND_BUS_CONTINUOUS 	= 0b111
}

struct config
{
        bool                     reset : 1;
        bool                     unused : 1;
        bus_voltage_range        bus_vol : 1;
        pga_gain                 pga : 2;
        adc_resolution_averaging bus_adc : 4;
        adc_resolution_averaging shunt_adc : 4;
        operating                mode : 3;
};
static_assert( sizeof( config ) == 2 );

struct bus_voltage_reg
{
        uint16_t value : 13;
        bool     unused : 1;
        bool     conversion_ready : 1;
        bool     overflow : 1;
};
static_assert( sizeof( power_reg ) == 2 );

struct calibration_reg
{
        uint16_t value : 15;
        bool     unused : 1;
};
static_assert( sizeof( calibration_reg ) == 2 );

}  // namespace emlabcpp::drivers::ina219
