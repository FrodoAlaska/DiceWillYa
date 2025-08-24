#pragma once 

#include <nikola/nikola_pch.h>

/// ----------------------------------------------------------------------
/// Pop-up manager functions 

void popup_manager_init(nikola::Window* window);

void popup_manager_spawn(const nikola::String& text, 
                         const nikola::Vec3 color, 
                         const nikola::Vec2 offset = nikola::Vec2(0.0f, -128.0f));

void popup_manager_update();

void popup_manager_render();

/// Pop-up manager functions 
/// ----------------------------------------------------------------------
