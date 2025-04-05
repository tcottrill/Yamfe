#ifndef FONT_HELPER_H
#define FONT_HELPER_H

#include <string>

extern void font_init();
extern void font_set_angle(int angle);
extern void font_set_scale(float scale);
extern void font_set_color(rgb_t color);
extern float font_get_string_width(std::string &string);
extern void font_print(float x, float y, std::string &string);
extern void font_print(float x, float y, rgb_t color, float scale, std::string &string);
extern void font_render();
extern void font_destroy();

#endif