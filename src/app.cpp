#include "app.h"
#include "entities.h"
#include "resource_database.h"
#include "sound_manager.h"
#include "pop_up_manager.h"
#include "game_event.h"

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

  nikola::i32 next_rank_points = 3000;
  nikola::u32 current_rank     = 1;

  // Transforms

  nikola::Transform plane;

  // UI
 
  nikola::UILayout game_hud;
  nikola::UILayout pop_ups;

  // GUI

  bool has_editor = false;
};
/// App
/// ----------------------------------------------------------------------

/// ----------------------------------------------------------------------
/// Private functions

static void init_hud(nikola::App* app) {
  nikola::ui_layout_create(&app->game_hud, app->window, resource_database_get(RESOURCE_FONT));

  // Instructions 

  nikola::ui_layout_begin(app->game_hud, nikola::UI_ANCHOR_BOTTOM_LEFT, nikola::Vec2(0.0f, -24.0f));

  nikola::Vec4 insts_color = nikola::Vec4(1.0f);

  nikola::ui_layout_push_text(app->game_hud, "C      - Continue turn", 24.0f, insts_color);
  nikola::ui_layout_push_text(app->game_hud, "B      - Bank the points", 24.0f, insts_color);
  nikola::ui_layout_push_text(app->game_hud, "R      - Re-roll (Rolls: 0)", 24.0f, insts_color);
  nikola::ui_layout_push_text(app->game_hud, "ARROWS - Move", 24.0f, insts_color);
  nikola::ui_layout_push_text(app->game_hud, "SPACE  - Select dice", 24.0f, insts_color);

  nikola::ui_layout_end(app->game_hud);

  // Scores
  
  nikola::ui_layout_begin(app->game_hud, nikola::UI_ANCHOR_TOP_LEFT, nikola::Vec2(0.0f, 24.0f));

  nikola::ui_layout_push_text(app->game_hud, "Turn points:     0", 24.0f, nikola::Vec4(1.0f));
  nikola::ui_layout_push_text(app->game_hud, "Unbanked points: 0", 24.0f, nikola::Vec4(1.0f, 0.0f, 0.0f, 1.0f));
  nikola::ui_layout_push_text(app->game_hud, "Banked points:   0", 24.0f, nikola::Vec4(0.0f, 1.0f, 0.0f, 1.0f));
  
  nikola::ui_layout_push_text(app->game_hud, "Points left: 0", 24.0f, nikola::Vec4(1.0f));
  nikola::ui_layout_push_text(app->game_hud, "Rank:        0", 24.0f, nikola::Vec4(1.0f));

  nikola::ui_layout_end(app->game_hud);

  // Farkle
  
  nikola::ui_layout_begin(app->game_hud, nikola::UI_ANCHOR_CENTER);
  
  nikola::ui_layout_push_text(app->game_hud, "FARKLED!", 48.0f, nikola::Vec4(1.0f, 0.0f, 0.0f, 1.0f));
  app->game_hud.texts[10].is_active = false; 

  nikola::ui_layout_end(app->game_hud);

  // Pop-ups

  nikola::ui_layout_create(&app->pop_ups, app->window, resource_database_get(RESOURCE_FONT));
  app->pop_ups.is_active = false;

  nikola::ui_layout_begin(app->pop_ups, nikola::UI_ANCHOR_CENTER);
  nikola::ui_layout_push_text(app->pop_ups, "+100", 42.0f, nikola::Vec4(1.0f, 0.0f, 0.0f, 1.0f), nikola::Vec2(0.0f, -128.0f));
  nikola::ui_layout_end(app->pop_ups);
}

/// Private functions
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

  // Pop-ups init
  popup_manager_init(window);

  // Turns init
  
  turn_create(&app->player_turn);
  app->current_turn = &app->player_turn;

  // HUD init
  init_hud(app);

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

  // Turn update
 
  turn_process_input(*app->current_turn);
  turn_update(*app->current_turn, (nikola::f32)delta_time);

  // Pop-ups update
  popup_manager_update();

  // Ranking management 

  if(app->current_turn->points > app->next_rank_points) { // We can level up!
    app->current_rank++;
    app->next_rank_points = app->current_rank * 3000;

    GameEvent event = {
      .type     = GAME_EVENT_RANKED, 
      .new_rank = app->current_rank,
    };
    game_event_dispatch(event);
  }
   
  if(app->next_rank_points < 3000) { // Better clamp the ranking points to a certain range
    app->next_rank_points = 3000;
  }
   
  if(app->current_rank <= 0) { // Clamp the rank
    app->current_rank = 1;
  } 
}
 
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
    nikola::ui_text_set_string(app->game_hud.texts[2], 
                               "R      - Re-roll (Rolls: " + std::to_string(app->current_turn->rolls_count) + ")");

    nikola::ui_text_set_string(app->game_hud.texts[5], 
                               "Turn points: " + std::to_string(app->current_turn->eval_points));
    
    nikola::ui_text_set_string(app->game_hud.texts[6], 
                               "Unbanked points: " + std::to_string(app->current_turn->unbanked_points));
    
    nikola::ui_text_set_string(app->game_hud.texts[7], 
                               "Banked points: " + std::to_string(app->current_turn->points));
    
    nikola::ui_text_set_string(app->game_hud.texts[8], 
                               "Next rank: " + std::to_string(app->next_rank_points));
    
    nikola::ui_text_set_string(app->game_hud.texts[9], 
                               "Rank: " + std::to_string(app->current_rank));
    
    app->game_hud.texts[10].is_active = false; // Farkle text 
    
    nikola::ui_layout_render(app->game_hud);
  }
  else {
    app->game_hud.texts[10].is_active = true; 
    
    nikola::ui_text_apply_animation(app->game_hud.texts[10], nikola::UI_TEXT_ANIMATION_FADE_IN, 1.5f);
    nikola::ui_text_render(app->game_hud.texts[10]);
  }

  // Pop-ups render
  popup_manager_render();
  
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
