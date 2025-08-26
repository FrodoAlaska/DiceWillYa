#include "resource_database.h"

#include <nikola/nikola_pch.h>

/// ----------------------------------------------------------------------
/// ResourceDatabase
struct ResourceDatabase {
  nikola::ResourceGroupID resource_group;
  nikola::ResourceID resources[RESOURCES_MAX];
};

static ResourceDatabase s_database;
/// ResourceDatabase
/// ----------------------------------------------------------------------

/// ----------------------------------------------------------------------
/// Resource database functions 

void resource_database_init(const nikola::ResourceGroupID& group_id) {
  // Push all the resources in...
  nikola::resources_push_dir(group_id, "res");

  // Meshes init
  s_database.resources[RESOURCE_CUBE] = nikola::resources_push_mesh(group_id, nikola::GEOMETRY_CUBE);
 
  // Skybox init

  nikola::ResourceID cubemap_id         = nikola::resources_get_id(group_id, "accurate_night");
  s_database.resources[RESOURCE_SKYBOX] = nikola::resources_push_skybox(group_id, cubemap_id);

  // Materials init

  nikola::MaterialDesc mat_desc = {
    .diffuse_id = nikola::resources_get_id(group_id, "brickwall"),
    .normal_id  = nikola::resources_get_id(group_id, "brickwall_normal"),
  };
  s_database.resources[RESOURCE_MATERIAL_PLANE] = nikola::resources_push_material(group_id, mat_desc);

  mat_desc = {
    .color        = nikola::Vec3(1.0f, 1.0f, 0.0f),
    .transparency = 0.2f,
    .depth_mask   = false,
  };
  s_database.resources[RESOURCE_MATERIAL_TRANSPARENT] = nikola::resources_push_material(group_id, mat_desc);

  // Audio init

  s_database.resources[RESOURCE_SOUND_DICE_CHOOSE]   = nikola::resources_get_id(group_id, "dice_choose");
  s_database.resources[RESOURCE_SOUND_DICE_INVALID]  = nikola::resources_get_id(group_id, "dice_invalid");
  s_database.resources[RESOURCE_SOUND_DICE_ROLL]     = nikola::resources_get_id(group_id, "dice_throw");
  s_database.resources[RESOURCE_SOUND_DICE_COMPLETE] = nikola::resources_get_id(group_id, "dice_complete");
  
  s_database.resources[RESOURCE_SOUND_BANKING]    = nikola::resources_get_id(group_id, "banking_multiplier");
  s_database.resources[RESOURCE_SOUND_COMBO_BANK] = nikola::resources_get_id(group_id, "banking");
  s_database.resources[RESOURCE_SOUND_FARKLED]    = nikola::resources_get_id(group_id, "farkle");
  s_database.resources[RESOURCE_SOUND_RANKED]     = nikola::resources_get_id(group_id, "ranked");
  s_database.resources[RESOURCE_SOUND_UI_CLICK]   = nikola::resources_get_id(group_id, "ui_click");

  s_database.resources[RESOURCE_MUSIC1] = nikola::resources_get_id(group_id, "music_1_old_tower_inn");
  s_database.resources[RESOURCE_MUSIC2] = nikola::resources_get_id(group_id, "music_2_the_bards_tale");
  s_database.resources[RESOURCE_MUSIC3] = nikola::resources_get_id(group_id, "music_3_minstrel_dance");
  s_database.resources[RESOURCE_MUSIC4] = nikola::resources_get_id(group_id, "music_4_market_day");

  // Fonts init
  s_database.resources[RESOURCE_FONT] = nikola::resources_get_id(group_id, "Kleader");

  // Models init
  
  s_database.resources[RESOURCE_DICE]  = nikola::resources_get_id(group_id, "dice");
  s_database.resources[RESOURCE_TABLE] = nikola::resources_get_id(group_id, "tavern_table");
}

const nikola::ResourceID& resource_database_get(const ResourceType type) {
  return s_database.resources[type];
}

/// Resource database functions 
/// ----------------------------------------------------------------------
