#pragma once

#include <nikola/nikola_pch.h>

/// ----------------------------------------------------------------------
/// GameEventType
enum GameEventType {
  GAME_EVENT_SOUND_PLAYED = 0,
  GAME_EVENT_MUSIC_PLAYED,
  GAME_EVENT_STATE_CHANGED,

  GAME_EVENTS_MAX = GAME_EVENT_STATE_CHANGED + 1,
};
/// GameEventType
/// ----------------------------------------------------------------------

/// ----------------------------------------------------------------------
/// GameEvent
struct GameEvent {
  GameEventType type; 

  nikola::i32 sound_type;
  nikola::i32 state_type;
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
