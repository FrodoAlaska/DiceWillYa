#pragma once

#include <nikola/nikola_pch.h>

/// ----------------------------------------------------------------------
/// ResourceType
enum ResourceType {
  RESOURCE_CUBE = 0,
  RESOURCE_SKYBOX,

  RESOURCE_MATERIAL_PLANE,
  RESOURCE_MATERIAL_TRANSPARENT,

  RESOURCE_SOUND_DICE_CHOOSE,
  RESOURCE_SOUND_DICE_INVALID,
  RESOURCE_SOUND_DICE_ROLL,
  RESOURCE_SOUND_DICE_COMPLETE,

  RESOURCE_SOUND_BANKING, 
  RESOURCE_SOUND_COMBO_BANK, 
  RESOURCE_SOUND_FARKLED, 
  RESOURCE_SOUND_RANKED, 
  RESOURCE_SOUND_UI_CLICK,

  RESOURCE_MUSIC1,
  RESOURCE_MUSIC2,
  RESOURCE_MUSIC3,
  RESOURCE_MUSIC4,

  RESOURCE_FONT,
  
  RESOURCE_DICE,
  RESOURCE_TABLE,

  RESOURCES_MAX,
};
/// ResourceType
/// ----------------------------------------------------------------------

/// ----------------------------------------------------------------------
/// Resource database functions 

void resource_database_init(const nikola::ResourceGroupID& group_id);

const nikola::ResourceID& resource_database_get(const ResourceType type);

/// Resource database functions 
/// ----------------------------------------------------------------------
