typedef struct led_style {
  uint8_t outline;
  uint8_t fill;
  bool accumulative;
  bool invert;
} LEDStyle;

void led_style_serialise(LEDStyle style, uint8_t * array) {
  array[0] = style.outline;
  array[1] = style.fill;
  array[2] = style.accumulative;
  array[3] = style.invert;
}

LEDStyle led_style_deserialise(uint8_t * array) {
  LEDStyle style = {array[0], array[1], !!array[2], !!array[3]};
  return style;
}