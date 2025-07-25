#pragma once

namespace RTSetings{

    inline const bool SHOW_FPS = true;
    inline const uint16_t MAX_FPS = 30;

    // lower performance if used
    inline bool USE_ANTI_ALIASING = false;

    inline const float ASPECT_RATIO = 16.0f / 9.0f;

    // commonly: 320, 640, 960, 1280, 1920
    inline const uint16_t WINDOW_WIDTH = 960;
    inline const uint16_t WINDOW_HEIGHT = WINDOW_WIDTH / ASPECT_RATIO;
}
