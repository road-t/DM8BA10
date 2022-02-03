#ifndef ROADT_CHARSET
#define ROADT_CHARSET

class Charset
{
  public:
    /*
     * These functions are obligatory to be overridden
     */
    // main function that return data for character by its code
    virtual word Char(word index) = 0;

    // this function should return character table size
    virtual word size() = 0;

    /*
     * The functions below are auxillary
     */
    // a character to be displayed instead of absent ones
    virtual inline word dummy() { return 0xFFFF; }
    
    // first character code
    virtual inline int16_t startingOffset() { return 0x20; }

    // redefine this function to map similar characters to one,
    // eliminate characterset gaps, add auxillary characters support, etc.
    virtual inline String& remap(String& string) { return string; }
};

#endif
