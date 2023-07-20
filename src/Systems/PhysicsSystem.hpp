#pragma once
#include <yaul.h>
#include "../constants.hpp"
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
            // Transformation matrix format
            //   |  0   1   2   3
            // --+----------------
            // 0 | aXx aXy aXz Ox
            // 1 | aYx aYy aYz Oy
            // 2 | aZx aZy aZz Oz

            // Rotate matrix around Z axis
            if (input->Left)
            {
                fix16_mat43_z_rotate(&transform->Matrix, &transform->Matrix, PLAYER_TURN_SPEED);
            }
            else if (input->Right)
            {
                fix16_mat43_z_rotate(&transform->Matrix, &transform->Matrix, -PLAYER_TURN_SPEED);
            }

            // We want to change players position
            if (input->Down || input->Up)
            {
                // Save current player origin
                fix16_vec3 lastLocation = { transform->Matrix.frow[0][3], transform->Matrix.frow[1][3], transform->Matrix.frow[2][3] };

                // Get current forward vector
                fix16_vec3 forward = { transform->Matrix.frow[0][0], transform->Matrix.frow[0][1], transform->Matrix.frow[0][2] };

                // Scale movement vector depending on where we want to move
                if (input->Down)
                {
                    fix16_vec3_scale(-PLAYER_BACKWARD_SPEED, &forward);
                }
                else if (input->Up)
                {
                    fix16_vec3_scale(PLAYER_FORWARD_SPEED, &forward);
                }

                // Test for collisions and move player back
                // !! TODO !!

                // Change player tranform to the new position
                fix16_mat43_translate(&transform->Matrix, &transform->Matrix, &forward);
            }
        }
    };
}