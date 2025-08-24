#pragma once 

#include "entities.h"

#include <nikola/nikola_pch.h>

/// ----------------------------------------------------------------------
/// Ranking manager functions

void ranking_manager_update(const Turn* turn);

const nikola::u32 ranking_manager_get_current_rank();

const nikola::i32 ranking_manager_get_next_rank_points();

/// Ranking manager functions
/// ----------------------------------------------------------------------
