#pragma once
#include <yaul.h>
#include "BaseSystem.hpp"
#include "../Components/InputComponent.hpp"
#include "../Components/TransformComponent.hpp"

namespace Utenyaa::Systems
{
    /** @brief Physics update system 
     */
    class PhysicsSystem : public BaseSystem<
        PhysicsSystem,
        Utenyaa::Components::InputComponent::Input,
        Utenyaa::Components::Transform>
    {
    public:
        /** @brief Process single input component
         * @param input Input component data
         */
        static void ProcessEntity(
            Utenyaa::Components::InputComponent::Input * input, 
            Utenyaa::Components::Transform * transform)
        {
            if (input->Down)
            {
                transform->Y--;
            }
            else if (input->Up)
            {
                transform->Y++;
            }
            
            if (input->Left)
            {
                transform->X--;
            }
            else if (input->Right)
            {
                transform->X++;
            }
        }
    };
}