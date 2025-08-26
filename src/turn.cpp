#include "entities.h"
#include "sound_manager.h"
#include "game_event.h"

#include <nikola/nikola_pch.h>

/// ----------------------------------------------------------------------
/// Turn functions

void turn_create(Turn* out_turn) {
  // Dices init

  for(nikola::sizei i = 0; i < DICES_MAX; i++) {
    nikola::Vec3 pos;
    pos.x = 9.9f;
    pos.y = 7.0f; 
    pos.z = 1.07f - (0.4f * i);

    dice_create(&out_turn->dices[i], pos);
  }

  // Cursor init

  nikola::transform_translate(out_turn->cursor, out_turn->dices[0].transform.position);
  nikola::transform_scale(out_turn->cursor, nikola::Vec3(0.18f));

  // Play some nice music
 
  nikola::u32 random_music = nikola::random_u32(SOUND_MUSIC1, SOUND_MUSIC4);
  sound_manager_play((SoundType)random_music);
}

void turn_reset(Turn& turn) {
  for(nikola::sizei i = 0; i < DICES_MAX; i++) {
    dice_reset(turn.dices[i]);
  }
  
  turn.points          = 0;
  turn.unbanked_points = 0;  
  turn.eval_points     = 0; 

  turn.continues = 1;
  turn.clearings = 1;

  turn.is_farkle   = false; 
  turn.dices_count = DICES_MAX;

  turn.rolls_count = ROLLS_MAX;

  turn_start(turn);
}

void turn_process_input(Turn& turn) {
  // Traverse between the shown dice

  if(nikola::input_key_pressed(nikola::KEY_LEFT)) {
    turn.dice_cursor--;
  }
  else if(nikola::input_key_pressed(nikola::KEY_RIGHT)) {
    turn.dice_cursor++;
  }
 
  // Make the cursor wrap around the ends

  if(turn.dice_cursor < 0) {
    turn.dice_cursor = DICES_MAX - 1;
  }
  else if(turn.dice_cursor >= DICES_MAX) {
    turn.dice_cursor = 0;
  }

  // Set the cursor's position to the currently selected dice (if it's active).

  Dice* dice = &turn.dices[turn.dice_cursor];
  
  nikola::Vec3 dice_pos = turn.cursor.position;
  nikola::transform_translate(turn.cursor, nikola::Vec3(dice_pos.x, dice_pos.y, dice->transform.position.z));
   
  // Select the dice
  
  if(nikola::input_key_pressed(nikola::KEY_SPACE)) {
    dice_toggle_select(*dice, !dice->is_selected);
    
    GameEvent event = {
      .type = GAME_EVENT_DICE_SELECTED,
    };
    game_event_dispatch(event);
  } 
  
  if(turn.rolls_count < 0) { // Clamp the rolls count
    turn.rolls_count = 0;
  }
}

void turn_update(Turn& turn, const nikola::f32 dt) {
  // Dices update

  for(nikola::sizei i = 0; i < DICES_MAX; i++) {
    dice_update(turn.dices[i], dt);
  }

  turn.eval_points = rulebook_evaluate_selected(turn.dices); 

  // Restart the game if all the dice have been discarded

  if(turn.dices_count <= 0) {
    turn.dices_count = DICES_MAX; 

    for(nikola::sizei i = 0; i < DICES_MAX; i++) {
      dice_reset(turn.dices[i]);
      dice_roll(turn.dices[i]); 
    }

    turn.clearings++;
 
    GameEvent event = {
      .type       = GAME_EVENT_HAND_COMPLETE,
      .multiplier = turn.clearings,
    };
    game_event_dispatch(event);
  }
}

void turn_start(Turn& turn) {
  // Roll the dice 
  
  for(nikola::sizei i = 0; i < DICES_MAX; i++) {
    dice_roll(turn.dices[i]);
  }
  
  GameEvent roll_event = {
    .type = GAME_EVENT_REROLL,
  };
  game_event_dispatch(roll_event);

  // Check for a farkle

  nikola::i32 eval_res = rulebook_evaluate_active(turn.dices);
  turn.is_farkle       = (eval_res <= 0);

  if(!turn.is_farkle) {
    return;
  }

  // Farkled! Deduce the points

  nikola::i32 old_points  = turn.points;
  nikola::i32 points_lost = (turn.points / 4); 
 
  // Send an event to inform the rest of the game

  // @NOTE: Sometimes the `points_lost` is 0 because there 
  // are no points that have been banked yet. That is not 
  // very informative. So, we use the unbanked points 
  // instead to show how much was lost

  GameEvent event = {
    .type        = GAME_EVENT_FARKLED,
    .points_lost = (nikola::u32)(points_lost + turn.unbanked_points),
  };
  game_event_dispatch(event);

  // Setup the turn state

  turn_reset(turn);

  turn.points  = old_points;
  turn.points -= points_lost;

  // Re-roll 
  // @TEMP: Potentially harmful!
  turn_start(turn);
}

void turn_continue(Turn& turn) {
  // We have to check for a valid evaluation first

  if(turn.eval_points <= 0) {
    // De-select the currently selected dice
    
    for(nikola::sizei i = 0; i < DICES_MAX; i++) {
      dice_toggle_select(turn.dices[i], false);
    }

    GameEvent event = {
      .type = GAME_EVENT_HAND_INVALID,
    };
    game_event_dispatch(event);
    
    return;
  } 

  // The evaluation was fine. 
  // Add the evaluation result to the accumulated points, 
  // and discard the dice that were selected.

  for(nikola::sizei i = 0; i < DICES_MAX; i++) {
    // The dice was successfully discarded and therefore 
    // it will be ignored.

    if(dice_try_discard(turn.dices[i])) { 
      turn.dices_count--;
      continue;
    }

    dice_toggle_select(turn.dices[i], false);
  }

  turn.continues++;
  turn.unbanked_points += turn.eval_points;  

  GameEvent event = {
    .type          = GAME_EVENT_HAND_CONTINUE,
    .points_gained = (nikola::u32)turn.eval_points, 
    .multiplier    = turn.continues,
  };
  game_event_dispatch(event);

  // Guess if the next turn is going to be a farkle or not
  if(turn.dices_count > 0) {
    turn_start(turn);
  }
}

void turn_bank(Turn& turn) {
  // We have to check for a valid evaluation first

  if(turn.eval_points <= 0) {
    // De-select the currently selected dice
    
    for(nikola::sizei i = 0; i < DICES_MAX; i++) {
      dice_toggle_select(turn.dices[i], false);
    }

    GameEvent event = {
      .type = GAME_EVENT_HAND_INVALID,
    };
    game_event_dispatch(event);

    sound_manager_play(SOUND_DICE_INVALID);
    return;
  } 

  // Bank the points otherwise

  nikola::u32 points_gained = (turn.unbanked_points + turn.eval_points) * turn.continues * turn.clearings;

  turn.points         += points_gained;
  turn.unbanked_points = 0;  
  turn.eval_points     = 0; 

  turn.continues = 1;
  turn.clearings = 1;

  turn.dices_count = DICES_MAX;
  turn.rolls_count = ROLLS_MAX;

  // Send an event
  
  GameEvent event = {
    .type          = GAME_EVENT_HAND_BANKED,
    .points_gained = points_gained,
  };
  game_event_dispatch(event);

  // Reset the dice
  
  for(nikola::sizei i = 0; i < DICES_MAX; i++) {
    dice_reset(turn.dices[i]);
  } 

  // Re-roll
  turn_start(turn);
}

/// Turn functions
/// ----------------------------------------------------------------------
