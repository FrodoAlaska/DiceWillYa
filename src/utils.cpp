#include "entities.h"

#include <nikola/nikola_pch.h>

/// ----------------------------------------------------------------------
/// Utils

const nikola::Vec4 get_rotation_from_value(const nikola::i32 value) {
  switch(value) {
    case 1:
      return nikola::Vec4(1.0f, 0.0f, 0.0f, 180.0f * nikola::DEG2RAD);
    case 2:
      return nikola::Vec4(1.0f, 0.0f, 0.0f, 270.0f * nikola::DEG2RAD);
    case 3:
      return nikola::Vec4(0.0f, 0.0f, 1.0f, 90.0f * nikola::DEG2RAD);
    case 4:
      return nikola::Vec4(1.0f, 0.0f, 0.0f, 90.0f * nikola::DEG2RAD);
    case 5:
      return nikola::Vec4(0.0f, 0.0f, 1.0f, -90.0f * nikola::DEG2RAD);
    case 6:
      return nikola::Vec4(0.0f, 1.0f, 0.0f, 90.0f * nikola::DEG2RAD);
    default:
      return nikola::Vec4(-1.0f);
  }
}

void bubble_sort(nikola::i32* values, const nikola::sizei values_count) {
  nikola::i32 top  = 0;
  nikola::f32 temp = 0.0f;

  do {
    for(nikola::i32 i = values_count - 1; i > top; i--) {
      if(values[i] < values[i - 1]) {
        temp = values[i];

        values[i]     = values[i - 1];
        values[i - 1] = temp;
      }

      top++;
    }
  } while(top < (values_count - 1));
}

/// Utils
/// ----------------------------------------------------------------------
