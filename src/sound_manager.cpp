#include "sound_manager.h"
#include "resource_database.h"
#include "game_event.h"

#include <nikola/nikola_pch.h>

/// ----------------------------------------------------------------------
/// SoundManager
struct SoundManager {
  nikola::AudioSourceID entries[SOUNDS_MAX];

  nikola::f32 master_volume = 1.0f; 
  nikola::f32 music_volume  = 0.3f;
  nikola::f32 sounds_volume = 1.0f; 
};

static SoundManager s_manager;
/// SoundManager
/// ----------------------------------------------------------------------

/// ----------------------------------------------------------------------
/// Callbacks

static void sound_events_callbacks(const GameEvent& event, void* dispatcher, void* listener) {
  SoundType sound_type = (SoundType)-1;

  switch(event.type) {
    case GAME_EVENT_FARKLED:
      sound_type = SOUND_FARKLED;
      break;
    case GAME_EVENT_HAND_BANKED:
      sound_type = SOUND_BANKING;
      break;
    case GAME_EVENT_HAND_CONTINUE:
      sound_type = SOUND_COMBO_BANK;
      break;
    case GAME_EVENT_HAND_COMPLETE:
      sound_type = SOUND_DICE_COMPLETE;
      break;
    case GAME_EVENT_HAND_INVALID:
      sound_type = SOUND_DICE_INVALID;
      break;
    case GAME_EVENT_REROLL:
      sound_type = SOUND_DICE_ROLL;
      break;
    case GAME_EVENT_RANKED:
      // @TODO
      break;
    case GAME_EVENT_DICE_SELECTED:
      sound_type = SOUND_DICE_CHOOSE;
      break;
  }

  sound_manager_play(sound_type);
}

static bool on_button_click(const nikola::Event& event, const void* dispatcher, const void* listener) {
  sound_manager_play(SOUND_UI_CLICK, nikola::random_f32(0.9f, 1.0f));
  return true;
}


/// Callbacks
/// ----------------------------------------------------------------------

/// ----------------------------------------------------------------------
/// Private functions

// @TODO: We can probably make an API out of this, but it doesn't seem worth it right now. 
// Sorry...

static void read_config_file(const nikola::FilePath& path) {
  nikola::File cfg_file; 
  if(!nikola::file_open(&cfg_file, path, (nikola::i32)(nikola::FILE_OPEN_BINARY | nikola::FILE_OPEN_READ))) {
    NIKOLA_LOG_ERROR("Could not read from CFG file at \'%s\'", path.c_str());
    return;
  }

  nikola::file_read_bytes(cfg_file, &s_manager.master_volume, sizeof(nikola::f32));
  NIKOLA_LOG_TRACE("READ the value %f from config file", s_manager.master_volume);
}

/// Private functions
/// ----------------------------------------------------------------------

/// ----------------------------------------------------------------------
/// Sound manager functions 

void sound_manager_init() {
  // Config file init
  read_config_file("config.nkcfg"); 

  // Sound effects init

  for(nikola::sizei i = SOUND_DICE_CHOOSE; i <= SOUND_UI_CLICK; i++) {
    ResourceType res_type = (ResourceType)((nikola::sizei)(RESOURCE_SOUND_DICE_CHOOSE + i));

    nikola::AudioSourceDesc audio_desc; 
    audio_desc.volume        = s_manager.sounds_volume; 
    audio_desc.buffers_count = 1; 
    audio_desc.buffers[0]    = nikola::resources_get_audio_buffer(resource_database_get(res_type));

    s_manager.entries[i] = nikola::audio_source_create(audio_desc); 
  }

  // Music init

  for(nikola::sizei i = SOUND_MUSIC1; i < SOUNDS_MAX; i++) {
    ResourceType res_type = (ResourceType)((nikola::sizei)(RESOURCE_SOUND_DICE_CHOOSE + i));

    nikola::AudioSourceDesc audio_desc; 
    audio_desc.volume        = s_manager.music_volume; 
    audio_desc.buffers_count = 1; 
    audio_desc.is_looping    = true;
    audio_desc.buffers[0]    = nikola::resources_get_audio_buffer(resource_database_get(res_type));

    s_manager.entries[i] = nikola::audio_source_create(audio_desc); 
  }

  // Audio listener init

  nikola::AudioListenerDesc listen_desc = {
    .volume = s_manager.master_volume,
  };
  nikola::audio_listener_init(listen_desc); 

  // Listen to events
  
  game_event_listen(GAME_EVENT_FARKLED, sound_events_callbacks);
  game_event_listen(GAME_EVENT_HAND_BANKED, sound_events_callbacks);
  game_event_listen(GAME_EVENT_HAND_CONTINUE, sound_events_callbacks);
  game_event_listen(GAME_EVENT_HAND_COMPLETE, sound_events_callbacks);
  game_event_listen(GAME_EVENT_HAND_INVALID, sound_events_callbacks);
  game_event_listen(GAME_EVENT_REROLL, sound_events_callbacks);
  game_event_listen(GAME_EVENT_RANKED, sound_events_callbacks);
  game_event_listen(GAME_EVENT_DICE_SELECTED, sound_events_callbacks);

  nikola::event_listen(nikola::EVENT_UI_BUTTON_CLICKED, on_button_click);
}

void sound_manager_shutdown() {
  // Sources destroy
  for(nikola::sizei i = 0; i < SOUNDS_MAX; i++) {
    nikola::audio_source_destroy(s_manager.entries[i]);
  }
}

void sound_manager_play(const SoundType type, const nikola::f32 pitch) {
  if(type < SOUND_DICE_CHOOSE || type >= SOUNDS_MAX) {
    NIKOLA_LOG_ERROR("Invalid sound type given to sound_manager_play! %i", (nikola::i32)type);
    return;
  }

  nikola::AudioSourceID source_id = s_manager.entries[(nikola::sizei)type];
  nikola::audio_source_set_pitch(source_id, pitch);
  nikola::audio_source_start(source_id);
}

void sound_manager_stop(const SoundType type) {
  nikola::AudioSourceID source_id = s_manager.entries[(nikola::sizei)type];

  if(nikola::audio_source_is_playing(source_id)) {
    nikola::audio_source_stop(source_id);
  }
}

void sound_manager_set_volume(const nikola::f32 master) {
  s_manager.master_volume = master; 
  nikola::audio_listener_set_volume(master);
}

const nikola::f32 sound_manager_get_volume() {
  return s_manager.master_volume;
}

/// Sound manager functions 
/// ----------------------------------------------------------------------
