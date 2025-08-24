#pragma once

#include "entities.h" // Ew!

#include <nikola/nikola_pch.h>

/// ----------------------------------------------------------------------
/// App functions 

nikola::App* app_init(const nikola::Args& args, nikola::Window* window);

void app_shutdown(nikola::App* app);

void app_update(nikola::App* app, const nikola::f64 delta_time);

void app_render(nikola::App* app);

void app_render_gui(nikola::App* app);

const Turn* app_get_current_turn(nikola::App* app); // Ew, ew, ew, ew, ew

/// App functions 
/// ----------------------------------------------------------------------
