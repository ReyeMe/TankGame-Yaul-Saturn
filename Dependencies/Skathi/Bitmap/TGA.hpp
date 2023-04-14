#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <yaul.h>
#include <tga.h>
#include "Image.hpp"
#include "../Cd.hpp"

namespace Skathi::Bitmap
{
    /** @brief Image loaded from TGA file
     */
    class TGAImage : Image
    {
    private:
        /** @brief Load image
         * @param file image data
         */
        void LoadImage(const uint8_t * file)
        {
            // Read header
            tga_t image;
            assert(tga_read(&image, file) == TGA_FILE_OK);

            // Set resolution
            this->bitmapSize.Height = image.tga_height;
            this->bitmapSize.Width = image.tga_width;

            // Decode palette data
            if (image.tga_bpp == 8)
            {
                this->bitmapFormat = Skathi::Bitmap::ImageFormat::Indexed;
                this->paletteSize = image.tga_cmap_len;

                // Load palette
                this->paletteData = (Color_t*)malloc(sizeof(Color_t) * image.tga_cmap_len);
                tga_cmap_decode(&image, (uint16_t*)this->paletteData);

                // Allocate image data
                this->data = (uint8_t*)malloc(image.tga_height * image.tga_width);
            }
            else
            {
                this->bitmapFormat = Skathi::Bitmap::ImageFormat::RGB;
                this->paletteSize = 0;
                this->paletteData = NULL;
                
                // Allocate image data
                this->data = (uint8_t*)malloc((image.tga_height * image.tga_width) << 1);
            }

            // Decode image data
            (void)tga_image_decode(&image, this->data);
        }

    public:
        /** @brief Construct a new TGAImage object
         *  @param file File data
         */
        TGAImage(const uint8_t * file)
        {
            this->LoadImage(file);
        }

        /** @brief Construct a new TGAImage object
         *  @param file File entry
         */
        TGAImage(const cdfs_filelist_entry_t * file)
        {
            // Read file into a buffer
            uint8_t * buffer = (uint8_t*)malloc(file->size);
            assert(Cd::ReadFile(file, buffer));

            // Load image
            this->LoadImage(buffer);

            // Free resources
            free(buffer);
        }

        /** @brief Construct a new TGAImage object
         *  @param filename File name
         */
        TGAImage(const char * filename) : TGAImage(Cd::FindFileByName(filename))
        {
            // Do nothing here
        }

        /** @brief Destroy the TGAImage object
         */
        ~TGAImage()
        {
            // Do nothing here
        }
    };
}
