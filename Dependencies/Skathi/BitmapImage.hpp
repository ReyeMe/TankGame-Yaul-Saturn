#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <yaul.h>
#include "Cd.hpp"

/** @brief Bitmap handling
 */
namespace Skathi::Bitmap
{
    /** @brief Image size
     */
    typedef struct
    {
        /** @brief Image width 
         */
        uint32_t Width;

        /** @brief Image height
         */
        uint32_t Height;
    } Size_t;
    
    /** @brief Image pixel format
     */
    enum class ImageFormat
    {
        /** @brief Indexed image
         */
        Indexed = 0,

        /** @brief RGB image
         */
        RGB = 1,
    };

    /** @brief Image info
     */
    typedef struct
    {
        /** @brief Image size
         */
        Size_t Size;

        /** @brief Image format
         */
        ImageFormat Format;
    } ImageInfo_t;

    /** @brief Color data
     */
    typedef struct
    {
        /** @brief Blue color
         */
        unsigned int B:5;

        /** @brief Green color
         */
        unsigned int G:5;

        /** @brief Red color
         */
        unsigned int R:5;

        /** @brief Alpha mask
         */
        unsigned int Alpha:1;
    } __packed Color_t;

    /** @brief Image in ARGB1555
     */
    class Image
    {
    private:
        /** @brief Image size
         */
        Size_t bitmapSize;

        /** @brief Image format
         */
        ImageFormat bitmapFormat;

        /** @brief Image data
         */
        uint8_t * data = NULL;

        /** @brief Palette data
         */
        Color_t * paletteData = NULL;

    public:
        /** @brief Construct a new Image object
         * @param width Image width
         * @param height Image height
         * @param format Image format
         */
        Image(uint32_t width, uint32_t height, ImageFormat format)
        {
            bitmapSize.Height = height;
            bitmapSize.Width = width;
            bitmapFormat = format;

            if (format == Bitmap::ImageFormat::Indexed)
            {
                paletteData = (Color_t*)malloc((height * width) << 1);
            }

            data = (uint8_t*)malloc((height * width) << (uint8_t)format);
        }

        /** @brief Construct a new Image object
         * @param size Image size
         * @param format Image format
         */
        Image(const Size_t * size, ImageFormat format) : Image(size->Width, size->Height, format)
        {
            // Do nothing here
        }

        /** @brief Destroy the Image object
         */
        ~Image()
        {
            if (data != NULL)
            {
                free(data);
            }

            if (paletteData != NULL)
            {
                free(paletteData);
            }
        }

        /** @brief Get the image size and format info
         * @return Current image info 
         */
        void GetInfo(ImageInfo_t * result)
        {
            assert(result != NULL);
            result->Format = bitmapFormat;
            result->Size.Height = bitmapSize.Height;
            result->Size.Width = bitmapSize.Width;
        }

        /** @brief Get the Palette object
         * @return Palette data
         */
        Color_t * GetPalette()
        {
            return paletteData;
        }

        /** @brief Get the image data
         * @return Raw image data
         */
        void * GetImageData()
        {
            return (void*)data;
        }

        /** @brief Set the pixel color index
         * @param x X coordinate
         * @param y Y coordinate
         * @param colorIndex Color index 
         */
        void SetPixel(uint32_t x, uint32_t y, uint8_t colorIndex)
        {
            assert(paletteData != NULL);
            data[(bitmapSize.Height * y) + x] = colorIndex;
        }

        /** @brief Set the RGB pixel
         * @param x X coordinate
         * @param y Y coordinate
         * @param r Red component
         * @param g Green component
         * @param b Blue component
         * @param a Alpha flag
         */
        void SetPixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
        {
            assert(paletteData == NULL);
            uint32_t pixel = ((bitmapSize.Height * y) + x) << 1;

            ((Color_t*)data)[pixel].Alpha = a;
            ((Color_t*)data)[pixel].R = r;
            ((Color_t*)data)[pixel].G = g;
            ((Color_t*)data)[pixel].B = b;
        }

        /** @brief Set the RGB pixel
         * @param x X coordinate
         * @param y Y coordinate
         * @param Pixel color 
         */
        void SetPixel(uint32_t x, uint32_t y, const Color_t * color)
        {
            SetPixel(x, y, color->R, color->G, color->B, color->Alpha);
        }

        /** @brief Get the image pixel
         * @param x X coordinate
         * @param y Y coordinate
         * @return Pixel color 
         */
        Color_t GetPixel(uint32_t x, uint32_t y)
        {
            assert(data != NULL);

            if (bitmapFormat == Bitmap::ImageFormat::Indexed)
            {
                assert(paletteData != NULL);
                return paletteData[data[(bitmapSize.Height * y) + x]];
            }
            else
            {
                return ((Color_t*)data)[(bitmapSize.Height * y) + x];
            }
        }
    };
}