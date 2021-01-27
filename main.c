#include "watch_this.h"

void wt_modified_callback(const char* file_name) {
    printf("\'%s\' has been modified...\n", file_name);
}

void wt_missing_callback(const char* file_name) {
    printf("\'%s\' has been moved of removed...\n", file_name);
}

int main(void) {
    wtFile file = wt_file_create("main.c", wt_modified_callback, wt_missing_callback);

    while (true) {
        wt_file_update(&file);
    }

    wt_file_destroy(&file);

    return 0;
}
