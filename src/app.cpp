#include "app.h"
#include "entities.h"
#include "resource_database.h"
#include "sound_manager.h"
#include "pop_up_manager.h"
#include "game_event.h"
#include "huds/hud_manager.h"
#include "ranking_manager.h"

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
  
  bool can_play  = false;
  bool is_paused = false;
  
  // Transforms

  nikola::Transform plane;

  // GUI

  bool has_editor = false;
};
/// App
/// ----------------------------------------------------------------------

/// ----------------------------------------------------------------------
/// Callbacks

static void game_start_callback(const GameEvent& event, void* dispatcher, void* listener) {
  nikola::App* app = (nikola::App*)listener;

  if(event.hud_type != HUD_GAME) {
    app->can_play  = false;
    app->is_paused = true;

    return;
  }
   
  app->can_play  = true;
  app->is_paused = false;
  
  // Roll the dice and have a new fresh start
  // We also need to make sure we don't farkle (lose) right away
 
  if(event.is_fresh_start) {
    turn_start(*app->current_turn);
    if(app->current_turn->is_farkle) {
      turn_start(*app->current_turn);
    }
  }
}

/// Callbacks
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
    .position     = nikola::Vec3(10.1f, 10.5f, 0.18f),
    .target       = nikola::Vec3(-3.0f, 10.0f, 0.0f),
    .up_axis      = nikola::Vec3(0.0f, 1.0f, 0.0f),
    .aspect_ratio = nikola::window_get_aspect_ratio(app->window),
    .move_func    = nullptr,
  };
  nikola::camera_create(&app->frame.camera, cam_desc);

  app->frame.camera.yaw   = 178.0f; 
  app->frame.camera.pitch = -89.0f; 

  // Resources init
  
  app->resource_group = nikola::resources_create_group("app_res", "./");
  resource_database_init(app->resource_group);

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
  
  // Ranking manager init
  ranking_manager_init();  

  // Turns init
  
  turn_create(&app->player_turn);
  app->current_turn = &app->player_turn;

  // HUDs init
  hud_manager_init(app->current_turn, window);

  // Transforms init

  nikola::transform_translate(app->plane, nikola::Vec3(10.0f, 5.0f, 0.15f));
  nikola::transform_rotate(app->plane, nikola::Vec3(1.0f, 0.0f, 0.0f), 4.7f);
  nikola::transform_scale(app->plane, nikola::Vec3(15.0f));
  
  // Listen to events
  game_event_listen(GAME_EVENT_HUD_CHANGED, game_start_callback, app); 

  return app;
}

void app_shutdown(nikola::App* app) {
  sound_manager_shutdown();
  nikola::resources_destroy_group(app->resource_group); 

  delete app;
}

void app_update(nikola::App* app, const nikola::f64 delta_time) {
  // Quit the application when the specified exit key is pressed
  
  if(nikola::input_key_pressed(nikola::KEY_F4)) {
    nikola::event_dispatch(nikola::Event{.type = nikola::EVENT_APP_QUIT});
    return;
  }

  // Update the camera
  nikola::camera_update(app->frame.camera);

  // Toggle the pause menu

  HUDType current_hud = hud_manager_get_current_hud();
  bool is_valid_hud   = (current_hud == HUD_GAME || current_hud == HUD_PAUSE);
  
  if(is_valid_hud && nikola::input_key_pressed(nikola::KEY_ESCAPE)) {
    app->is_paused   = !app->is_paused;
    HUDType hud_type = app->is_paused ? HUD_PAUSE : HUD_GAME;

    game_event_dispatch({
      .type     = GAME_EVENT_HUD_CHANGED, 
      .hud_type = (nikola::u32)hud_type, 
    });
  }
  
  // Can't play when paused! 
  if(app->is_paused) {
    return;
  }

  // Make sure that the game has started
  if(!app->can_play) {
    return;
  }

  // Turn update
 
  turn_process_input(*app->current_turn);
  turn_update(*app->current_turn, (nikola::f32)delta_time);

  // Pop-ups update
  popup_manager_update();

  // HUDs update
  hud_manager_update();
}
 
void app_render(nikola::App* app) {
  nikola::renderer_begin(app->frame);

  nikola::ResourceID cube_id = resource_database_get(RESOURCE_CUBE);

  // Plane
  nikola::renderer_queue_model(resource_database_get(RESOURCE_TABLE), app->plane);

  if(app->can_play) {
    // Dices 

    nikola::ResourceID dice_id = resource_database_get(RESOURCE_DICE);
    for(nikola::sizei i = 0; i < DICES_MAX; i++) {
      nikola::renderer_queue_model(dice_id, app->current_turn->dices[i].transform);
    }

    // Cursor
    nikola::renderer_queue_mesh(cube_id, app->current_turn->cursor, resource_database_get(RESOURCE_MATERIAL_TRANSPARENT));
  }

  nikola::renderer_end();

  nikola::batch_renderer_begin();
  
  // HUDs render
  hud_manager_render(); 

  // Pop-ups render
  popup_manager_render();
  
  nikola::batch_renderer_end();
}

/// App functions 
/// ----------------------------------------------------------------------
