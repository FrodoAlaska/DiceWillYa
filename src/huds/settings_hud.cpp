#include "hud_manager.h"
#include "game_event.h"
#include "resource_database.h"

#include <nikola/nikola_pch.h> 

/// ----------------------------------------------------------------------
/// SettingsButtonID 
enum SettingsButtonID {
  BUTTON_APPLY = 0xEE,
  BUTTON_MENU,
};
/// SettingsButtonID 
/// ----------------------------------------------------------------------

/// ----------------------------------------------------------------------
/// SettingsHUD
struct SettingsHUD {
  nikola::UILayout layout;

  nikola::f32 master_volume = 1.0f; 
  nikola::f32 music_volume  = 0.3f; 
  nikola::f32 sounds_volume = 1.0f;
};

static SettingsHUD s_hud;
/// MenuHUD
/// ----------------------------------------------------------------------

/// ----------------------------------------------------------------------
/// Callbacks

static bool settings_buttons_callback(const nikola::Event& event, const void* dispatcher, const void* listener) {
  switch(event.button->id) {
    case BUTTON_APPLY:
      // @TODO: Save settings file
      break;
    case BUTTON_MENU:
      game_event_dispatch({
        .type     = GAME_EVENT_HUD_CHANGED, 
        .hud_type = (nikola::u32)HUD_MAIN_MENU,
      });      
      break;
  }

  return true;
}

/// Callbacks
/// ----------------------------------------------------------------------

/// ----------------------------------------------------------------------
/// Settings hud functions

void settings_hud_init(nikola::Window* window) {
  // HUD init  
  
  nikola::ui_layout_create(&s_hud.layout, window, resource_database_get(RESOURCE_FONT));
  s_hud.layout.buttons_padding = nikola::Vec2(40.0f, 8.0f);

  // Layout setup 

  nikola::ui_layout_begin(s_hud.layout, nikola::UI_ANCHOR_TOP_CENTER, nikola::Vec2(0.0f, 32.0f));
  nikola::ui_layout_push_text(s_hud.layout, "General Settings", 50.0f, nikola::Vec4(1.0f));
  nikola::ui_layout_end(s_hud.layout);
  
  nikola::ui_layout_begin(s_hud.layout, nikola::UI_ANCHOR_CENTER, nikola::Vec2(0.0f, 68.0f));
  nikola::ui_layout_push_text(s_hud.layout, "Master volume:", 24.0f, nikola::Vec4(1.0f), nikola::Vec2(-52.0f, 0.0f));
  nikola::ui_layout_push_slider(s_hud.layout, 
                                &s_hud.master_volume, 
                                0.0f, 
                                1.0f, 
                                0.1f, 
                                nikola::Vec4(1.0f), 
                                nikola::Vec4(0.0f, 0.0f, 0.0f, 1.0f), 
                                -1, 
                                nikola::Vec2(0.0f, -64.0f));
  nikola::ui_layout_end(s_hud.layout);
  
  nikola::ui_layout_begin(s_hud.layout, nikola::UI_ANCHOR_BOTTOM_CENTER);
  nikola::ui_layout_push_button(s_hud.layout, 
                                "Apply", 
                                30.0f, 
                                nikola::Vec4(1.0f), 
                                nikola::Vec4(0.0f, 0.0f, 0.0f, 1.0f), 
                                nikola::Vec4(0.0f, 0.0f, 0.0f, 1.0f),
                                BUTTON_APPLY,
                                nikola::Vec2(0.0f, -256.0f));
  nikola::ui_layout_end(s_hud.layout);
  
  nikola::ui_layout_begin(s_hud.layout, nikola::UI_ANCHOR_BOTTOM_LEFT);
  nikola::ui_layout_push_button(s_hud.layout, 
                                "Menu", 
                                30.0f, 
                                nikola::Vec4(1.0f), 
                                nikola::Vec4(0.0f, 0.0f, 0.0f, 1.0f), 
                                nikola::Vec4(0.0f, 0.0f, 0.0f, 1.0f),
                                BUTTON_MENU,
                                nikola::Vec2(15.0f, -10.0f));
  nikola::ui_layout_end(s_hud.layout);

  // Listen to events
  nikola::event_listen(nikola::EVENT_UI_BUTTON_CLICKED, settings_buttons_callback);
}

void settings_hud_render() {
  nikola::ui_layout_render(s_hud.layout);
}

/// Settings hud functions
/// ----------------------------------------------------------------------
