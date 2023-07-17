#pragma once
#include <yaul.h>

/** @brief Input component
 */
namespace Utenyaa::Components::InputComponent
{
    enum InputSource : uint8_t
    {
        /** @brief Source is set to first controller port
         */
        P1 = 0,

        /** @brief Source is set to second controller port
         */
        P2 = 1,

        /** @brief Source is set to third controller port
         */
        P3 = 2,

        /** @brief Source is set to fourth controller port
         */
        P4 = 3,

        /** @brief Source is set to fifth controller port
         */
        P5 = 4,

        /** @brief Source is set to sixth controller port
         */
        P6 = 5,

        /** @brief Source is set to seventh controller port
         */
        P7 = 6,

        /** @brief Source is set to eighth controller port
         */
        P8 = 7,

        /** @brief Source is set to ninth controller port
         */
        P9 = 8,

        /** @brief Source is set to tenth controller port
         */
        P10 = 9,

        /** @brief Source is set to eleventh controller port
         */
        P11 = 10,

        /** @brief Source is set to twelveth controller port
         */
        P12 = 11,

        /** @brief Source is set to AI controller
         */
        AI = 12,

        /** @brief Source is set to Network controller
         */
        Network = 13,

        /** @brief No control source is present
         */
        NotPresent,
    };

    /** @brief Input component
     */
    struct Input
    {
        /** @brief Input source
         */
        InputSource Source:4;

        /** @brief Right D-Pad direction
         */
        unsigned int Right:1;

        /** @brief Left D-Pad direction
         */
        unsigned int Left:1;

        /** @brief Down D-Pad direction
         */
        unsigned int Down:1;

        /** @brief Up D-Pad direction
         */
        unsigned int Up:1;

        /** @brief Start button is pressed 
         */
        unsigned int Start:1;

        /** @brief A button is pressed
         */
        unsigned int A:1;

        /** @brief C button is pressed
         */
        unsigned int C:1;

        /** @brief B button is pressed
         */
        unsigned int B:1;

        /** @brief Right trigger is pressed
         */
        unsigned int Rt:1;

        /** @brief X button is pressed
         */
        unsigned int X:1;

        /** @brief Y button is pressed
         */
        unsigned int Y:1;

        /** @brief Z button is pressed
         */
        unsigned int Z:1;

        /** @brief Left trigger is pressed 
         */
        unsigned int Lt:1;

    } __aligned(2) struct_name;
}