#pragma once

// define
static constexpr float PI = 3.14159265358f;
static Vec2f WINDOW_SIZE = { 640.f, 480.f };
static float ASPECT_RATIO = WINDOW_SIZE.x / WINDOW_SIZE.y;
static constexpr Vec2i FIELD_SIZE = { 10, 20 };

// undef
#undef min
#undef max

