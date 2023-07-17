#pragma once
#include "../../Dependencies/HyperionEngine/ECS/Entity.hpp"

namespace Utenyaa::Systems
{
    /**
     * @brief Base system type
     * @tparam SystemType Target system type
     * @tparam ComponentTypes Supported entity components
     */
    template<class SystemType, class... ComponentTypes>
    class BaseSystem
    {
    public:
        /** @brief Process system call
         */
        static void Process()
        {
            using EntityST = Entity<ComponentTypes...>;
            EntityST::ForEach(
                [](ComponentTypes&... args)
                {
                    SystemType::ProcessEntity(&args...);
                });
        }
    };
}