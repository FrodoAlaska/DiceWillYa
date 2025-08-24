#pragma once 

#include "app.h"

#include <nikola/nikola_pch.h> 

/// ----------------------------------------------------------------------
/// HUDType
enum HUDType {
  HUD_MAIN_MENU = 0, 
  HUD_INSTRUCTIONS,
  HUD_SETTINGS, 
  HUD_PAUSE,
  HUD_GAME, 
};
/// HUDType
/// ----------------------------------------------------------------------

/// ----------------------------------------------------------------------
/// HUD manager functions

void hud_manager_init(nikola::App* app, nikola::Window* window);

void hud_manager_update();

void hud_manager_render();

/// HUD manager functions
/// ----------------------------------------------------------------------

/// ----------------------------------------------------------------------
/// Game hud functions

void game_hud_init(nikola::App* app, nikola::Window* window);

void game_hud_update(); 

void game_hud_render();

/// Game hud functions
/// ----------------------------------------------------------------------
