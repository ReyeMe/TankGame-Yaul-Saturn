#include <stdio.h>
#include <stdlib.h>
#include <yaul.h>
#include "..\Dependencies\HyperionEngine\ECS\Entity.hpp"
#include "..\Dependencies\Skathi\Skathi.hpp"
#include "Components/InputComponent.hpp"
#include "Components/TransformComponent.hpp"
#include "Systems/BaseSystem.hpp"
#include "Systems/InputSystem.hpp"
#include "Systems/PhysicsSystem.hpp"

extern "C"
{
#include <mic3d.h>
};

/** @brief Shading table
 */
vdp1_gouraud_table_t pool_shading_tables[CMDT_COUNT] __aligned(16);
vdp1_gouraud_table_t pool_shading_tables2[512] __aligned(16);

/** @brief VBlank-out handler
 */
static void
vblank_out_handler(void *work __unused)
{
    smpc_peripheral_intback_issue();
}

/** @brief Main program entry
 */
void main(void)
{
    dbgio_init();
    dbgio_dev_default_init(DBGIO_DEV_VDP2_ASYNC);
    dbgio_dev_font_load();

    /* NOT NEEDED IN THIS EARLY STAGE
    // Initialize 3D
    mic3d_init();

    // Initialize camera
    camera_t camera __unused;
    camera.position.x = FIX16(0.0f);
    camera.position.y = FIX16(0.0f);
    camera.position.z = FIX16(-30.0f);
    camera.target.x = FIX16_ZERO;
    camera.target.y = FIX16_ZERO;
    camera.target.z = FIX16_ZERO;
    camera.up.x = FIX16_ZERO;
    camera.up.y = -FIX16_ONE;
    camera_lookat(&camera);

    // Initialize shading
    vdp1_vram_partitions_t vdp1_vram_partitions;
    vdp1_vram_partitions_get(&vdp1_vram_partitions);
    light_gst_set(pool_shading_tables, CMDT_COUNT, (vdp1_vram_t)(vdp1_vram_partitions.gouraud_base + 512));

    for (uint32_t i = 0; i < 512; i++)
    {
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
    */

    Utenyaa::Components::Transform transform = Utenyaa::Components::Transform();
    fix16_mat43_identity(&transform.Matrix);

    Entity::Create(Utenyaa::Components::InputComponent::Input { Source : Utenyaa::Components::InputComponent::P1 },
                   transform);

    //Skathi::Cd::Initialize();

    while (true)
    {
        dbgio_puts("[H[2J");
        
        // Fetch input
        Skathi::Input::Peripherals::FetchAll();

        // Process entity components
        Utenyaa::Systems::InputSystem::Process();
        Utenyaa::Systems::PhysicsSystem::Process();

        // Debug print entities
        Entity::ForEach(
            [](Utenyaa::Components::Transform &v)
            {
                fix16_vec3 lastLocation = { v.Matrix.frow[0][3], v.Matrix.frow[1][3], v.Matrix.frow[2][3] };
                fix16_vec3 forward = { v.Matrix.frow[0][0], v.Matrix.frow[0][1], v.Matrix.frow[0][2] };

                dbgio_printf("Position x:%f y:%f z:%f\n", lastLocation.x, lastLocation.y, lastLocation.z);
                dbgio_printf("Dir x:%f y:%f z:%f\n", forward.x, forward.y, forward.z);
            });

        // Start rendering to screen
        //render();

        //vdp1_sync_render();
        //vdp1_sync();
        //vdp1_sync_wait();
        dbgio_flush();
        vdp2_sync();
        vdp2_sync_wait();
    }
}

/** @brief User stuff initialization
 */
void user_init(void)
{
    smpc_peripheral_init();
    vdp_sync_vblank_out_set(vblank_out_handler, NULL);

    vdp2_tvmd_display_res_set(VDP2_TVMD_INTERLACE_NONE, VDP2_TVMD_HORZ_NORMAL_B, VDP2_TVMD_VERT_224);

    vdp2_scrn_back_color_set(VDP2_VRAM_ADDR(3, 0x01FFFE), RGB1555(1, 0, 0, 0));

    vdp1_sync_interval_set(-1);

    vdp1_env_default_set();

    vdp2_sprite_priority_set(0, 6);

    vdp2_tvmd_display_set();

    vdp2_sync();
    vdp2_sync_wait();
}
