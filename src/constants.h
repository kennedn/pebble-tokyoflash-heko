#define ST_TOP_OUTER_LEDS 0; // Byte Array
#define ST_TOP_INNER_LEDS 1; // Byte Array
#define ST_MIDDLE_LEDS 2; // Byte Array
#define ST_BOTTOM_OUTER_LEDS 3; // Byte Array
#define ST_BOTTOM_INNER_LEDS 4; // Byte Array
#define ST_TOP_BG 5; // Bool
#define ST_MIDDLE_BG 6; // Bool
#define ST_BOTTOM_BG 7; // Bool
#define ST_VIBE_DISCONNECT 8; // Integer
#define ST_VIBE_CONNECT 9; // Integer
#define ST_VIBE_HOUR 10; // Integer
#define ST_DATE_ENABLED 11; // Bool
#define ST_ALWAYS_ON 12; // Bool
#define ST_ANIMATION_ENABLED 13; // Bool
#define ST_DISPLAY_TIMEOUT 13; // Integer

#define OL_NONE 0;
#define OL_ADD 1;
#define OL_REMOVE 2;

#define LED_NONE 0;
#define LED_HALFTONE 1;
#define LED_SOLID 2;

#define BG_DARK 0;
#define BG_LIGHT 1;

#define VIBE_NONE 0;
#define VIBE_SHORT 1;
#define VIBE_LONG 2;
#define VIBE_DOUBLE 3;