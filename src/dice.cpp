#include "entities.h"

#include <nikola/nikola_pch.h>

/// ----------------------------------------------------------------------
/// Private functions

// @TEMP
static const nikola::i32 better_random(Dice& dice) {
  nikola::i32 rand = nikola::random_i32(1, DICES_MAX);

  if(rand == dice.value) { // Try again if we got the same number
    rand = nikola::random_i32(1, DICES_MAX);
  }

  return rand;
}

/// Private functions
/// ----------------------------------------------------------------------

/// ----------------------------------------------------------------------
/// Dice functions

void dice_create(Dice* out_dice, const nikola::Vec3 start_pos) {
  out_dice->initial_pos = start_pos;
  out_dice->value       = better_random(*out_dice);
  out_dice->rotation    = get_rotation_from_value(out_dice->value);

  nikola::transform_translate(out_dice->transform, out_dice->initial_pos);
  nikola::transform_rotate(out_dice->transform, out_dice->rotation);
  nikola::transform_scale(out_dice->transform, nikola::Vec3(8.0f));
}

void dice_reset(Dice& dice) {
  dice.is_active   = true; 
  dice.is_selected = false;
  dice.value       = better_random(dice);
  dice.rotation    = get_rotation_from_value(dice.value);

  nikola::transform_translate(dice.transform, dice.initial_pos);
}

void dice_update(Dice& dice, const nikola::f32 dt) {
  nikola::Vec3 dest_pos = dice.transform.position;

  if(!dice.is_active) {
    dest_pos = nikola::Vec3(dest_pos.x, 10.0f, -10.0f);
  }
  else {
    dest_pos   = dice.initial_pos; 
    dest_pos.y = dice.is_selected ? -1.0f : -3.0f; 
  }

  nikola::transform_lerp_position(dice.transform, dest_pos, dt * 5.1f);
  nikola::transform_lerp_rotation(dice.transform, nikola::Vec3(dice.rotation), dice.rotation.w, dt * 5.1f);
}

const bool dice_try_discard(Dice& dice) {
  if(dice.is_selected && dice.is_active) {
    dice.is_active = false;
    return true;
  }

  return false;
}

void dice_toggle_select(Dice& dice, const bool selected) {
  if(!dice.is_active) {
    return;
  }

  dice.is_selected = selected;
}

void dice_roll(Dice& dice) {
  if(!dice.is_active) {
    return;
  }

  dice.value                = better_random(dice);
  dice.is_selected          = false;
  dice.transform.position.y = -3.0f;
  dice.rotation             = get_rotation_from_value(dice.value);

  nikola::transform_translate(dice.transform, dice.transform.position);
}

/// Dice functions
/// ----------------------------------------------------------------------
