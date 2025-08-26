#include "hud_manager.h"
#include "game_event.h"
#include "resource_database.h"

#include <nikola/nikola_pch.h> 

/// ----------------------------------------------------------------------
/// MenuButtonID 
enum MenuButtonID {
  BUTTON_PLAY = 0, 
  BUTTON_SETTINGS, 
  BUTTON_CONTROLS, 
  BUTTON_QUIT,
};
/// MenuButtonID 
/// ----------------------------------------------------------------------

/// ----------------------------------------------------------------------
/// MenuHUD
struct MenuHUD {
  nikola::UILayout layout;
};

static MenuHUD s_hud;
/// MenuHUD
/// ----------------------------------------------------------------------

/// ----------------------------------------------------------------------
/// Callbacks

static bool menu_buttons_callback(const nikola::Event& event, const void* dispatcher, const void* listener) {
  if(event.type != nikola::EVENT_UI_BUTTON_CLICKED) {
    return false;
  }

  switch(event.button->id) {
    case BUTTON_PLAY:
      game_event_dispatch({
        .type     = GAME_EVENT_HUD_CHANGED, 
        .hud_type = (nikola::u32)HUD_GAME,
      });      
      break;
    case BUTTON_SETTINGS:
      game_event_dispatch({
        .type     = GAME_EVENT_HUD_CHANGED, 
        .hud_type = (nikola::u32)HUD_SETTINGS,
      });      
    case BUTTON_CONTROLS:
      game_event_dispatch({
        .type     = GAME_EVENT_HUD_CHANGED, 
        .hud_type = (nikola::u32)HUD_CONTROLS,
      });      
      break;
    case BUTTON_QUIT:
      nikola::event_dispatch({
        .type = nikola::EVENT_APP_QUIT,     
      });
      break;
  }

  return true;
}

/// Callbacks
/// ----------------------------------------------------------------------

/// ----------------------------------------------------------------------
/// Menu hud functions

void menu_hud_init(nikola::Window* window) {
  // HUD init  
  nikola::ui_layout_create(&s_hud.layout, window, resource_database_get(RESOURCE_FONT));

  // Layout setup 

  nikola::ui_layout_begin(s_hud.layout, nikola::UI_ANCHOR_TOP_CENTER, nikola::Vec2(0.0f, 32.0f));
  nikola::ui_layout_push_text(s_hud.layout, "Play Dice With Me, Will Ya?", 50.0f, nikola::Vec4(1.0f));
  nikola::ui_layout_end(s_hud.layout);
  
  nikola::ui_layout_begin(s_hud.layout, nikola::UI_ANCHOR_CENTER, nikola::Vec2(0.0f, 64.0f));
 
  s_hud.layout.buttons_padding = nikola::Vec2(40.0f, 8.0f);

  nikola::ui_layout_push_button(s_hud.layout, 
                                "Play", 
                                30.0f, 
                                nikola::Vec4(1.0f), 
                                nikola::Vec4(0.0f, 0.0f, 0.0f, 1.0f), 
                                nikola::Vec4(0.0f, 0.0f, 0.0f, 1.0f), 
                                nikola::Vec2(0.0f, -64.0f));
  
  nikola::ui_layout_push_button(s_hud.layout, 
                                "Settings", 
                                30.0f, 
                                nikola::Vec4(1.0f), 
                                nikola::Vec4(0.0f, 0.0f, 0.0f, 1.0f), 
                                nikola::Vec4(0.0f, 0.0f, 0.0f, 1.0f), 
                                nikola::Vec2(0.0f, -64.0f));
  
  nikola::ui_layout_push_button(s_hud.layout, 
                                "Controls", 
                                30.0f, 
                                nikola::Vec4(1.0f), 
                                nikola::Vec4(0.0f, 0.0f, 0.0f, 1.0f), 
                                nikola::Vec4(0.0f, 0.0f, 0.0f, 1.0f), 
                                nikola::Vec2(0.0f, -64.0f));
  
  nikola::ui_layout_push_button(s_hud.layout, 
                                "Quit", 
                                30.0f, 
                                nikola::Vec4(1.0f), 
                                nikola::Vec4(0.0f, 0.0f, 0.0f, 1.0f), 
                                nikola::Vec4(0.0f, 0.0f, 0.0f, 1.0f), 
                                nikola::Vec2(0.0f, -64.0f));

  nikola::ui_layout_end(s_hud.layout);

  // Listen to events
  nikola::event_listen(nikola::EVENT_UI_BUTTON_CLICKED, menu_buttons_callback);
}

void menu_hud_render() {
  nikola::ui_layout_render(s_hud.layout);
}

/// Menu hud functions
/// ----------------------------------------------------------------------
