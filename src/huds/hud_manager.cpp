#include "hud_manager.h"
#include "game_event.h"

#include <nikola/nikola_pch.h> 

/// ----------------------------------------------------------------------
/// HUDManager
struct HUDManager {
  HUDType current_hud = HUD_GAME;
};

static HUDManager s_hud;
/// HUDManager
/// ----------------------------------------------------------------------

/// ----------------------------------------------------------------------
/// Callbacks

static void hud_event_callback(const GameEvent& event, void* dispatcher, void* listener) {
  s_hud.current_hud = (HUDType)event.hud_type;
}

/// Callbacks
/// ----------------------------------------------------------------------

/// ----------------------------------------------------------------------
/// HUD manager functions

void hud_manager_init(Turn* turn, nikola::Window* window) {
  // HUDs init 
  
  game_hud_init(turn, window);

  // Listen to events
  game_event_listen(GAME_EVENT_HUD_CHANGED, hud_event_callback); 
}

void hud_manager_update() {
  switch(s_hud.current_hud) {
    case HUD_MAIN_MENU:
      break;
    case HUD_INSTRUCTIONS:
      break;
    case HUD_SETTINGS:
      break;
    case HUD_PAUSE:
      break;
    case HUD_GAME:
      game_hud_update();
      break;
  }
}

void hud_manager_render() {
  switch(s_hud.current_hud) {
    case HUD_MAIN_MENU:
      break;
    case HUD_INSTRUCTIONS:
      break;
    case HUD_SETTINGS:
      break;
    case HUD_PAUSE:
      break;
    case HUD_GAME:
      game_hud_render();
      break;
  }
}

/// HUD manager functions
/// ----------------------------------------------------------------------
