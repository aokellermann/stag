#include "ppm.h"

int main(int argc, char* argv[]) {
    int inject;
    if (argc == 5 && strcmp("inject", argv[1]) == 0)
        inject = 1;
    else if (argc == 4 && strcmp("extract", argv[1]) == 0)
        inject = 0;
    else {
        puts("Usage:\n$stag inject super_input sub_input output\nor\n"
             "$stag extract input output");
        return 0;
    }

    char* ext1 = &argv[2][strlen(argv[2]) - 4], * ext2 = &argv[3][strlen(argv[3]) - 4];
    if (strcmp(ext1, ext2) != 0) {
        puts("Incompatible extensions.");
        return 0;
    }

    if (strcmp(ext1, ".ppm") == 0) {
        PPM* super_ppm = ppm_read_file(argv[2]);
        PPM* sub_ppm = NULL;
        if (inject) {
            sub_ppm = ppm_read_file(argv[3]);
            ppm_inject_ppm(super_ppm, sub_ppm);
            ppm_write_file(super_ppm, argv[4]);
        } else {
            sub_ppm = ppm_extract_ppm(super_ppm);
            ppm_write_file(sub_ppm, argv[3]);
        }
        ppm_destroy(&super_ppm);
        ppm_destroy(&sub_ppm);
    }

    return 0;
}