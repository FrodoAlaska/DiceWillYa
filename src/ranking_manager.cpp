#include "ranking_manager.h"
#include "entities.h"
#include "game_event.h"

#include <nikola/nikola_pch.h>

/// ----------------------------------------------------------------------
/// RankingManager
struct RankingManager {
  nikola::u32 current_rank     = 1; 
  nikola::i32 next_rank_points = 3000;
};

static RankingManager s_ranking;
/// RankingManager
/// ----------------------------------------------------------------------

/// ----------------------------------------------------------------------
/// Ranking manager functions

void ranking_manager_update(const Turn* turn) {
  if(turn->points > s_ranking.next_rank_points) { // We can level up!
    s_ranking.current_rank++;
    s_ranking.next_rank_points = s_ranking.current_rank * 3000;

    GameEvent event = {
      .type     = GAME_EVENT_RANKED, 
      .new_rank = s_ranking.current_rank,
    };
    game_event_dispatch(event);
  }
   
  if(s_ranking.next_rank_points < 3000) { // Better clamp the ranking points to a certain range
    s_ranking.next_rank_points = 3000;
  }
   
  if(s_ranking.current_rank <= 0) { // Clamp the rank
    s_ranking.current_rank = 1;
  } 
}

const nikola::u32 ranking_manager_get_current_rank() {
  return s_ranking.current_rank;
}

const nikola::i32 ranking_manager_get_next_rank_points() {
  return s_ranking.next_rank_points;
}

/// Ranking manager functions
/// ----------------------------------------------------------------------
