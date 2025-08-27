#include "hud_manager.h"
#include "game_event.h"
#include "resource_database.h"

#include <nikola/nikola_pch.h> 

/// ----------------------------------------------------------------------
/// Consts

const nikola::sizei INSTRUCTIONS_MAX = 14;

/// Consts
/// ----------------------------------------------------------------------

/// ----------------------------------------------------------------------
/// InstructionsButtonID
enum InstructionsButtonID {
  BUTTON_MENU = 0x88, 
  BUTTON_GAME,
};
/// InstructionsButtonID
/// ----------------------------------------------------------------------

/// ----------------------------------------------------------------------
/// InstructionsHUD
struct InstructionsHUD {
  nikola::UILayout layout;

  nikola::String instructions_list[INSTRUCTIONS_MAX];
};

static InstructionsHUD s_hud;
/// InstructionsHUD
/// ----------------------------------------------------------------------

/// ----------------------------------------------------------------------
/// Callbacks

static bool on_button_click(const nikola::Event& event, const void* dispatcher, const void* listener) {
  switch(event.button->id) {
    case BUTTON_MENU:
      game_event_dispatch({
        .type     = GAME_EVENT_HUD_CHANGED, 
        .hud_type = (nikola::u32)HUD_MAIN_MENU
      });
      break;
    case BUTTON_GAME:
      game_event_dispatch({
        .type     = GAME_EVENT_HUD_CHANGED, 
        .hud_type = (nikola::u32)HUD_GAME
      });
      break;
  } 

  return true;
}

/// Callbacks
/// ----------------------------------------------------------------------

/// ----------------------------------------------------------------------
/// Instructions hud functions

void instructions_hud_init(nikola::Window* window) {
  // HUD init  

  nikola::ui_layout_create(&s_hud.layout, window, resource_database_get(RESOURCE_FONT));
  s_hud.layout.buttons_padding = nikola::Vec2(40.0f, 8.0f);

  // Instructions init

  s_hud.instructions_list[0] = "- A die roll of 1 is always worth 100 points";
  s_hud.instructions_list[1] = "- A die roll of 5 is always worth 5 points";
  s_hud.instructions_list[2] = "- A pair of 1 dice is worth 200 points ";
  s_hud.instructions_list[3] = "- A pair of 5 dice is worth 100 points";
  s_hud.instructions_list[4] = "- A three of a kind is worth the value multiplied by 3 (i.e, 3 dice of 6 is worth 600) ";
  s_hud.instructions_list[5] = "- A four of a kind worth 1000 points";
  s_hud.instructions_list[6] = "- A five of a kind worth 2000 points";
  s_hud.instructions_list[7] = "- A six of a kind worth 3000 points";
  s_hud.instructions_list[8] = "- Three pairs of the same kind is worth 1500";
  s_hud.instructions_list[9] = "- A running sequence of numbers (i.e, 1, 2, 3, 4, 5, 6) is worth 2500";

  s_hud.instructions_list[10] = "- Getting a \'farkle\' occurs when none of the rules above are present in the current rolled hand.";
  s_hud.instructions_list[11] = "- You can either \'keep\' the score and roll again, or you can \'bank\' your points";
  s_hud.instructions_list[12] = "- Keeping your score will not save it to your total, but it will grant you a multiplier. However, you are at a risk of losing all these points in you get \'farkled\'";
  s_hud.instructions_list[13] = "- Banking, on the other hand, will save your points to your total.";

  // Layout setup 

  nikola::ui_layout_begin(s_hud.layout, nikola::UI_ANCHOR_TOP_CENTER);
  nikola::ui_layout_push_text(s_hud.layout, "Farkle Guide", 50.0f, nikola::Vec4(1.0f));
  nikola::ui_layout_end(s_hud.layout);
  
  nikola::ui_layout_begin(s_hud.layout, nikola::UI_ANCHOR_CENTER_LEFT, nikola::Vec2(0.0f, 32.0f));
 
  for(nikola::sizei i = 0; i < INSTRUCTIONS_MAX; i++) {
    nikola::ui_layout_push_text(s_hud.layout, s_hud.instructions_list[i], 24.0f, nikola::Vec4(1.0f), nikola::Vec2(0.0f, -256.0f));
  }
  
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
  
  nikola::event_listen(nikola::EVENT_UI_BUTTON_CLICKED, on_button_click);
}

void instructions_hud_render() {
  nikola::ui_layout_render(s_hud.layout);
}

/// Instructions hud functions
/// ----------------------------------------------------------------------
