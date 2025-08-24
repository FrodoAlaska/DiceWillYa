#include "hud_manager.h"
#include "game_event.h"
#include "app.h"
#include "entities.h"
#include "resource_database.h"
#include "ranking_manager.h"

#include <nikola/nikola_pch.h> 

/// ----------------------------------------------------------------------
/// GameHUD
struct GameHUD {
  nikola::App* app;

  nikola::UILayout layout;
};

static GameHUD s_hud;
/// GameHUD
/// ----------------------------------------------------------------------

/// ----------------------------------------------------------------------
/// Game hud functions

void game_hud_init(nikola::App* app, nikola::Window* window) {
  s_hud.app = app; // Ew, ew, ew, ew

  nikola::ui_layout_create(&s_hud.layout, window, resource_database_get(RESOURCE_FONT));

  // Scores
  
  nikola::ui_layout_begin(s_hud.layout, nikola::UI_ANCHOR_TOP_LEFT, nikola::Vec2(0.0f, 24.0f));

  nikola::ui_layout_push_text(s_hud.layout, "Turn points:     0", 24.0f, nikola::Vec4(1.0f));
  nikola::ui_layout_push_text(s_hud.layout, "Unbanked points: 0", 24.0f, nikola::Vec4(1.0f, 0.0f, 0.0f, 1.0f));
  nikola::ui_layout_push_text(s_hud.layout, "Banked points:   0", 24.0f, nikola::Vec4(0.0f, 1.0f, 0.0f, 1.0f));
  
  nikola::ui_layout_push_text(s_hud.layout, "Rank:        0", 24.0f, nikola::Vec4(1.0f));
  nikola::ui_layout_push_text(s_hud.layout, "Points left: 0", 24.0f, nikola::Vec4(1.0f));

  nikola::ui_layout_end(s_hud.layout);
}

void game_hud_update() {
  const Turn* turn = app_get_current_turn(s_hud.app);

  nikola::ui_text_set_string(s_hud.layout.texts[0], 
      "Turn points: " + std::to_string(turn->eval_points));

  nikola::ui_text_set_string(s_hud.layout.texts[1], 
      "Unbanked points: " + std::to_string(turn->unbanked_points));

  nikola::ui_text_set_string(s_hud.layout.texts[2], 
      "Banked points: " + std::to_string(turn->points));

  nikola::ui_text_set_string(s_hud.layout.texts[3], 
      "Rank: " + std::to_string(ranking_manager_get_current_rank()));

  nikola::ui_text_set_string(s_hud.layout.texts[4], 
      "Next rank: " + std::to_string(ranking_manager_get_next_rank_points()));
}

void game_hud_render() {
  nikola::ui_layout_render(s_hud.layout);
}

/// Game hud functions
/// ----------------------------------------------------------------------
