#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <yaul.h>

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

        /** @brief Number of color entries in the palette
         */
        uint8_t PaletteSize;
    } ImageInfo_t;

    /** @brief Color data
     */
    typedef struct Color
    {
        union 
        {
            struct
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
            } __packed Components;

            /** @brief Color data
             */
            uint16_t Data;
        };

        /** @brief Construct a new Color object
         */
        Color()
        {
            this->Data = 0x8000;
        }

        /** @brief Construct a new Color object
         * @param data Color data
         */
        Color(uint16_t data)
        {
            this->Data = data;
        }

        /** @brief Construct a new Color object
         * @param r Red component
         * @param g Green component
         * @param b Blue component
         * @param alpha Alpha flag
         */
        Color(uint8_t r, uint8_t g, uint8_t b, uint8_t alpha)
        {
            this->Components.R = r;
            this->Components.G = g;
            this->Components.B = b;
            this->Components.Alpha = alpha;
        }

        /** @brief Convert to unsigned short
         * @return Converted value 
         */
        operator uint16_t() const
        {
            return this->Data;
        }

        /** @brief Assign from unsigned short
         * @param data Color data
         * @return Color value
         */
        Color& operator=(uint16_t data)
        {
            this->Data = data;
            return *this;
        }
    } __packed Color_t;

    /** @brief Image in ARGB1555
     */
    class Image
    {
    protected:
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

        /** @brief 
         */
        uint8_t paletteSize = 0;

        /** @brief Construct a new Image object
         * @param size Image size
         * @param format Image format
         */
        Image()
        {
            this->bitmapSize.Height = 0;
            this->bitmapSize.Width = 0;
            this->bitmapFormat = Bitmap::ImageFormat::Indexed;
            this->paletteData = NULL;
            this->data = NULL;
        }

    public:
        /** @brief Construct a new Image object
         * @param width Image width
         * @param height Image height
         * @param format Image format
         */
        Image(uint32_t width, uint32_t height, ImageFormat format)
        {
            this->bitmapSize.Height = height;
            this->bitmapSize.Width = width;
            this->bitmapFormat = format;
            this->paletteSize = 0;

            if (format == Bitmap::ImageFormat::Indexed)
            {
                this->paletteData = (Color_t*)malloc(sizeof(Color_t) * 255);
                this->paletteSize = 255;
            }

            this->data = (uint8_t*)malloc((height * width) << (uint8_t)format);
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
            result->Format = this->bitmapFormat;
            result->Size.Height = this->bitmapSize.Height;
            result->Size.Width = this->bitmapSize.Width;
            result->PaletteSize = this->paletteSize;
        }

        /** @brief Get the Palette object
         * @return Palette data
         */
        Color_t * GetPalette()
        {
            return this->paletteData;
        }

        /** @brief Get the image data
         * @return Raw image data
         */
        uint8_t * GetImageData()
        {
            return (uint8_t*)this->data;
        }

        /** @brief Set the pixel color index
         * @param x X coordinate
         * @param y Y coordinate
         * @param colorIndex Color index 
         */
        void SetPixel(uint32_t x, uint32_t y, uint8_t colorIndex)
        {
            assert(this->paletteData != NULL);
            this->data[(this->bitmapSize.Height * y) + x] = colorIndex;
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
            assert(this->paletteData == NULL);
            uint32_t pixel = ((this->bitmapSize.Height * y) + x) << 1;
            ((Color_t*)this->data)[pixel] = Color_t(r, g, b, a);
        }

        /** @brief Set the RGB pixel
         * @param x X coordinate
         * @param y Y coordinate
         * @param Pixel color 
         */
        void SetPixel(uint32_t x, uint32_t y, const Color_t * color)
        {
            this->SetPixel(x, y, color->Components.R, color->Components.G, color->Components.B, color->Components.Alpha);
        }

        /** @brief Get the image pixel
         * @param x X coordinate
         * @param y Y coordinate
         * @return Pixel color 
         */
        Color_t GetPixel(uint32_t x, uint32_t y)
        {
            assert(this->data != NULL);

            if (this->bitmapFormat == Bitmap::ImageFormat::Indexed)
            {
                assert(this->paletteData != NULL);
                return this->paletteData[this->data[(this->bitmapSize.Height * y) + x]];
            }
            else
            {
                return ((Color_t*)this->data)[(this->bitmapSize.Height * y) + x];
            }
        }
    };
}