#include "emlabcpp/drivers/INA219/base.h"
#include "emlabcpp/protocol/register_map.h"

namespace emlabcpp::drivers::ina219
{

using regmap = protocol::register_map<
    std::endian::little,
    register_pair< CONFIGURATION_REGISTER, config >,
    register_pair< SHUNT_VOLTAGE_REGISTER, int16_t >,  // TODO: this is actualy 2's complement,
                                                       // let's do something about it?
    register_pair< BUS_VOLTAGE_REGISTER, bus_voltage_reg >,
    register_pair< POWER_REGISTER, uint16_t >,
    register_pair< CURRENT_REGISTER, int16_t >,  // 2's complement format
    register_pair< CALIBRATION_REGISTER, calibration_reg > >;

}
