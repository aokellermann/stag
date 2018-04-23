#include "png.h"

PNG* png_read_file(char* file_path) {
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
    unsigned char header[8];
    if (fread(header, sizeof(char), 8, fp) != 8) {
        puts("Issue reading file.");
        fclose(fp);
        return NULL;
    }

    // Return NULL if not PNG formatted
    if (png_sig_cmp(header, 0, 8) != 0) {
        puts("Not PNG formatted.");
        fclose(fp);
        return NULL;
    }

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) {
        puts("Error creating PNG read struct.");
        fclose(fp);
        return NULL;
    }

    png_infop info = png_create_info_struct(png);
    if (!info) {
        puts("Error creating PNG info struct.");
        png_destroy_read_struct(&png, NULL, NULL);
        fclose(fp);
        return NULL;
    }

    png_init_io(png, fp);
    png_set_sig_bytes(png, 8);
    png_read_info(png, info);

    PNG* png_image = malloc(sizeof(PNG));
    png_image->width = png_get_image_width(png, info);
    png_image->height = png_get_image_height(png, info);
    png_image->bit_depth = png_get_bit_depth(png, info);
    png_image->color_type = png_get_color_type(png, info);
    png_image->row_bytes = png_get_rowbytes(png, info);
    png_image->components = png_get_channels(png, info);
    if (png_image->bit_depth == 16)
        png_set_strip_16(png);

    if (png_image->color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png);

    if (png_image->color_type == PNG_COLOR_TYPE_GRAY && png_get_bit_depth(png, info) < 8)
        png_set_expand_gray_1_2_4_to_8(png);

    if (png_get_valid(png, info, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png);

    if (png_image->color_type == PNG_COLOR_TYPE_GRAY || png_image->color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(png);

    png_read_update_info(png, info);

    png_bytepp row_pointers = malloc(sizeof(png_bytep) * png_image->height);
    for (unsigned int i = 0; i < png_image->height; i++)
        row_pointers[i] = malloc(png_image->row_bytes);

    png_image->row_pointers = row_pointers;
    png_read_image(png, row_pointers);
    png_read_end(png, info);
    png_destroy_read_struct(&png, &info, NULL);
    fclose(fp);
    return png_image;
}

void png_write_file(PNG* png, char* file_path) {
    FILE* fp = fopen(file_path, "wb");
    if (!fp) {
        puts("Failed to open file!");
        return;
    }

    png_structp png_write = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop info = png_create_info_struct(png_write);
    png_init_io(png_write, fp);

    if (setjmp(png_jmpbuf(png_write))) {
        puts("[write_png_file] Error during writing header");
        return;
    }

    png_set_IHDR(png_write, info, png->width, png->height, png->bit_depth, png->color_type,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
    png_write_info(png_write, info);
    png_write_image(png_write, png->row_pointers);
    png_write_end(png_write, NULL);
    png_destroy_write_struct(&png_write, &info);
    fclose(fp);
}

void png_inject_png(PNG* super_png, PNG* sub_png){
    png_bytep super_row, sub_row;
    png_byte data_point;
    int index;
    for (unsigned int i = 0; i < super_png->height; i++) {
        super_row = super_png->row_pointers[i];
        sub_row = sub_png->row_pointers[i];
        for (unsigned int j = 0; j < super_png->width; j++) {
            for (int k = 0; k < super_png->components; k++) {
                index = k + j * super_png->components;
                data_point = super_row[index];
                data_point >>= 2;
                data_point <<= 2; // Clears right two bits
                // Sets right two bits to value from 0 to 3
                super_row[index] = (unsigned char) (data_point + sub_row[index] / 64);
            }
        }
    }
}

PNG* png_extract_png(PNG* super_png){
    PNG* sub_png = malloc(sizeof(PNG));
    sub_png->row_pointers = malloc(sizeof(png_bytep) * super_png->height);
    for (unsigned int i = 0; i < super_png->height; i++) {
        sub_png->row_pointers[i] = malloc(super_png->row_bytes);
        memcpy(sub_png->row_pointers[i], super_png->row_pointers[i], super_png->row_bytes);
    }
    sub_png->width = super_png->width;
    sub_png->height = super_png->height;
    sub_png->bit_depth = super_png->bit_depth;
    sub_png->color_type = super_png->color_type;
    sub_png->row_bytes = super_png->row_bytes;
    sub_png->components = super_png->components;

    png_bytep super_row, sub_row;
    png_byte data_point;
    int index;
    for (unsigned int i = 0; i < super_png->height; i++) {
        for (unsigned int j = 0; j < super_png->width; j++) {
            for (int k = 0; k < super_png->components; k++) {
                super_row = super_png->row_pointers[i];
                sub_row = sub_png->row_pointers[i];
                index = k + j * super_png->components;
                data_point = super_row[index];
                data_point <<= 6;
                data_point >>= 6; // Clears left 6 bits
                sub_row[index] = (unsigned char) (data_point * 85);
            }
        }
    }
    return sub_png;
}

void png_destroy(PNG** phPNG) {
    PNG* ptr = *phPNG;
    for (unsigned int i = 0; i < ptr->height; i++)
        free(ptr->row_pointers[i]);
    free(ptr->row_pointers);
    free(ptr);
    *phPNG = NULL;
}