#pragma once

#include <nikola/nikola_pch.h>

/// ----------------------------------------------------------------------
/// Consts

const nikola::sizei DICES_MAX = 6;
const nikola::sizei RULES_MAX = 8;
const nikola::sizei ROLLS_MAX = 5;

/// Consts
/// ----------------------------------------------------------------------

/// ----------------------------------------------------------------------
/// Dice 
struct Dice {
  nikola::Transform transform; 
  nikola::Vec3 initial_pos;
  nikola::Vec4 rotation;

  nikola::i32 value = 0;

  bool is_selected = false; 
  bool is_active   = true;
};
/// Dice
/// ----------------------------------------------------------------------

/// ----------------------------------------------------------------------
/// Turn 
struct Turn {
  Dice dices[DICES_MAX];
  nikola::Transform cursor;

  nikola::i32 dice_cursor = 0;

  nikola::i32 points          = 0; 
  nikola::i32 unbanked_points = 0;
  nikola::i32 eval_points     = 0;
  
  nikola::u32 continues = 1;

  nikola::i32 dices_count = DICES_MAX;
  nikola::i32 rolls_count = ROLLS_MAX;

  bool is_farkle = false;
};
/// Turn 
/// ----------------------------------------------------------------------

/// ----------------------------------------------------------------------
/// Callbacks

using RuleEvalFunc = const nikola::i32(*)(const nikola::i32* values, const nikola::sizei values_count);

/// Callbacks
/// ----------------------------------------------------------------------

/// ----------------------------------------------------------------------
/// Utils

const nikola::Vec4 get_rotation_from_value(const nikola::i32 value);

void bubble_sort(nikola::i32* values, const nikola::sizei values_count);

/// Utils
/// ----------------------------------------------------------------------

/// ----------------------------------------------------------------------
/// Dice functions

void dice_create(Dice* out_dice, const nikola::Vec3 start_pos);

void dice_reset(Dice& dice);

void dice_update(Dice& dice, const nikola::f32 dt);

const bool dice_try_discard(Dice& dice);

void dice_toggle_select(Dice& dice, const bool selected);

void dice_roll(Dice& dice);

/// Dice functions
/// ----------------------------------------------------------------------

/// ----------------------------------------------------------------------
/// Turn functions

void turn_create(Turn* out_turn);

void turn_reset(Turn& turn);

void turn_process_input(Turn& turn);

void turn_update(Turn& turn, const nikola::f32 dt);

void turn_start(Turn& turn);

void turn_continue(Turn& turn);

void turn_bank(Turn& turn);

/// Turn functions
/// ----------------------------------------------------------------------

/// ----------------------------------------------------------------------
/// Rulebook functions

void rulebook_init();

const nikola::i32 rulebook_evaluate_active(Dice* dices);

const nikola::i32 rulebook_evaluate_selected(Dice* dices);

/// Rulebook functions
/// ----------------------------------------------------------------------
