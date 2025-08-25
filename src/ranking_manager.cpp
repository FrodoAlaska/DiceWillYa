#include "ranking_manager.h"
#include "entities.h"
#include "game_event.h"

#include <nikola/nikola_pch.h>

/// ----------------------------------------------------------------------
/// RankingManager
struct RankingManager {
  nikola::u32 current_rank     = 1; 
  nikola::i32 next_rank_points = 3000;

  nikola::u32 current_points = 0; 
};

static RankingManager s_ranking;
/// RankingManager
/// ----------------------------------------------------------------------

/// ----------------------------------------------------------------------
/// Callbacks 

static void hand_banked_callback(const GameEvent& event, void* dispatcher, void* listener) {
  s_ranking.current_points += event.points_gained; 
  
  // Ranked!
   
  if(s_ranking.current_points >= s_ranking.next_rank_points) {
    s_ranking.current_rank++; 
    s_ranking.next_rank_points += s_ranking.current_rank * 1500; 

    GameEvent ranked_event = {
      .type     = GAME_EVENT_RANKED, 
      .new_rank = s_ranking.current_rank,
    };
    game_event_dispatch(ranked_event);
  }
}

static void farkled_callback(const GameEvent& event, void* dispatcher, void* listener) {
  nikola::u32 last_rank_points = s_ranking.current_points - (s_ranking.current_rank * 1500);
 
  // Check for a demotion

  s_ranking.current_points -= event.points_lost; 
  if(s_ranking.current_points < last_rank_points) {
    s_ranking.current_rank--;
    s_ranking.next_rank_points = last_rank_points;
  }

  if(s_ranking.next_rank_points < 3000) {
    s_ranking.next_rank_points = 3000;
  }

  if(s_ranking.current_rank <= 0) {
    s_ranking.current_rank = 1;
  }
}

/// Callbacks 
/// ----------------------------------------------------------------------

/// ----------------------------------------------------------------------
/// Ranking manager functions

void ranking_manager_init() {
  // Listen to events
  
  game_event_listen(GAME_EVENT_HAND_BANKED, hand_banked_callback);
  game_event_listen(GAME_EVENT_FARKLED, farkled_callback);
}

const nikola::u32 ranking_manager_get_current_rank() {
  return s_ranking.current_rank;
}

const nikola::i32 ranking_manager_get_next_rank_points() {
  return s_ranking.next_rank_points;
}

/// Ranking manager functions
/// ----------------------------------------------------------------------
