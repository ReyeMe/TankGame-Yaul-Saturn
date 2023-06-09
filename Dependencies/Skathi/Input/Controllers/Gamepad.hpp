#pragma once

#include <yaul.h>
#include "../Peripherals.hpp"
#include "../Digital.hpp"

namespace Skathi::Input::Controllers
{
    /** @brief Generic gamepad
     */
    class Gamepad : public Skathi::Input::Digital<Gamepad>
    {
    public:
        /** @brief Is digital peripheral connected
         * @param peripheral Peripheral to check
         * @return true Digital peripheral is connected
         * @return false Digital peripheral is not connected
         */
        static bool IsConnected(const smpc_peripheral_t * peripheral);
        
        /** @brief Is digital peripheral connected
         * @param port Input port
         * @return true Digital peripheral is connected
         * @return false Digital peripheral is not connected
         */
        static bool IsConnected(uint8_t port)
        {
            return Gamepad::IsConnected(Input::Peripherals::GetPeripheral(port));
        }
    };

    /** @brief Is digital peripheral connected
     * @param peripheral Peripheral to check
     * @return true Digital peripheral is connected
     * @return false Digital peripheral is not connected
     */
    bool Gamepad::IsConnected(const smpc_peripheral_t * peripheral)
    {
        return peripheral != NULL &&
            Input::Peripherals::GetType(peripheral) == Input::Peripherals::PeripheralType::Gamepad;
    }
}
