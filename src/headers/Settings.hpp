#pragma once
#include <limits>

namespace RTSetings{

    inline const bool SHOW_FPS = true;
    inline const uint16_t MAX_FPS = 600;

    // performance and RAM heavy if used
    // TODO: kinda broken rn
    inline const bool USE_ANTI_ALIASING = false;
    // used for anti-aliasing
    inline const int SAMPLES_PER_PIXEL = 10;

    inline const float ASPECT_RATIO = 16.0f / 9.0f;

    // commonly: 320, 640, 960, 1280, 1920
    inline const uint16_t WINDOW_WIDTH = 960;
    inline const uint16_t WINDOW_HEIGHT = WINDOW_WIDTH / ASPECT_RATIO;
}
