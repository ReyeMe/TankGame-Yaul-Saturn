#pragma once

#include <yaul.h>

/** @brief Reyes super kit
 */
namespace rkit::Input
{
    /** @brief Controller input wrapper
     */
    class Peripherals
    {
    private:
        /** @brief Number of controller ports
         */
        static const uint8_t Count;

        /** @brief Peripherals
         */
        static smpc_peripheral_t *inputs[];

    public:
        
        /** @brief Peripheral type family
         */
        enum class PeripheralFamily
        {
            /** @brief Digital controllers (control pad, fighting stick, etc...)
             */
            Digital = 0x00,

            /** @brief Analog controllers (3d pad, racing wheel, train controller, etc...)
             */
            Analog = 0x10,

            /** @brief Mouse controllers (Saturn mouse, Gun)
             */
            Pointing = 0x20,

            /** @brief Keyboard controllers (NetLink keyboard)
             */
            KeyBoard = 0x30,

            /** @brief Megadrive controllers (MD mouse, MD gamepads)
             */
            MegaDrive = 0xe0,

            /** @brief Controller not connected, Unknown type
             */
            Unknown = 0xf0,
        };

        /** @brief Peripheral type
         */
        enum class PeripheralType
        {
            /** @brief Generic gamepad
             */
            Gamepad = 0x02,

            /** @brief Racing wheel
             */
            Racing = 0x13,

            /** @brief Mission stick
             */
            AnalogPad = 0x15,

            /** @brief 3D analog pad
             */
            Analog3dPad = 0x16,

            /** @brief NetLink/XBand mouse
             */
            Mouse = 0x23,

            /** @brief Light gun
             */
            Gun = 0x25,

            /** @brief NetLink/XBand keyboard
             */
            Keyboard = 0x34,

            /** @brief 3 button Megadrive controller
             */
            MD3ButtonPad = 0xe1,

            /** @brief 6 button Megadrive controller
             */
            MD6ButtonPad = 0xe2,

            /** @brief Megadrive mouse
             */
            ShuttleMouse = 0xe3,

            /** @brief Controller not connected
             */
            NotConnected = 0xff
        };

        /** @brief Get count of the peripheral input
         *  @return uint8_t Peripheral input count
         */
        static uint8_t GetCount();

        /** @brief Fetch controller input (should be called at start of each game loop)
         */
        static void FetchAll();

        /** @brief Gets connected peripheral type
         *  @param peripheral Peripheral data
         *  @return Peripheral type
         */
        static PeripheralType GetType(const smpc_peripheral * peripheral);

        /** @brief Gets connected peripheral type family
         *  @param peripheral Peripheral data
         *  @return Peripheral type family
         */
        static PeripheralFamily GetFamily(const smpc_peripheral * peripheral);

        /** @brief Gets connected peripheral type
         *  @param port Peripheral port
         *  @return Peripheral type
         */
        static PeripheralType GetType(uint8_t port);

        /** @brief Gets connected peripheral type family
         *  @param port Peripheral port
         *  @return Peripheral type family
         */
        static PeripheralFamily GetFamily(uint8_t port);

        /** @brief Get the connected peripheral at specified port
         *  @param port Peripheral port
         *  @return smpc_peripheral_t* nullptr if not connected
         */
        static smpc_peripheral_t * GetPeripheral(uint8_t port);

        /** @brief Get the Nth connected peripheral
         *  @param peripheral Index of the peripheral
         *  @return smpc_peripheral_t* nullptr if not found
         */
        static smpc_peripheral_t * GetNthConnectedPeripheral(uint8_t peripheral);
    };

    /** @brief Number of controller ports
     */
    const uint8_t Peripherals::Count = 12;

    /** @brief Peripherals
     */
    smpc_peripheral_t *Peripherals::inputs[12];

    /** @brief Get count of the peripheral input
     *  @return uint8_t Peripheral input count
     */
    uint8_t Peripherals::GetCount()
    {
        return Peripherals::Count;
    }

    /** @brief Fetch controller input (should be called at start of each game loop)
     */
    void Peripherals::FetchAll()
    {
        int virtualInput = 0;

        // Get input data
        smpc_peripheral_process();

        // Parse input data
        for (int port = 1; port <= 2; port++)
        {
            const smpc_peripheral_port_t * realPort = smpc_peripheral_raw_port(port);
            const smpc_peripherals_t * subPorts = &realPort->peripherals;

            if (realPort->peripheral != NULL)
            {
                if (realPort->peripheral->connected)
                {
                    Peripherals::inputs[virtualInput] = realPort->peripheral;
                    virtualInput += 6;
                }
                else
                {
                    smpc_peripheral_t * peripheral;

                    TAILQ_FOREACH(peripheral, subPorts, peripherals)
                    {
                        if (peripheral->connected)
                        {
                            Peripherals::inputs[virtualInput] = peripheral;
                        }

                        virtualInput++;
                    }
                }
            }
        }
    }

    /** @brief Gets connected peripheral type
     *  @param peripheral Peripheral data
     *  @return Peripheral type
     */
    Peripherals::PeripheralType Peripherals::GetType(const smpc_peripheral * peripheral)
    {
        if (peripheral != NULL && peripheral->connected)
        {
            return (Peripherals::PeripheralType)peripheral->type;
        }

        return Peripherals::PeripheralType::NotConnected;
    }

    /** @brief Gets connected peripheral type family
     *  @param peripheral Peripheral data
     *  @return Peripheral type family
     */
    Peripherals::PeripheralFamily Peripherals::GetFamily(const smpc_peripheral * peripheral)
    {
        if (peripheral != NULL && peripheral->connected)
        {
            return (Peripherals::PeripheralFamily)(peripheral->type & 0xf0);
        }

        return Peripherals::PeripheralFamily::Unknown;
    }

    /** @brief Gets connected peripheral type
     *  @param port Peripheral port
     *  @return Peripheral type
     */
    Peripherals::PeripheralType Peripherals::GetType(uint8_t port)
    {
        assert(port < Peripherals::Count);
        return Peripherals::GetType(Peripherals::inputs[port]);
    }

    /** @brief Gets connected peripheral type family
     *  @param port Peripheral port
     *  @return Peripheral type family
     */
    Peripherals::PeripheralFamily Peripherals::GetFamily(uint8_t port)
    {
        assert(port < Peripherals::Count);
        return Peripherals::GetFamily(Peripherals::inputs[port]);
    }

    /** @brief Get the connected peripheral at specified port
     *  @param port Peripheral port
     *  @return smpc_peripheral_t* nullptr if not connected
     */
    smpc_peripheral_t * Peripherals::GetPeripheral(uint8_t port)
    {
        assert(port < Peripherals::Count);

        if (port < Peripherals::Count && Peripherals::inputs[port] != NULL && Peripherals::inputs[port]->connected)
        {
            return Peripherals::inputs[port];
        }

        return nullptr;
    }

    /** @brief Get the Nth connected peripheral
     *  @param peripheral Index of the peripheral
     *  @return smpc_peripheral_t* nullptr if not found
     */
    smpc_peripheral_t * Peripherals::GetNthConnectedPeripheral(uint8_t peripheral)
    {
        int count = 0;
        int searched = 0;

        assert(peripheral < Peripherals::Count);

        do
        {
            if (Peripherals::inputs[searched] != NULL && Peripherals::inputs[searched]->connected)
            {
                if (peripheral == count)
                {
                    return Peripherals::inputs[searched];
                }
                
                count++;
            }

        } while (++searched < Peripherals::Count);
        
        return nullptr;
    }
}