#include "emlabcpp/drivers/INA219/base.h"
#include "emlabcpp/protocol/register_map.h"

namespace emlabcpp::protocol
{
template <>
struct proto_traits< drivers::ina219::config >
{
        using value_type                      = drivers::ina219::config;
        static constexpr std::size_t max_size = 2;
        static constexpr std::size_t min_size = 2;
};
}  // namespace emlabcpp::protocol

namespace emlabcpp::drivers::ina219
{

using regmap = protocol::register_map<
    std::endian::big,
    protocol::register_pair< CONFIGURATION_REGISTER, config >,
    protocol::register_pair< SHUNT_VOLTAGE_REGISTER, int16_t >,  // TODO: this is actualy 2's
                                                                 // complement, let's do something
                                                                 // about it?
    protocol::register_pair< BUS_VOLTAGE_REGISTER, uint16_t >,
    protocol::register_pair< POWER_REGISTER, uint16_t >,
    protocol::register_pair< CURRENT_REGISTER, int16_t >,  // 2's complement format
    protocol::register_pair< CALIBRATION_REGISTER, uint16_t > >;

}

