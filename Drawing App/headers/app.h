#pragma once

#include <draw.h>                   // For types

// Screen dimensions
#define SCREEN_WIDTH    1280
#define SCREEN_HEIGHT   720

AppState app_initSystem();          // Side-effects!!
void app_close(AppState finalState);
