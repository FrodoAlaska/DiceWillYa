#pragma once

#include <nikola/nikola_pch.h>

/// ----------------------------------------------------------------------
/// ResourceType
enum ResourceType {
  RESOURCE_CUBE = 0,
  RESOURCE_SKYBOX,

  RESOURCE_MATERIAL_PLANE,
  RESOURCE_MATERIAL_TRANSPARENT,

  RESOURCE_DICE,
  RESOURCE_FONT,

  RESOURCES_MAX = RESOURCE_FONT + 1,
};
/// ResourceType
/// ----------------------------------------------------------------------

/// ----------------------------------------------------------------------
/// Resource database functions 

void resource_database_init(const nikola::ResourceGroupID& group_id);

const nikola::ResourceID& resource_database_get(const ResourceType type);

/// Resource database functions 
/// ----------------------------------------------------------------------
