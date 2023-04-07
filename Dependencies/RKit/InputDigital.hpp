#pragma once

#include <yaul.h>
#include "InputPeripherals.hpp"

namespace rkit::Input
{
    class Digital
    {
    public:
        /** @brief Digital gamepad buttons
         */
        enum class Button
        {
            /** @brief D-Pad right direction
             */
            Right = 1 << 6,

            /** @brief D-Pad left direction
             */
            Left = 1 << 7,

            /** @brief D-Pad down direction
             */
            Down = 1 << 5,

            /** @brief D-Pad up direction
             */
            Up = 1 << 4,

            /** @brief Start button
             */
            START = 1 << 11,

            /** @brief A button
             */
            A = 1 << 10,

            /** @brief B button
             */
            B = 1 << 8,

            /** @brief C button
             */
            C = 1 << 9,

            /** @brief X button
             */
            X = 1 << 2,

            /** @brief Y button
             */
            Y = 1 << 1,

            /** @brief Z button
             */
            Z = 1 << 0,

            /** @brief Right trigger
             */
            R = 1 << 3,

            /** @brief Left trigger
             */
            L = 1 << 15
        };

        /** @brief Is digital peripheral connected
         * @param peripheral Peripheral to check
         * @return true Digital peripheral is connected
         * @return false Digital peripheral is not connected
         */
        static bool IsConnected(const smpc_peripheral_t * peripheral);

        /** @brief Check if user is holding down a button
         * @param peripheral Connected peripheral
         * @param button Button to check
         * @return true Button was pressed
         * @return false Button was not pressed
         */
        static bool IsHeld(const smpc_peripheral_t * peripheral, Digital::Button button);

        /** @brief Check if user pressed a button
         * @param peripheral Connected peripheral
         * @param button Button to check
         * @return true Button is being held down
         * @return false Button is not held down
         */
        static bool IsDown(const smpc_peripheral_t * peripheral, Digital::Button button);

        /** @brief Check if user released a button
         * @param peripheral Connected peripheral
         * @param button Button to check
         * @return true Button was released
         * @return false Button was not released
         */
        static bool IsUp(const smpc_peripheral_t * peripheral, Digital::Button button);

        /** @brief Is digital peripheral connected
         * @param port Input port
         * @return true Digital peripheral is connected
         * @return false Digital peripheral is not connected
         */
        static bool IsConnected(uint8_t port)
        {
            return Digital::IsConnected(Input::Peripherals::GetPeripheral(port));
        }

        /** @brief Check if user is holding down a button
         * @param port Input port
         * @param button Button to check
         * @return true Button was pressed
         * @return false Button was not pressed
         */
        static bool IsHeld(uint8_t port, Digital::Button button)
        {
            return Digital::IsHeld(Input::Peripherals::GetPeripheral(port), button);
        }

        /** @brief Check if user pressed a button
         * @param port Input port
         * @param button Button to check
         * @return true Button is being held down
         * @return false Button is not held down
         */
        static bool IsDown(uint8_t port, Digital::Button button)
        {
            return Digital::IsDown(Input::Peripherals::GetPeripheral(port), button);
        }

        /** @brief Check if user released a button
         * @param port Input port
         * @param button Button to check
         * @return true Button was released
         * @return false Button was not released
         */
        static bool IsUp(uint8_t port, Digital::Button button)
        {
            return Digital::IsUp(Input::Peripherals::GetPeripheral(port), button);
        }
    };
    
    /** @brief Is digital peripheral connected
     * @param peripheral Peripheral to check
     * @return true Digital peripheral is connected
     * @return false Digital peripheral is not connected
     */
    bool Digital::IsConnected(const smpc_peripheral_t * peripheral)
    {
        return peripheral != NULL &&
            Input::Peripherals::GetFamily(peripheral) == Input::Peripherals::PeripheralFamily::Digital;
    }

    /** @brief Check if user is holding down a button
     * @param peripheral Connected peripheral
     * @param button Button to check
     * @return true Button was pressed
     * @return false Button was not pressed
     */
    bool Digital::IsHeld(const smpc_peripheral_t * peripheral, Digital::Button button)
    {
        return Digital::IsConnected(peripheral) && ((*(uint16_t*)peripheral->data) & (uint16_t)button) != 0;
    }

    /** @brief Check if user pressed a button
     * @param peripheral Connected peripheral
     * @param button Button to check
     * @return true Button is being held down
     * @return false Button is not held down
     */
    bool Digital::IsDown(const smpc_peripheral_t * peripheral, Digital::Button button)
    {
        if (Digital::IsConnected(peripheral))
        {
            uint16_t current = *(uint16_t*)peripheral->data;
            uint16_t last = *(uint16_t*)peripheral->previous_data;
            return (((current ^ last) & current) & (uint16_t)button) != 0;
        }

        return false;
    }

    /** @brief Check if user released a button
     * @param peripheral Connected peripheral
     * @param button Button to check
     * @return true Button was released
     * @return false Button was not released
     */
    bool Digital::IsUp(const smpc_peripheral_t * peripheral, Digital::Button button)
    {
        if (Digital::IsConnected(peripheral))
        {
            uint16_t current = *(uint16_t*)peripheral->data;
            uint16_t last = *(uint16_t*)peripheral->previous_data;
            return (((current ^ last) & last) & (uint16_t)button) != 0;
        }

        return false;
    }
}
