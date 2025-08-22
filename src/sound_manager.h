#pragma once

#include <nikola/nikola_pch.h>

/// ----------------------------------------------------------------------
/// SoundType
enum SoundType {
  SOUND_DICE_CHOOSE = 0, 
  SOUND_DICE_INVALID, 
  SOUND_DICE_ROLL, 
  SOUND_DICE_COMPLETE, 
  
  SOUND_BANKING, 
  SOUND_COMBO_BANK,
  SOUND_FARKLED, 
  
  // SOUND_UI_CLICK, 
  //
  // SOUND_BACKGROUND_MUSIC,

  SOUNDS_MAX,
};
/// SoundType
/// ----------------------------------------------------------------------

/// ----------------------------------------------------------------------
/// Sound manager functions 

void sound_manager_init();

void sound_manager_shutdown();

void sound_manager_play(const SoundType type, const nikola::f32 pitch = 1.0f);

void sound_manager_stop(const SoundType type);

void sound_manager_set_volume(const nikola::f32 master, const nikola::f32 music, const nikola::f32 sfx);

/// Sound manager functions 
/// ----------------------------------------------------------------------
