#pragma once

#include <nikola/nikola_pch.h>

/// ----------------------------------------------------------------------
/// GameEventType
enum GameEventType {
  GAME_EVENT_FARKLED, 
  GAME_EVENT_HAND_BANKED,
  GAME_EVENT_HAND_CONTINUE,
  GAME_EVENT_HAND_COMPLETE,
  GAME_EVENT_HAND_INVALID,
  GAME_EVENT_REROLL, 
  GAME_EVENT_RANKED,
  GAME_EVENT_DICE_SELECTED,

  GAME_EVENT_HUD_CHANGED,

  GAME_EVENTS_MAX,
};
/// GameEventType
/// ----------------------------------------------------------------------

/// ----------------------------------------------------------------------
/// GameEvent
struct GameEvent {
  GameEventType type; 

  nikola::u32 points_gained = 0; 
  nikola::u32 points_lost   = 0;
  nikola::u32 new_rank      = 0;

  nikola::u32 hud_type;
};
/// GameEvent
/// ----------------------------------------------------------------------

/// ----------------------------------------------------------------------
/// Callbacks

using OnGameEventFireFunc = void(*)(const GameEvent& event, void* dispatcher, void* listener);

/// Callbacks
/// ----------------------------------------------------------------------

/// ----------------------------------------------------------------------
/// GameEvent functions

void game_event_listen(const GameEventType type, const OnGameEventFireFunc& func, const void* listener = nullptr);

void game_event_dispatch(const GameEvent& event, const void* dispatcher = nullptr);

/// GameEvent functions
/// ----------------------------------------------------------------------
