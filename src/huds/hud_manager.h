#pragma once 

#include "entities.h"

#include <nikola/nikola_pch.h> 

/// ----------------------------------------------------------------------
/// HUDType
enum HUDType {
  HUD_MAIN_MENU = 0, 
  HUD_SETTINGS, 
  HUD_CONTROLS,
  HUD_PAUSE,
  HUD_GAME, 
};
/// HUDType
/// ----------------------------------------------------------------------

/// ----------------------------------------------------------------------
/// HUD manager functions

void hud_manager_init(Turn* turn, nikola::Window* window);

void hud_manager_update();

void hud_manager_render();

const HUDType hud_manager_get_current_hud();

/// HUD manager functions
/// ----------------------------------------------------------------------

/// ----------------------------------------------------------------------
/// Game hud functions

void game_hud_init(Turn* turn, nikola::Window* window);

void game_hud_update(); 

void game_hud_render();

/// Game hud functions
/// ----------------------------------------------------------------------

/// ----------------------------------------------------------------------
/// Menu hud functions

void menu_hud_init(nikola::Window* window);

void menu_hud_render();

/// Menu hud functions
/// ----------------------------------------------------------------------

/// ----------------------------------------------------------------------
/// Settings hud functions

void settings_hud_init(nikola::Window* window);

void settings_hud_render();

/// Settings hud functions
/// ----------------------------------------------------------------------

/// ----------------------------------------------------------------------
/// Pause hud functions

void pause_hud_init(nikola::Window* window);

void pause_hud_render();

/// Pause hud functions
/// ----------------------------------------------------------------------
