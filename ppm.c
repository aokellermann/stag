#include "ppm.h"

PPM* ppm_read_file(char* file_path) {
    // Return NULL if file doesn't exist
    struct stat file_info;
    if (stat(file_path, &file_info)) {
        puts("Failed to retrieve file info!");
        return NULL;
    }

    // Return NULL if file cannot be opened
    FILE* fp = fopen(file_path, "rb");
    if (!fp) {
        puts("Failed to open file!");
        return NULL;
    }

    // Return NULL if issue with fread()
    unsigned char* raw_data = malloc((size_t) file_info.st_size);
    if (fread(raw_data, sizeof(char), (size_t) file_info.st_size, fp) != (size_t) file_info.st_size) {
        puts("Issue reading file.");
        fclose(fp);
        return NULL;
    }
    fclose(fp);

    // Return NULL if no P6 header
    if (raw_data[0] != 'P' || raw_data[1] != '6') {
        puts("Not PPM 256 formatted.");
        free(raw_data);
        return NULL;
    }

    char width[16], height[16];
    int i = 3;

    // Skip past comments
    while (raw_data[i] == '#') {
        while (raw_data[i] != '\n')
            i++;
        i++;
    }

    int j;

    // Read width
    for (j = 0; raw_data[i] != ' '; i++, j++)
        width[j] = raw_data[i];
    width[j] = '\0';

    i++;

    // Read height
    for (j = 0; raw_data[i] != '\n'; i++, j++)
        height[j] = raw_data[i];
    height[j] = '\0';

    i++;

    // Return NULL if not 256 color
    if (raw_data[i] != '2' || raw_data[i + 1] != '5' || raw_data[i + 2] != '5') {
        puts("Not PPM 256 formatted.");
        putchar(raw_data[i]);
        free(raw_data);
        return NULL;
    }
    i += 4;

    PPM* ppm = malloc(sizeof(PPM));
    ppm->image = malloc((size_t) file_info.st_size - i); // malloc() file size minus header
    ppm->width = (unsigned int) strtol(width, NULL, 10);
    ppm->height = (unsigned int) strtol(height, NULL, 10);
    memcpy(ppm->image, &raw_data[i], (size_t) ppm->width * ppm->height * 3); // memcpy data after header
    free(raw_data);
    return ppm;
}

void ppm_write_file(PPM* ppm, char* file_path) {
    FILE* fp = fopen(file_path, "wb");
    if (!fp) {
        puts("Failed to open file!");
        return;
    }
    fprintf(fp, "P6\n%d %d\n255\n", ppm->width, ppm->height);
    fwrite(ppm->image, sizeof(char), (size_t) ppm->width * ppm->height * 3, fp);
    fclose(fp);
}

void ppm_inject_ppm(PPM* super_ppm, PPM* sub_ppm) {
    unsigned long index;
    unsigned char data_point;
    for (unsigned int i = 0; i < super_ppm->height; i++) {
        for (unsigned int j = 0; j < super_ppm->width; j++) {
            for (int k = 0; k < COMPONENTS; k++) {
                index = COMPONENTS * (i * super_ppm->width + j) + k;
                data_point = super_ppm->image[index];
                data_point >>= 2;
                data_point <<= 2; // Clears right two bits
                // Sets right two bits to value from 0 to 3
                super_ppm->image[index] = (unsigned char) (data_point + sub_ppm->image[index] / 64);
            }
        }
    }
}

PPM* ppm_extract_ppm(PPM* super_ppm) {
    PPM* sub_ppm = malloc(sizeof(PPM));
    sub_ppm->image = malloc(super_ppm->width * super_ppm->height * COMPONENTS);
    sub_ppm->width = super_ppm->width;
    sub_ppm->height = super_ppm->height;
    unsigned long index;
    unsigned char data_point;
    for (unsigned int i = 0; i < super_ppm->height; i++) {
        for (unsigned int j = 0; j < super_ppm->width; j++) {
            for (int k = 0; k < COMPONENTS; k++) {
                index = COMPONENTS * (i * super_ppm->width + j) + k;
                data_point = super_ppm->image[index];
                data_point <<= 6;
                data_point >>= 6; // Clears left 6 bits
                sub_ppm->image[index] = (unsigned char) (data_point * 85);
            }
        }
    }
    return sub_ppm;
}

void ppm_destroy(PPM** phPPM) {
    free((*phPPM)->image);
    free(*phPPM);
    *phPPM = NULL;
}