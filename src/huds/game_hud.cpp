#include "hud_manager.h"
#include "game_event.h"
#include "entities.h"
#include "resource_database.h"
#include "ranking_manager.h"

#include <nikola/nikola_pch.h> 

/// ----------------------------------------------------------------------
/// GameHUD
struct GameHUD {
  Turn* turn;
  nikola::UILayout layout;
};

static GameHUD s_hud;
/// GameHUD
/// ----------------------------------------------------------------------

/// ----------------------------------------------------------------------
/// Game hud functions

void game_hud_init(Turn* turn, nikola::Window* window) {
  // HUD init  

  s_hud.turn = turn;
  nikola::ui_layout_create(&s_hud.layout, window, resource_database_get(RESOURCE_FONT));

  // Layout setup 

  nikola::ui_layout_begin(s_hud.layout, nikola::UI_ANCHOR_TOP_LEFT, nikola::Vec2(0.0f, 32.0f));

  nikola::ui_layout_push_text(s_hud.layout, "0", 32.0f, nikola::Vec4(0.0f, 1.0f, 0.0f, 1.0f));          // Points
  nikola::ui_layout_push_text(s_hud.layout, "0", 32.0f, nikola::Vec4(1.0f, 1.0f, 0.0f, 1.0f));          // Unbanked points
  nikola::ui_layout_push_text(s_hud.layout, "0", 32.0f, nikola::Vec4(1.0f), nikola::Vec2(0.0f, 32.0f)); // Rank
  nikola::ui_layout_push_text(s_hud.layout, "0", 32.0f, nikola::Vec4(1.0f), nikola::Vec2(0.0f, 32.0f)); // Points till next rank
  nikola::ui_layout_push_text(s_hud.layout, "0", 32.0f, nikola::Vec4(1.0f), nikola::Vec2(0.0f, 32.0f)); // Rolls left

  nikola::ui_layout_end(s_hud.layout);
}

void game_hud_update() {
  nikola::ui_text_set_string(s_hud.layout.texts[0], "Banked: " + std::to_string(s_hud.turn->points));
  nikola::ui_text_set_string(s_hud.layout.texts[1], "Unbanked: " + std::to_string(s_hud.turn->unbanked_points));
  nikola::ui_text_set_string(s_hud.layout.texts[2], "Rank: " + std::to_string(ranking_manager_get_current_rank()));
  nikola::ui_text_set_string(s_hud.layout.texts[3], "Next rank: " + std::to_string(ranking_manager_get_next_rank_points()));
  nikola::ui_text_set_string(s_hud.layout.texts[4], "Rolls left: " + std::to_string(s_hud.turn->rolls_count));
}

void game_hud_render() {
  nikola::ui_layout_render(s_hud.layout);
}

/// Game hud functions
/// ----------------------------------------------------------------------
