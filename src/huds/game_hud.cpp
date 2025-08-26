#include "hud_manager.h"
#include "game_event.h"
#include "entities.h"
#include "resource_database.h"
#include "ranking_manager.h"

#include <nikola/nikola_pch.h> 

/// ----------------------------------------------------------------------
/// GameButtonID
enum GameButtonID {
  BUTTON_CONTINUE = 0x32, 
  BUTTON_ROLL,
  BUTTON_BANK,
};
/// GameButtonID
/// ----------------------------------------------------------------------

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
/// Callbacks

static bool on_button_click(const nikola::Event& event, const void* dispatcher, const void* listener) {
  switch(event.button->id) {
    case BUTTON_CONTINUE:
      turn_continue(*s_hud.turn);
      break;
    case BUTTON_ROLL:
      if(s_hud.turn->rolls_count > 0) {
        turn_start(*s_hud.turn);
        s_hud.turn->rolls_count--;
      }
      break;
    case BUTTON_BANK:
      turn_bank(*s_hud.turn);
      break;
  } 

  return true;
}

/// Callbacks
/// ----------------------------------------------------------------------

/// ----------------------------------------------------------------------
/// Game hud functions

void game_hud_init(Turn* turn, nikola::Window* window) {
  // HUD init  

  s_hud.turn = turn;
  
  nikola::ui_layout_create(&s_hud.layout, window, resource_database_get(RESOURCE_FONT));
  s_hud.layout.buttons_padding = nikola::Vec2(40.0f, 8.0f);

  // Layout setup 

  nikola::ui_layout_begin(s_hud.layout, nikola::UI_ANCHOR_TOP_LEFT, nikola::Vec2(0.0f, 32.0f));

  nikola::ui_layout_push_text(s_hud.layout, "0", 32.0f, nikola::Vec4(0.0f, 1.0f, 0.0f, 1.0f));          // Points
  nikola::ui_layout_push_text(s_hud.layout, "0", 32.0f, nikola::Vec4(1.0f, 1.0f, 0.0f, 1.0f));          // Unbanked points
  nikola::ui_layout_push_text(s_hud.layout, "0", 32.0f, nikola::Vec4(1.0f));                            // Eval points
  nikola::ui_layout_push_text(s_hud.layout, "0", 32.0f, nikola::Vec4(1.0f), nikola::Vec2(0.0f, 64.0f)); // Rank
  nikola::ui_layout_push_text(s_hud.layout, "0", 32.0f, nikola::Vec4(1.0f), nikola::Vec2(0.0f, 64.0f)); // Points till next rank
  nikola::ui_layout_push_text(s_hud.layout, "0", 32.0f, nikola::Vec4(1.0f), nikola::Vec2(0.0f, 64.0f)); // Rolls left

  nikola::ui_layout_end(s_hud.layout);
  
  nikola::ui_layout_begin(s_hud.layout, nikola::UI_ANCHOR_BOTTOM_CENTER, nikola::Vec2(128.0f, 0.0f));
  nikola::ui_layout_push_button(s_hud.layout, 
                                "Keep", 
                                30.0f, 
                                nikola::Vec4(1.0f, 1.0f, 0.0f, 1.0f), 
                                nikola::Vec4(0.0f, 0.0f, 0.0f, 1.0f), 
                                nikola::Vec4(0.0f, 0.0f, 0.0f, 1.0f),
                                BUTTON_CONTINUE, 
                                nikola::Vec2(-128.0f, -32.0f));
  nikola::ui_layout_push_button(s_hud.layout, 
                                "Roll", 
                                30.0f, 
                                nikola::Vec4(1.0f), 
                                nikola::Vec4(0.0f, 0.0f, 0.0f, 1.0f), 
                                nikola::Vec4(0.0f, 0.0f, 0.0f, 1.0f),
                                BUTTON_ROLL, 
                                nikola::Vec2(-128.0f, -32.0f));
  nikola::ui_layout_push_button(s_hud.layout, 
                                "Bank", 
                                30.0f, 
                                nikola::Vec4(0.0f, 1.0f, 0.0f, 1.0f), 
                                nikola::Vec4(0.0f, 0.0f, 0.0f, 1.0f), 
                                nikola::Vec4(0.0f, 0.0f, 0.0f, 1.0f),
                                BUTTON_BANK, 
                                nikola::Vec2(-128.0f, -32.0f));
  nikola::ui_layout_end(s_hud.layout);

  // Listen to events
  
  nikola::event_listen(nikola::EVENT_UI_BUTTON_CLICKED, on_button_click);
}

void game_hud_update() {
  nikola::ui_text_set_string(s_hud.layout.texts[0], "Banked: " + std::to_string(s_hud.turn->points));
  nikola::ui_text_set_string(s_hud.layout.texts[1], "Unbanked: " + std::to_string(s_hud.turn->unbanked_points));
  nikola::ui_text_set_string(s_hud.layout.texts[2], "Turn points: " + std::to_string(s_hud.turn->eval_points));
  nikola::ui_text_set_string(s_hud.layout.texts[3], "Rank: " + std::to_string(ranking_manager_get_current_rank()));
  nikola::ui_text_set_string(s_hud.layout.texts[4], "Next rank: " + std::to_string(ranking_manager_get_next_rank_points()));
  nikola::ui_text_set_string(s_hud.layout.texts[5], "Rolls left: " + std::to_string(s_hud.turn->rolls_count));
}

void game_hud_render() {
  nikola::ui_layout_render(s_hud.layout);
}

/// Game hud functions
/// ----------------------------------------------------------------------
