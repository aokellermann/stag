#ifndef STAG_PNG_H
#define STAG_PNG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <zlib.h>
#include <png.h>

struct png {
    png_bytepp row_pointers;
    png_uint_32 width;
    png_uint_32 height;
    png_byte bit_depth;
    png_byte color_type;
};

typedef struct png PNG;

/**
 * Stores a .png file in a PNG struct and returns it
 * @param file_path .png file path to read from
 * @return PNG struct pointer
 */
PNG* png_read_file(char* file_path);

/**
 * Writes a PNG struct's data to a new file
 * @param png PNG struct
 * @param file_path .png file path to write to
 */
void png_write_file(PNG* png, char* file_path);

/**
 * Frees any heap allocated data contained in a PNG struct and sets the handle to NULL
 * @param phPNG pointer to handle to PNG struct
 */
void png_destroy(PNG** phPNG);

#endif