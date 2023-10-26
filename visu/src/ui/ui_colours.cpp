#include "ui/ui_colours.h"

static std::array<ImVec4, 256> colour_lookup;

/// Convert HSV to RGB
ImVec4 UIColours::HSVtoRGB(float h) {
  float r, g, b;

  int i   = int(h * 6.0f);
  float f = (h * 6.0f) - (float)i;
  float q = (float)1.0f * (1.0f - f * 1.0f);
  float t = (float)1.0f * (1.0f - (1.0f - f) * 1.0f);

  switch (i % 6) {
    case 0:
      r = 1.0, g = t, b = 0;
      break;
    case 1:
      r = q, g = 1.0, b = 0;
      break;
    case 2:
      r = 0, g = 1.0, b = t;
      break;
    case 3:
      r = 0, g = q, b = 1.0;
      break;
    case 4:
      r = t, g = 0, b = 1.0;
      break;
    case 5:
      r = 1.0, g = 0, b = q;
      break;
    default:
      r = 0, g = 0, b = 0;
      break;
  }

  return {r, g, b, 0.8f};
}

void UIColours::init() {
  for (uint8_t i = 0x00; i < 0xFF; ++i) {
    float hue = (float)i / 280.0f;  // Use > 255 so the colors will not wrap
                                    // (differentiate 0x00 and 0xFF)
    colour_lookup[i] = HSVtoRGB(hue);
  }
}

ImVec4 UIColours::get_hex_color(uint8_t value) {
  return colour_lookup.at(value);
}

// Compute the luminance of the background color, if the luminance is greater
// than a threshold, use black; otherwise, use white
ImU32 UIColours::get_text_color(ImVec4& background_color) {
  float luminance = 0.299f * background_color.x + 0.587f * background_color.y +
                    0.114f * background_color.z;

  return luminance > 0.5f ? IM_COL32(0, 0, 0, 255)
                          : IM_COL32(255, 255, 255, 255);
}
