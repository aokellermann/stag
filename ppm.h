#ifndef STAG_PPM_H
#define STAG_PPM_H

#define COMPONENTS 3

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

struct ppm {
    unsigned char* image;
    unsigned int width;
    unsigned int height;
};

typedef struct ppm PPM;

/**
 * Stores a .ppm file in a PPM struct and returns it
 * @param file_path .ppm file path to read from
 * @return PPM struct pointer
 */
PPM* ppm_read_file(char* file_path);

/**
 * Writes a PPM struct's data to a new file
 * @param ppm PPM struct
 * @param file_path .ppm file path to write to
 */
void ppm_write_file(PPM* ppm, char* file_path);

/**
 * Overwrites the two least significant bits of super_ppm with the data from sub_ppm
 * @param super_ppm the PPM to inject into
 * @param sub_ppm the PPM to inject from
 */
void ppm_inject_ppm(PPM* super_ppm, PPM* sub_ppm);

/**
 * Reads the two least significant bits of super_ppm and returns the data in a PPM struct
 * @param super_ppm PPM struct that has previously been injected
 * @return a PPM struct containing extracted data
 */
PPM* ppm_extract_ppm(PPM* super_ppm);

/**
 * Frees any heap allocated data contained in a PPM struct and sets the handle to NULL
 * @param phPPM pointer to handle to PPM struct
 */
void ppm_destroy(PPM** phPPM);

#endif