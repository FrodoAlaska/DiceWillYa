#include "entities.h"

#include <nikola/nikola_pch.h>

/// ----------------------------------------------------------------------
/// Rules

static RuleEvalFunc s_rules[RULES_MAX];

/// Rules
/// ----------------------------------------------------------------------

/// ----------------------------------------------------------------------
/// Callbacks

static const int accumulate_kinds(const nikola::i32* values, const nikola::sizei values_count, const nikola::u32 kind) {
  int count = 0;

  for(nikola::sizei i = 0; i < values_count; i++) {
    if(values[i] == kind) {
      count++;
    }
  }

  return count;
}

static const int ones(const nikola::i32* values, const nikola::sizei values_count) {
  int count = accumulate_kinds(values, values_count, 1);

  switch(count) {
    case 0:
      return 0;
    case 3:
    case 4:
      return 1000;
    case 5:
      return 2000;
    case 6:
      return 3000;
    default:
      return 100 * count;
  }
}

static const int twos(const nikola::i32* values, const nikola::sizei values_count) {
  int count = accumulate_kinds(values, values_count, 2);

  switch(count) {
    case 1:
    case 2:
      return -1;
    case 3:
      return 200;
    case 4:
      return 1000;
    case 5:
      return 2000;
    case 6:
      return 3000;
    default:
      return 0;
  }
}

static const int threes(const nikola::i32* values, const nikola::sizei values_count) {
  int count = accumulate_kinds(values, values_count, 3);

  switch(count) {
    case 1:
    case 2:
      return -1;
    case 3:
      return 300;
    case 4:
      return 1000;
    case 5:
      return 2000;
    case 6:
      return 3000;
    default:
      return 0;
  }
}

static const int fours(const nikola::i32* values, const nikola::sizei values_count) {
  int count = accumulate_kinds(values, values_count, 4);

  switch(count) {
    case 1:
    case 2:
      return -1;
    case 3:
      return 400;
    case 4:
      return 1000;
    case 5:
      return 2000;
    case 6:
      return 3000;
    default:
      return 0;
  }
}

static const int fives(const nikola::i32* values, const nikola::sizei values_count) {
  int count = accumulate_kinds(values, values_count, 5);

  switch(count) {
    case 0:
      return 0;
    case 3:
      return 500;
    case 4:
      return 1000;
    case 5:
      return 2000;
    case 6:
      return 3000;
    default:
      return 50 * count;
  }
}

static const int sixes(const nikola::i32* values, const nikola::sizei values_count) {
  int count = accumulate_kinds(values, values_count, 6);

  switch(count) {
    case 1:
    case 2:
      return -1;
    case 3:
      return 600;
    case 4:
      return 1000;
    case 5:
      return 2000;
    case 6:
      return 3000;
    default:
      return 0;
  }
}

static const int run_sequence(const nikola::i32* values, const nikola::sizei values_count) {
  if(values_count <= 0) {
    return 0;
  }

  // Sort the array in ascending order
  
  nikola::i32* sorted_values = (nikola::i32*)values;
  bubble_sort(sorted_values, values_count);

  // Check if the sequence is present

  bool is_sequence = false;
  for(nikola::sizei i = 1; i <= DICES_MAX; i++) {
    if(sorted_values[i - 1] == (nikola::i32)i) {
      is_sequence = true;
      continue;
    }

    is_sequence = false;
    break;
  }

  return (is_sequence) ? 2500 : 0;
}

/// Callbacks
/// ----------------------------------------------------------------------

/// ----------------------------------------------------------------------
/// Rulebook functions

void rulebook_init() {
  s_rules[0] = ones;
  s_rules[1] = twos;
  s_rules[2] = threes;
  s_rules[3] = fours;
  s_rules[4] = fives;
  s_rules[5] = sixes;
  s_rules[6] = run_sequence;
}

const nikola::i32 rulebook_evaluate_active(Dice* dices) {
  nikola::i32 res = 0; 

  // Gather the active dices

  nikola::i32 active_values[DICES_MAX];
  nikola::sizei values_count = 0; 

  for(nikola::sizei i = 0; i < DICES_MAX; i++) {
    if(dices[i].is_active) {
      active_values[values_count++] = dices[i].value;
    }
  }

  // Accumulate the results

  for(nikola::sizei i = 0; i < RULES_MAX; i++) {
    res += s_rules[i](active_values, values_count);
  }

  return res;
}

const nikola::i32 rulebook_evaluate_selected(Dice* dices) {
  nikola::i32 eval_res = 0; 

  // Gather the selected dices

  nikola::i32 selected_values[DICES_MAX];
  nikola::sizei values_count = 0; 

  for(nikola::sizei i = 0; i < DICES_MAX; i++) {
    if(dices[i].is_selected && dices[i].is_active) {
      selected_values[values_count++] = dices[i].value;
    }
  }

  // Accumulate the results

  for(nikola::sizei i = 0; i < RULES_MAX; i++) {
    nikola::i32 res = s_rules[i](selected_values, values_count);
    if(res == -1) { // An invalid or incompatible value found
      return 0;
    }
 
    eval_res += res;
  }

  return eval_res;
}

/// Rulebook functions
/// ----------------------------------------------------------------------
