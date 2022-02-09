#ifndef ROADT_LCD_EFFECTS
#define ROADT_LCD_EFFECTS

typedef word Sequence;

/*
 * Some presets for animation
 * pass it to drawChar() function
 * increasing frame position
 */

const Sequence Spinstar[] =
{
  1, 256, 512, 8192, 32768, 128, 32, 2, // go around
  4, 16388, 16404, 18452, 22548, 22612, 22620, 23644, // fill the middle
  65535, // all the segments
  65535, 65535 // remain filled
};

const Sequence Valve[] =
{
  5192, // +
  18452, // X
  5192, // +
  33313, // dashed border 0
  8578,  // dashed border 1
  41891, // full border
  41891,
  0,
};

const Sequence Typing[] =
{
 257, // underscore
 0,
 0,
 257,
 0,
 0,
 257,
 0,
 0,
};

const Sequence Loading[] = 
{
  2, 1024, 512, 2, 1024, 512,
};

const Sequence Snake[] = 
{
  0, 512, 8704, 41472, 41024, 33856, 1089,  1027, 35, 34, 32,
  0, 512, 8704, 41472, 41024, 33856, 1089,  1027, 35, 34, 32,
};

#endif
