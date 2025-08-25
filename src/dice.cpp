#include "entities.h"
#include "game_event.h"

#include <nikola/nikola_pch.h>

#include <random>

/// ----------------------------------------------------------------------
/// Private functions

// @TEMP
static const nikola::i32 better_random(Dice& dice) {
  std::random_device rand_device;
  std::mt19937 engine(rand_device());
  
  std::uniform_int_distribution<nikola::i32> uni_dist(1, DICES_MAX);
  nikola::i32 rand = uni_dist(engine);

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
  nikola::transform_scale(out_dice->transform, nikola::Vec3(1.5f));
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
    dest_pos = nikola::Vec3(2.0f, 2.0f, dest_pos.z);
  }
  else {
    dest_pos   = dice.initial_pos; 
    dest_pos.y = dice.is_selected ? 7.5f : 7.0f; 
  }

  nikola::transform_lerp_position(dice.transform, dest_pos, dt * 4.1f);
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
  dice.transform.position.y = 7.0f;
  dice.rotation             = get_rotation_from_value(dice.value);

  nikola::transform_translate(dice.transform, dice.transform.position);
}

/// Dice functions
/// ----------------------------------------------------------------------
