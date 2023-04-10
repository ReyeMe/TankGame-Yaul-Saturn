#include <stdio.h>
#include <stdlib.h>
#include <yaul.h>
//#include "..\Dependencies\HyperionEngine\ECS\Entity.hpp"
#include "..\Dependencies\Skathi\Skathi.hpp"

extern "C" {    
    #include <mic3d.h>
};

uint32_t fileIndex = 0;
char * text = NULL;

/** @brief Shading table
 */
vdp1_gouraud_table_t pool_shading_tables[CMDT_COUNT] __aligned(16);
vdp1_gouraud_table_t pool_shading_tables2[512] __aligned(16);

/** @brief VBlank-out handler
 */
static void
vblank_out_handler(void *work __unused) {
    smpc_peripheral_intback_issue();
}

/** @brief Main program entry
 */
void
main(void)
{
    //using EntityST = Entity<Position, Velocity>;

    dbgio_init();
    dbgio_dev_default_init(DBGIO_DEV_VDP2_ASYNC);
    dbgio_dev_font_load();

    // Initialize 3D
    mic3d_init();

    // Initialize camera
    camera_t camera __unused;
    camera.position.x = FIX16(  0.0f);
    camera.position.y = FIX16(  0.0f);
    camera.position.z = FIX16(-30.0f);
    camera.target.x = FIX16_ZERO;
    camera.target.y = FIX16_ZERO;
    camera.target.z = FIX16_ZERO;
    camera.up.x =  FIX16_ZERO;
    camera.up.y = -FIX16_ONE;
    camera_lookat(&camera);

    // Initialize shading
    vdp1_vram_partitions_t vdp1_vram_partitions;
    vdp1_vram_partitions_get(&vdp1_vram_partitions);
    light_gst_set(pool_shading_tables, CMDT_COUNT, (vdp1_vram_t)(vdp1_vram_partitions.gouraud_base + 512));

    for (uint32_t i = 0; i < 512; i++) {
        const rgb1555_t color = RGB1555(1,
                                        (uint32_t)fix16_int32_to(fix16_int32_from(i * 31) / (uint32_t)512),
                                        (uint32_t)fix16_int32_to(fix16_int32_from(i * 31) / (uint32_t)512),
                                        (uint32_t)fix16_int32_to(fix16_int32_from(i * 31) / (uint32_t)512));

        pool_shading_tables2[i].colors[0] = color;
        pool_shading_tables2[i].colors[1] = color;
        pool_shading_tables2[i].colors[2] = color;
        pool_shading_tables2[i].colors[3] = color;
    }

    gst_set((vdp1_vram_t)vdp1_vram_partitions.gouraud_base);
    gst_put(pool_shading_tables2, 512);
    gst_unset();

    // Initialize entity
    //EntityST::Create(Position(10, 20), Velocity(1, 2));

    Skathi::Cd::Initialize();

    while (true) {

        dbgio_puts("[H[2J");

        // Get list of files
        cdfs_filelist_t * files = Skathi::Cd::GetAll();

        // Fetch input
        Skathi::Input::Peripherals::FetchAll();

        // Select file
        if (Skathi::Input::Controllers::Gamepad::IsDown((uint8_t)0, Skathi::Input::Controllers::Gamepad::Button::Up))
        {
            fileIndex--;
        }
        else if (Skathi::Input::Controllers::Gamepad::IsDown((uint8_t)0, Skathi::Input::Controllers::Gamepad::Button::Down))
        {
            fileIndex++;
        }

        // Read file
        if (Skathi::Input::Controllers::Gamepad::IsDown((uint8_t)0, Skathi::Input::Controllers::Gamepad::Button::START))
        {
            if (text != NULL)
            {
                free(text);
            }

            text = (char*)malloc(files->entries[fileIndex].size + 1);

            if (Skathi::Cd::ReadFile(&files->entries[fileIndex], (void*)text))
            {
                text[files->entries[fileIndex].size] = '\0'; 
            }
            else
            {
                text = NULL;
            }
        }

        // Draw file list
        fileIndex = min(fileIndex, files->entries_count - 1);

        for (uint32_t file = 0; file < files->entries_count; file++)
        {
            if (file == fileIndex) 
            {
                dbgio_printf(">%s\n", files->entries[file].name);
            }
            else
            {
                dbgio_printf(" %s\n", files->entries[file].name);
            }
        }

        if (text != NULL)
        {
            dbgio_printf("\n%s", text);
        }

        // Start rendering to screen
        render();

        vdp1_sync_render();
        vdp1_sync();
        vdp1_sync_wait();
        dbgio_flush();
        vdp2_sync();
        vdp2_sync_wait();
    }
}

/** @brief User stuff initialization
 */
void
user_init(void)
{
    smpc_peripheral_init();
    vdp_sync_vblank_out_set(vblank_out_handler, NULL);

    vdp2_tvmd_display_res_set(VDP2_TVMD_INTERLACE_NONE, VDP2_TVMD_HORZ_NORMAL_B, VDP2_TVMD_VERT_224);

    vdp2_scrn_back_color_set(VDP2_VRAM_ADDR(3, 0x01FFFE), RGB1555(1, 0, 3, 15));

    vdp1_sync_interval_set(-1);

    vdp1_env_default_set();

    vdp2_sprite_priority_set(0, 6);

    vdp2_tvmd_display_set();

    vdp2_sync();
    vdp2_sync_wait();
}
