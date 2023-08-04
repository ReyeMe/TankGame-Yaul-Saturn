#pragma once
#include <yaul.h>
#include "../Bitmap/Bitmap.hpp"

extern "C"
{
#include <mic3d.h>
};

/** @brief Wrappers to help with VDP1
 */
namespace Skathi::Vdp1
{
    /** @brief Sprite functions
     */
    class Sprite
    {
    public:
        // TODO
    };
    
    /** @brief Texture utility functions
     */
    class TextureUtils
    {
    public:
        /** @brief Loads RGB image as texture
         * @param image Image to load
         * @param texture Loaded texture
         */
        static size_t LoadTexture(Skathi::Bitmap::Image * image, vdp1_vram_t textureBase, texture_t * texture)
        {
            assert(image != NULL);
            assert(texture != NULL);

            Skathi::Bitmap::ImageInfo_t info;
            image->GetInfo(&info);

            assert(info.Size.Width % 8 == 0)

            texture->size = TEXTURE_SIZE(info.Size.Width, info.Size.Height);
            texture->vram_index = TEXTURE_VRAM_INDEX(textureBase);

            uint8_t * data = image->GetImageData();
            assert(data != NULL);

            int dataSize = (info.Size.Height * info.Size.Width) * (info.PaletteSize > 0 ? 1 : 2);
            scu_dma_transfer(0, (void *)textureBase, data, dataSize);
            scu_dma_transfer_wait(0);

            return dataSize;
        }

        /** @brief Loads paletted image as texture
         * @param image Image to load
         * @param startPaletteColorIndex Index of first color in CRAM to load palette to
         * @param texture Loaded texture
         * @param textureBase Where to load texture to in VRAM
         * @return Size of loaded image data
         */
        static size_t LoadTexture(Skathi::Bitmap::Image * image, uint16_t startPaletteColorIndex, vdp1_vram_t textureBase, texture_t * texture)
        {
            // Load image data
            size_t imageDataSize = TextureUtils::LoadTexture(image, textureBase, texture);

            // Load palette data
            Skathi::Bitmap::ImageInfo_t info;
            image->GetInfo(&info);
            
            scu_dma_transfer(0, (void *)VDP2_CRAM_ADDR(startPaletteColorIndex), image->GetPalette(), info.PaletteSize);
            scu_dma_transfer_wait(0);

            return imageDataSize;
        }
    };
}