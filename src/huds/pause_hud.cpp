#include "hud_manager.h"
#include "game_event.h"
#include "resource_database.h"

#include <nikola/nikola_pch.h> 

/// ----------------------------------------------------------------------
/// PauseButtonID 
enum PauseButtonID {
  BUTTON_RESUME = 0x02,
  BUTTON_INSTRUCTIONS,
  BUTTON_MENU,
};
/// PauseButtonID 
/// ----------------------------------------------------------------------

/// ----------------------------------------------------------------------
/// PauseHUD
struct PauseHUD {
  nikola::UILayout layout;
};

static PauseHUD s_hud;
/// MenuHUD
/// ----------------------------------------------------------------------

/// ----------------------------------------------------------------------
/// Callbacks

static bool pause_buttons_callback(const nikola::Event& event, const void* dispatcher, const void* listener) {
  switch(event.button->id) {
    case BUTTON_RESUME:
      game_event_dispatch({
        .type     = GAME_EVENT_HUD_CHANGED, 
        .hud_type = (nikola::u32)HUD_GAME,
      });      
      break;
    case BUTTON_INSTRUCTIONS:
      game_event_dispatch({
        .type     = GAME_EVENT_HUD_CHANGED, 
        .hud_type = (nikola::u32)HUD_INSTRUCTIONS,
      });      
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
/// Pause hud functions

void pause_hud_init(nikola::Window* window) {
  // HUD init  
  
  nikola::ui_layout_create(&s_hud.layout, window, resource_database_get(RESOURCE_FONT));
  s_hud.layout.buttons_padding = nikola::Vec2(40.0f, 8.0f);

  // Layout setup 

  nikola::ui_layout_begin(s_hud.layout, nikola::UI_ANCHOR_CENTER, nikola::Vec2(0.0f, 64.0f));
  
  nikola::ui_layout_push_text(s_hud.layout, "PAUSED", 50.0f, nikola::Vec4(1.0f), nikola::Vec2(0.0f, -50.0f));
  nikola::ui_layout_push_button(s_hud.layout, 
                                "Resume", 
                                30.0f, 
                                nikola::Vec4(1.0f), 
                                nikola::Vec4(0.0f, 0.0f, 0.0f, 1.0f), 
                                nikola::Vec4(0.0f, 0.0f, 0.0f, 1.0f),
                                BUTTON_RESUME, 
                                nikola::Vec2(0.0f, -40.0f));
  nikola::ui_layout_push_button(s_hud.layout, 
                                "Instructions", 
                                30.0f, 
                                nikola::Vec4(1.0f), 
                                nikola::Vec4(0.0f, 0.0f, 0.0f, 1.0f), 
                                nikola::Vec4(0.0f, 0.0f, 0.0f, 1.0f),
                                BUTTON_INSTRUCTIONS,
                                nikola::Vec2(0.0f, -40.0f));
  nikola::ui_layout_push_button(s_hud.layout, 
                                "Menu", 
                                30.0f, 
                                nikola::Vec4(1.0f), 
                                nikola::Vec4(0.0f, 0.0f, 0.0f, 1.0f), 
                                nikola::Vec4(0.0f, 0.0f, 0.0f, 1.0f),
                                BUTTON_MENU,
                                nikola::Vec2(0.0f, -40.0f));
  
  nikola::ui_layout_end(s_hud.layout);

  // Listen to events
  nikola::event_listen(nikola::EVENT_UI_BUTTON_CLICKED, pause_buttons_callback);
}

void pause_hud_render() {
  nikola::ui_layout_render(s_hud.layout);
}

/// Pause hud functions
/// ----------------------------------------------------------------------
