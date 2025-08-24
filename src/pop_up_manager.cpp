#include "pop_up_manager.h"
#include "game_event.h"
#include "resource_database.h"

#include <nikola/nikola_pch.h>

/// ----------------------------------------------------------------------
/// Consts

const nikola::sizei POP_UPS_MAX = 12;

/// Consts
/// ----------------------------------------------------------------------

/// ----------------------------------------------------------------------
/// PopupManager
struct PopupManager {
  nikola::UIText texts[POP_UPS_MAX]; 
};

static PopupManager s_pops;
/// PopupManager
/// ----------------------------------------------------------------------

/// ----------------------------------------------------------------------
/// Private functions 

static void pops_reset() {
  for(nikola::sizei i = 0; i < POP_UPS_MAX; i++) {
    s_pops.texts[i].is_active = false;
  }
}

/// Private functions 
/// ----------------------------------------------------------------------

/// ----------------------------------------------------------------------
/// Callbacks

static void pop_texts_callbacks(const GameEvent& event, void* dispatcher, void* listener) {
  switch(event.type) {
    case GAME_EVENT_FARKLED:
      pops_reset();

      popup_manager_spawn("FARKLED!", nikola::Vec3(1.0f, 0.0f, 0.0f), nikola::Vec2(0.0f, -200.0f));
      popup_manager_spawn("-" + std::to_string(event.points_lost), nikola::Vec3(1.0f, 0.0f, 0.0f));
      break;
    case GAME_EVENT_HAND_BANKED:
      popup_manager_spawn("+" + std::to_string(event.points_gained), nikola::Vec3(0.0f, 1.0f, 0.0f));
      break;
    case GAME_EVENT_HAND_CONTINUE:
      popup_manager_spawn("+" + std::to_string(event.points_gained), nikola::Vec3(1.0f, 1.0f, 0.0f));
      break;
    case GAME_EVENT_RANKED:
      popup_manager_spawn("RANKED!", nikola::Vec3(0.0f, 1.0f, 0.0f), nikola::Vec2(0.0f, -200.0f));
      break;
  }
}

/// Callbacks
/// ----------------------------------------------------------------------

/// ----------------------------------------------------------------------
/// Pop-up manager functions 

void popup_manager_init(nikola::Window* window) {
  // Texts init

  nikola::UITextDesc tex_desc = {
    .string = "+", 

    .font_id   = resource_database_get(RESOURCE_FONT),
    .font_size = 42.0f,

    .anchor = nikola::UI_ANCHOR_CENTER, 
    .offset = nikola::Vec2(0.0f, -128.0f),
  };

  for(nikola::sizei i = 0; i < POP_UPS_MAX; i++) {
    nikola::ui_text_create(&s_pops.texts[i], window, tex_desc);
  }

  // Listen to events

  game_event_listen(GAME_EVENT_FARKLED, pop_texts_callbacks);
  game_event_listen(GAME_EVENT_HAND_BANKED, pop_texts_callbacks);
  game_event_listen(GAME_EVENT_HAND_CONTINUE, pop_texts_callbacks);
  game_event_listen(GAME_EVENT_RANKED, pop_texts_callbacks);
}

void popup_manager_spawn(const nikola::String& text, const nikola::Vec3 color, const nikola::Vec2 offset) {
  // Find a non-busy pop-ups

  nikola::UIText* ui_text = nullptr;
  for(nikola::sizei i = 0; i < POP_UPS_MAX; i++) {
    if(s_pops.texts[i].is_active) { // Only pull the inactive texts 
      continue;
    }

    ui_text = &s_pops.texts[i];
  }

  if(!ui_text) { // We don't have any free pop-up yet...
    return;
  }

  // Release the pop-up into the aether

  ui_text->is_active = true;
  ui_text->color     = nikola::Vec4(color, 1.0f);
  ui_text->offset    = offset; 

  nikola::ui_text_set_string(*ui_text, text);
}

void popup_manager_update() {
  for(nikola::sizei i = 0; i < POP_UPS_MAX; i++) {
    s_pops.texts[i].is_active = (s_pops.texts[i].color.a > 0.1f);
  }
}

void popup_manager_render() {
  for(nikola::sizei i = 0; i < POP_UPS_MAX; i++) {
    nikola::UIText* text = &s_pops.texts[i];

    nikola::ui_text_apply_animation(*text, nikola::UI_TEXT_ANIMATION_FADE_OUT, 1.5f);
    nikola::ui_text_apply_animation(*text, nikola::UI_TEXT_ANIMATION_SLIDE_UP, 32.0f);

    nikola::ui_text_render(*text);
  }
}

/// Pop-up manager functions 
/// ----------------------------------------------------------------------
