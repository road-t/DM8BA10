#ifndef ROADT_CYRILLIC_MIN
#define ROADT_CYRILLIC_MIN

#include "charset.h"

/* 
 *  Basic Latin characters + Cyrillic characters used in Russian, Ukrainian,
 *  Belorussian, Rusyn, Serbian, Bulgarian and Macedonian languages
*/

class CyrillicMinCharset : public Charset
{
  public:
    const word chars[96] =
    {
    // 0x00:
      0x0000, 0x0540, 0x0060, 0x156B, 0xB5CB, 0xBC5B, 0xD345, 0x0040, //  !"#$%&'
      0x0014, 0x4800, 0x5C5C, 0x1448, 0x0800, 0x1008, 0x0100, 0x0810, // ()*+,-./
    // 0x10:  
      0xABB3, 0x0032, 0x93A9, 0x81AB, 0x302A, 0xB18B, 0xB38B, 0x80A2, // 01234567
      0xB3AB, 0xB1AB, 0x0440, 0x0840, 0x1014, 0x1109, 0x4808, 0x84A8, // 89:;<=>?
    // 0x20:
      0xB2AA, 0xB38B, 0x85EB, 0xA280, 0x0933, 0xB381, 0x366A, 0x81AB, // АБВГДЕЖЗ
      0x2A32, 0x2AB2, 0x3214, 0x0832, 0x6232, 0x322A, 0xA3A3, 0xA2A2, // ИЙКЛМНОП
    // 0x30:
      0xB2A8, 0xA381, 0x84C0, 0x312B, 0xB4E8, 0x4814, 0x2741, 0x302A, // РСТУФХЦЧ
      0x2763, 0x2763, 0x844B, 0x3722, 0x330B, 0x81AB, 0x36E3, 0xB8AA, // ШЩЪЫЬЭЮЯ
    // 0x40
      0x1704, 0x041B, 0x7700, 0x1401, 0xB540, 0x1B00, 0x4C54, 0x009B, // абвгдежз
      0x0E00, 0x4E00, 0x0454, 0x0C00, 0x160A, 0x3640, 0x1700, 0x1600, // ийклмноп
    // 0x50
      0xB240, 0x1300, 0x1408, 0x4810, 0x170B, 0x4814, 0x0700, 0x1400, // рстуфхцч
      0x0703, 0x0703, 0x1405, 0x0407, 0x044B, 0x000F, 0x0E0B, 0xBC40, // шщъыьэюя
    };
  
    inline word Char(word index)
    {
      return index < size() ? chars[index] : spacer();
    }
    
    inline word size() override { return 0x60; }

    /*
     * remapUTF takes a pointer to char* array
     * and removes all the UTF-8 prefixes for
     * cyrillic characters, fixing even macOS 
     * 'wrong' Ë code (0xC38B)
     */
    String& remap(String& string) override
    { 
      word curLen = 0;
      byte prefix = 0;
    
      for (word i = 0; i < string.length(); i++)
      {
        byte ch = string[i];
    
        if (ch == 0xD0 || ch == 0xD1 || ch == 0xC3)
        {
          prefix = ch;
          continue;
        }

        if (prefix)
        {
          switch (prefix)
          {
            case 0xC3:
              if (ch == 0x8B)
                ch = 0x35;
            break;

            case 0xD0:
              ch -= 0x50;
            break;

            case 0xD1:
            ch -= 0x10;
            break;
          }
         
          prefix = 0;
        }
    
        string[curLen++] = ch;
      }

      string = string.substring(0, curLen);
      
      return string; 
    }
};

#endif
