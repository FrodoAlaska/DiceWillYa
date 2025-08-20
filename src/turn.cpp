#include "entities.h"

#include <nikola/nikola_pch.h>

/// ----------------------------------------------------------------------
/// Turn functions

void turn_create(Turn* out_turn) {
  // Dices init

  for(nikola::sizei i = 0; i < DICES_MAX; i++) {
    nikola::Vec3 pos;
    pos.x = 5.0f + (i * 2.0f);
    pos.z = -2.5f;
    pos.y = -3.0f; 

    dice_create(&out_turn->dices[i], pos);
  }

  // Cursor init

  nikola::transform_translate(out_turn->cursor, out_turn->dices[0].transform.position);
  nikola::transform_scale(out_turn->cursor, nikola::Vec3(1.0f));

  // Roll the dice and start an early turn
  turn_start(*out_turn);
}

void turn_reset(Turn& turn) {
  for(nikola::sizei i = 0; i < DICES_MAX; i++) {
    dice_reset(turn.dices[i]);
  }
  
  turn.points          = 0;
  turn.unbanked_points = 0;  
  turn.eval_points     = 0; 

  turn.is_farkle   = false; 
  turn.dices_count = DICES_MAX;
}

void turn_process_input(Turn& turn) {
  // Traverse between the shown dice

  if(nikola::input_key_pressed(nikola::KEY_LEFT)) {
    turn.dice_cursor--;
  }
  else if(nikola::input_key_pressed(nikola::KEY_RIGHT)) {
    turn.dice_cursor++;
  }
  
  turn.dice_cursor = nikola::clamp_int(turn.dice_cursor, 0, DICES_MAX - 1); 

  // Set the cursor's position to the currently selected dice (if it's active.

  Dice* dice = &turn.dices[turn.dice_cursor];
  
  nikola::Vec3 dice_pos = turn.cursor.position;
  nikola::transform_translate(turn.cursor, nikola::Vec3(dice->transform.position.x, dice_pos.y, dice_pos.z));
   
  // Select the dice
  
  if(nikola::input_key_pressed(nikola::KEY_SPACE)) {
    dice_toggle_select(*dice, !dice->is_selected);
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
  }
}

void turn_start(Turn& turn) {
  for(nikola::sizei i = 0; i < DICES_MAX; i++) {
    dice_roll(turn.dices[i]);
  }

  nikola::i32 eval_res = rulebook_evaluate_active(turn.dices);
  turn.is_farkle       = (eval_res <= 0);
}

void turn_continue(Turn& turn) {
  // We have to check for a valid evaluation first

  if(turn.eval_points == 0) {
    // De-select the currently selected dice
    
    for(nikola::sizei i = 0; i < DICES_MAX; i++) {
      dice_toggle_select(turn.dices[i], false);
    }

    return;
  } 

  // The evaluation was fine. 
  // Add the evaluation result to the accumulated points, 
  // and discard the dice that were selected.

  turn.unbanked_points += turn.eval_points;  
  for(nikola::sizei i = 0; i < DICES_MAX; i++) {
    // The dice was successfully discarded and therefore 
    // it will be ignored.

    if(dice_try_discard(turn.dices[i])) { 
      turn.dices_count--;
      continue;
    }

    dice_toggle_select(turn.dices[i], false);
  }

  // Guess if the next turn is going to be a farkle or not
  if(turn.dices_count > 0) {
    turn_start(turn);
  }
}

void turn_bank(Turn& turn) {
  // We have to check for a valid evaluation first

  if(turn.eval_points == 0) {
    // De-select the currently selected dice
    
    for(nikola::sizei i = 0; i < DICES_MAX; i++) {
      dice_toggle_select(turn.dices[i], false);
    }

    return;
  } 

  // Bank the points otherwise

  turn.points          += (turn.unbanked_points + turn.eval_points);
  turn.unbanked_points = 0;  
  turn.eval_points     = 0; 

  turn.dices_count = DICES_MAX;

  for(nikola::sizei i = 0; i < DICES_MAX; i++) {
    dice_reset(turn.dices[i]);
  } 

  turn_bank(turn);
}

/// Turn functions
/// ----------------------------------------------------------------------
