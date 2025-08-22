#include "app.h"
#include "entities.h"
#include "resource_database.h"
#include "sound_manager.h"

#include <nikola/nikola_pch.h>
#include <imgui/imgui.h>

/// ----------------------------------------------------------------------
/// App
struct nikola::App {
  // Engine

  nikola::Window* window; 
  nikola::FrameData frame;

  // Resources

  nikola::ResourceGroupID resource_group;

  // Gameplay state
  
  Turn player_turn; 
  Turn* current_turn = nullptr;

  // Transforms

  nikola::Transform plane;
 
  // GUI

  bool has_editor = false;
};
/// App
/// ----------------------------------------------------------------------

/// ----------------------------------------------------------------------
/// App functions 

nikola::App* app_init(const nikola::Args& args, nikola::Window* window) {
  // App init
  nikola::App* app = new nikola::App{};

  // Window init
  app->window = window;

  // Camera init
  
  nikola::CameraDesc cam_desc = {
    .position     = nikola::Vec3(10.0f, 16.0f, 1.0f),
    .target       = nikola::Vec3(-3.0f, 16.0f, 0.0f),
    .up_axis      = nikola::Vec3(0.0f, 1.0f, 0.0f),
    .aspect_ratio = nikola::window_get_aspect_ratio(app->window),
    .move_func    = nullptr,
  };
  nikola::camera_create(&app->frame.camera, cam_desc);

  app->frame.camera.yaw   = -89.4f; 
  app->frame.camera.pitch = -84.0f; 

  // Resources init
  
  app->resource_group = nikola::resources_create_group("app_res", "./");
  nikola::resources_push_dir(app->resource_group, "res");

  resource_database_init(app->resource_group);

  // GUI init
  nikola::gui_init(window);

  // Skybox init
  app->frame.skybox_id = resource_database_get(RESOURCE_SKYBOX);

  // Lights init

  app->frame.dir_light.direction = nikola::Vec3(1.0f, -1.0f, 1.0f);
  app->frame.ambient             = nikola::Vec3(1.0f);

  // Sound manager init
  sound_manager_init();

  // Rules init
  rulebook_init();

  // Turns init
  
  turn_create(&app->player_turn);
  app->current_turn = &app->player_turn;

  // Transforms init

  nikola::transform_translate(app->plane, nikola::Vec3(10.0f, -5.0f, 0.0f));
  nikola::transform_scale(app->plane, nikola::Vec3(16.0f, 1.0f, 16.0f));

  return app;
}

void app_shutdown(nikola::App* app) {
  sound_manager_shutdown();

  nikola::gui_shutdown();
  nikola::resources_destroy_group(app->resource_group); 

  delete app;
}

void app_update(nikola::App* app, const nikola::f64 delta_time) {
  // Quit the application when the specified exit key is pressed
  
  if(nikola::input_key_pressed(nikola::KEY_ESCAPE)) {
    nikola::event_dispatch(nikola::Event{.type = nikola::EVENT_APP_QUIT});
    return;
  }

  // Disable/enable the GUI
  
  if(nikola::input_key_pressed(nikola::KEY_F1)) {
    app->has_editor             = !app->has_editor;
    app->frame.camera.is_active = !app->has_editor;

    nikola::input_cursor_show(app->has_editor);
  }

  // Update the camera
  nikola::camera_update(app->frame.camera);

  // Restart turn
  
  if(app->current_turn->is_farkle && nikola::input_key_pressed(nikola::KEY_R)) {
    nikola::i32 old_points = app->current_turn->points; 
    old_points            /= 2;

    if(old_points < 0) {
      old_points = 0;
    }

    turn_reset(*app->current_turn);
    app->current_turn->points = old_points;

    return;
  }

  // Start turn

  if(app->current_turn->rolls_count > 0 && nikola::input_key_pressed(nikola::KEY_R)) {
    turn_start(*app->current_turn);

    app->current_turn->rolls_count--;
    if(app->current_turn->rolls_count < 0) {
      app->current_turn->rolls_count = 0;
    }
  }

  // Continue turn 

  if(nikola::input_key_pressed(nikola::KEY_C)) {
    turn_continue(*app->current_turn);
  }

  // Bank turn 

  if(nikola::input_key_pressed(nikola::KEY_B)) {
    turn_bank(*app->current_turn);
  }

  // Turn update
 
  turn_process_input(*app->current_turn);
  turn_update(*app->current_turn, (nikola::f32)delta_time);
}
 
static nikola::Vec2 s_pos = nikola::Vec2(500.0f, 280.0f);

void app_render(nikola::App* app) {
  nikola::renderer_begin(app->frame);

  nikola::ResourceID cube_id = resource_database_get(RESOURCE_CUBE);

  // Plane
  nikola::renderer_queue_mesh(cube_id, app->plane, resource_database_get(RESOURCE_MATERIAL_PLANE));
 
  // Dices 

  nikola::ResourceID dice_id = resource_database_get(RESOURCE_DICE);
  for(nikola::sizei i = 0; i < DICES_MAX; i++) {
    nikola::renderer_queue_model(dice_id, app->current_turn->dices[i].transform);
  }

  // Cursor
  nikola::renderer_queue_mesh(cube_id, app->current_turn->cursor, resource_database_get(RESOURCE_MATERIAL_TRANSPARENT));

  nikola::renderer_end();

  // UI

  nikola::batch_renderer_begin();
  nikola::Font* font = nikola::resources_get_font(resource_database_get(RESOURCE_FONT));
   
  if(!app->current_turn->is_farkle) {
    // Instructions

    nikola::batch_render_text(font, "C      - Continue turn", nikola::Vec2(20.0f, 32.0f), 24.0f, nikola::Vec4(1.0f));
    nikola::batch_render_text(font, "B      - Bank the points", nikola::Vec2(20.0f, 56.0f), 24.0f, nikola::Vec4(1.0f));
    nikola::batch_render_text(font, 
                              "R      - Re-roll (Rolls: " + std::to_string(app->current_turn->rolls_count) + ")", 
                              nikola::Vec2(20.0f, 80.0f), 
                              24.0f, 
                              nikola::Vec4(1.0f));
    nikola::batch_render_text(font, "ARROWS - Move", nikola::Vec2(20.0f, 104.0f), 24.0f, nikola::Vec4(1.0f));
    nikola::batch_render_text(font, "SPACE  - Select dice", nikola::Vec2(20.0f, 128.0f), 24.0f, nikola::Vec4(1.0f));

    // Points
    
    nikola::batch_render_text(font, 
                              ("Turn points: " + std::to_string(app->current_turn->eval_points)), 
                              nikola::Vec2(20.0f, 200.0f), 
                              24.0f, 
                              nikola::Vec4(1.0f));
    
    nikola::batch_render_text(font, 
                              ("Unbanked points: " + std::to_string(app->current_turn->unbanked_points)), 
                              nikola::Vec2(20.0f, 224.0f), 
                              24.0f, 
                              nikola::Vec4(1.0f, 0.0f, 0.0f, 1.0f));

    nikola::batch_render_text(font, 
                              "Banked points: " + std::to_string(app->current_turn->points), 
                              nikola::Vec2(20.0f, 248.0f), 
                              24.0f, 
                              nikola::Vec4(0.0f, 1.0f, 0.0f, 1.0f));

  }
  else {
    // Lost

    if(app->current_turn->is_farkle) {
      nikola::batch_render_text(font, "FARKLED!!", nikola::Vec2(100.0f, 32.0f), 24.0f, nikola::Vec4(1.0f, 0.0f, 0.0f, 1.0f));
      nikola::batch_render_text(font, "R - Restart", nikola::Vec2(100.0f, 56.0f), 24.0f, nikola::Vec4(1.0f));
    }
  }
  
  nikola::batch_renderer_end();
}

void app_render_gui(nikola::App* app) {
  if(!app->has_editor) {
    return;
  }

  nikola::gui_begin();
  nikola::gui_debug_info();
  
  nikola::gui_begin_panel("Main");
 
  // Entities 

  if(ImGui::CollapsingHeader("Entities")) {
    nikola::gui_edit_transform("Plane", &app->plane);
  }

  // Lights
  
  if(ImGui::CollapsingHeader("Lights")) {
    nikola::gui_edit_directional_light("Directional", &app->frame.dir_light);

    for(int i = 0; i < app->frame.point_lights.size(); i++) {
      nikola::PointLight* light = &app->frame.point_lights[i];
      nikola::String light_name = ("Point " + std::to_string(i));

      nikola::gui_edit_point_light(light_name.c_str(), light);
    }
    
    for(int i = 0; i < app->frame.spot_lights.size(); i++) {
      nikola::SpotLight* light = &app->frame.spot_lights[i];
      nikola::String light_name = ("Spot " + std::to_string(i));

      nikola::gui_edit_spot_light(light_name.c_str(), light);
    }
 
    ImGui::Separator();
    if(ImGui::Button("Add PointLight")) {
      nikola::Vec3 point_pos = nikola::Vec3(10.0f, 5.0f, 10.0f);
      app->frame.point_lights.push_back(nikola::PointLight(point_pos));
    }
    
    if(ImGui::Button("Add SpotLight")) {
      nikola::Vec3 spot_pos = nikola::Vec3(10.0f, 5.0f, 10.0f);
      app->frame.spot_lights.push_back(nikola::SpotLight());
    }
  }

  // Camera
  
  if(ImGui::CollapsingHeader("Camera")) {
    nikola::gui_edit_camera("Editor Camera", &app->frame.camera); 
    ImGui::DragFloat3("Ambient", &app->frame.ambient[0], 0.1f, 0.0f, 1.0f);
  }

  nikola::gui_end_panel();
  nikola::gui_end();
}

/// App functions 
/// ----------------------------------------------------------------------
