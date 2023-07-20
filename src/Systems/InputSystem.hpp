#pragma once
#include "BaseSystem.hpp"
#include "../Components/InputComponent.hpp"
#include "../../Dependencies/Skathi/Skathi.hpp"

namespace Utenyaa::Systems
{
    /** @brief Input update system
     */
    class InputSystem : public BaseSystem<
        InputSystem, 
        Utenyaa::Components::InputComponent::Input>
    {
    public:
        /** @brief Process single input component
         * @param input Input component data
         */
        static void ProcessEntity(Utenyaa::Components::InputComponent::Input * input)
        {
            if (input->Source != Utenyaa::Components::InputComponent::InputSource::AI &&
                input->Source != Utenyaa::Components::InputComponent::InputSource::NotPresent &&
                Skathi::Input::Controllers::Gamepad::IsConnected((uint8_t)input->Source))
            {
                input->Right = (unsigned int)Skathi::Input::Controllers::Gamepad::IsHeld((uint8_t)input->Source, Skathi::Input::Controllers::Gamepad::Button::Right);
                input->Left = (unsigned int)Skathi::Input::Controllers::Gamepad::IsHeld((uint8_t)input->Source, Skathi::Input::Controllers::Gamepad::Button::Left);
                input->Up = (unsigned int)Skathi::Input::Controllers::Gamepad::IsHeld((uint8_t)input->Source, Skathi::Input::Controllers::Gamepad::Button::Up);
                input->Down = (unsigned int)Skathi::Input::Controllers::Gamepad::IsHeld((uint8_t)input->Source, Skathi::Input::Controllers::Gamepad::Button::Down);

                input->Rt = (unsigned int)Skathi::Input::Controllers::Gamepad::IsHeld((uint8_t)input->Source, Skathi::Input::Controllers::Gamepad::Button::R);
                input->Lt = (unsigned int)Skathi::Input::Controllers::Gamepad::IsHeld((uint8_t)input->Source, Skathi::Input::Controllers::Gamepad::Button::L);

                input->A = (unsigned int)Skathi::Input::Controllers::Gamepad::IsHeld((uint8_t)input->Source, Skathi::Input::Controllers::Gamepad::Button::A);
                input->B = (unsigned int)Skathi::Input::Controllers::Gamepad::IsHeld((uint8_t)input->Source, Skathi::Input::Controllers::Gamepad::Button::B);
                input->C = (unsigned int)Skathi::Input::Controllers::Gamepad::IsHeld((uint8_t)input->Source, Skathi::Input::Controllers::Gamepad::Button::C);

                input->X = (unsigned int)Skathi::Input::Controllers::Gamepad::IsHeld((uint8_t)input->Source, Skathi::Input::Controllers::Gamepad::Button::X);
                input->Y = (unsigned int)Skathi::Input::Controllers::Gamepad::IsHeld((uint8_t)input->Source, Skathi::Input::Controllers::Gamepad::Button::Y);
                input->Z = (unsigned int)Skathi::Input::Controllers::Gamepad::IsHeld((uint8_t)input->Source, Skathi::Input::Controllers::Gamepad::Button::Z);

                input->Start = (unsigned int)Skathi::Input::Controllers::Gamepad::IsHeld((uint8_t)input->Source, Skathi::Input::Controllers::Gamepad::Button::START);
            }
            else if (input->Source == Utenyaa::Components::InputComponent::InputSource::AI)
            {
                // TODO: imploement AI controller
            }
        }
    };
}