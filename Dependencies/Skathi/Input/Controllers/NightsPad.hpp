#pragma once

#include <yaul.h>
#include "../Peripherals.hpp"
#include "../Digital.hpp"

namespace Skathi::Input::Controllers
{
    /** @brief Nights controller (3D pad)
     */
    class NightsPad : public Skathi::Input::Digital<NightsPad>
    {
    public:
        /** @brief Thumb stick axis
         */
        enum class Axis
        {
            /** @brief X axis
             */
            X = 0,

            /** @brief Y axis
             */
            Y = 1,

            /** @brief Right trigger axis
             */
            R = 2,

            /** @brief Left trigger axis
             */
            L = 3,
        };

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
            return NightsPad::IsConnected(Input::Peripherals::GetPeripheral(port));
        }

        /** @brief Get thumb stick axis
         * @param peripheral Peripheral to check
         * @param axis Which axis to get
         * @return uint8_t Axis in range of -127 to +127
         */
        static uint8_t GetAxis(const smpc_peripheral_t * peripheral, NightsPad::Axis axis);

        /** @brief Get thumb stick axis
         * @param port Input port
         * @param axis Which axis to get
         * @return uint8_t Axis in range of 0 to 255
         */
        static uint8_t GetAxis(uint8_t port, NightsPad::Axis axis)
        {
            return NightsPad::GetAxis(Input::Peripherals::GetPeripheral(port), axis);
        }
    };

    /** @brief Is digital peripheral connected
     * @param peripheral Peripheral to check
     * @return true Digital peripheral is connected
     * @return false Digital peripheral is not connected
     */
    bool NightsPad::IsConnected(const smpc_peripheral_t * peripheral)
    {
        return peripheral != NULL &&
            (Input::Peripherals::GetType(peripheral) == Input::Peripherals::PeripheralType::Analog3dPad ||
             Input::Peripherals::GetType(peripheral) == Input::Peripherals::PeripheralType::Gamepad);
    }

    /** @brief Get thumb stick axis
     * @param peripheral Peripheral to check
     * @param axis Which axis to get
     * @return uint8_t Axis in range of -127 to +127
     */
    uint8_t NightsPad::GetAxis(const smpc_peripheral_t * peripheral, NightsPad::Axis axis)
    {
        if (NightsPad::IsConnected(peripheral))
        {
            // Check if nights pad is running in digital only mode
            if (Input::Peripherals::GetType(peripheral) == Input::Peripherals::PeripheralType::Gamepad)
            {
                switch (axis)
                {
                case NightsPad::Axis::X:
                    return NightsPad::IsHeld(peripheral, NightsPad::Button::Left) ? 0xff : (NightsPad::IsHeld(peripheral, NightsPad::Button::Right) ? 0x00 : 0x7F);
                
                case NightsPad::Axis::Y:
                    return NightsPad::IsHeld(peripheral, NightsPad::Button::Up) ? 0xff : (NightsPad::IsHeld(peripheral, NightsPad::Button::Down) ? 0x00 : 0x7F);

                case NightsPad::Axis::R:
                    return NightsPad::IsHeld(peripheral, NightsPad::Button::R) ? 0x00 : 0xff;

                case NightsPad::Axis::L:
                    return NightsPad::IsHeld(peripheral, NightsPad::Button::L) ? 0x00 : 0xff;
                    
                default:
                    return 0x7F;
                }
            }
            else
            {
                return peripheral->data[2 + (int)axis];
            }
        }

        // Center axis in case of XY, and max out axis in case of RL
        return axis == NightsPad::Axis::L || axis == NightsPad::Axis::R ? 0xff : 0x7F;
    }
}
