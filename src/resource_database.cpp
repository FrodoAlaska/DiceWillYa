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
  // Meshes init
  s_database.resources[RESOURCE_CUBE] = nikola::resources_push_mesh(group_id, nikola::GEOMETRY_CUBE);
 
  // Skybox init

  nikola::ResourceID cubemap_id         = nikola::resources_get_id(group_id, "corona");
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

  // Models init
  s_database.resources[RESOURCE_DICE] = nikola::resources_get_id(group_id, "dice");

  // Fonts init
  s_database.resources[RESOURCE_FONT] = nikola::resources_get_id(group_id, "Kleader");
}

const nikola::ResourceID& resource_database_get(const ResourceType type) {
  return s_database.resources[type];
}

/// Resource database functions 
/// ----------------------------------------------------------------------
