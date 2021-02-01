#ifndef WATCH_THIS_H
#define WATCH_THIS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <string.h>
#include <assert.h>
#include <time.h>

#if defined(__win32__)
    #include <winbase.h>
#elif defined(__unix__)
    #include <unistd.h>
    #include <dirent.h>
    #include <sys/types.h>
    #include <sys/stat.h>
#else
    #warning 'watch_this.h' not supported on your targeted OS yet...
#endif

struct wtFile;

typedef void (wt_callback)(const char* file_name);

typedef struct wtFile {

#if defined (__win32__)
    LARGE_INTEGER mod_time;
#elif defined (__unix__)
    time_t mod_time;
#endif
    char* name;
    wt_callback* modified_callback;
    wt_callback* missing_callback;
} wtFile;

wtFile wt_file_create(const char* file_name, wt_callback* modified_callback, wt_callback* missing_callback) {
    if (file_name == NULL) {
        return (wtFile) {};
    }

    wtFile watch_file = {};

#if defined (__win32__)
///////////////////////
#elif defined (__unix__)
    struct stat st;
    stat(file_name, &st);
    watch_file.mod_time = st.st_mtime;
#endif

    watch_file.name = (char*) calloc(strlen(file_name) + 1, sizeof(char));
    assert(watch_file.name != NULL);

    memcpy(watch_file.name, file_name, sizeof(char) * strlen(file_name));

    watch_file.modified_callback = modified_callback;
    watch_file.missing_callback  = missing_callback;

    printf("Watching file:\n\t%s\n", file_name);

    return watch_file;
}

void wt_file_destroy(wtFile* watch_file) {
    if (watch_file == NULL) {
        return;
    }

    watch_file->modified_callback = NULL;
    watch_file->missing_callback  = NULL;

    free(watch_file->name);
}

void wt_file_update(wtFile* file) { // add callback ok cool
    if (file == NULL) {
        return;
    }

    
#if defined (__win32__)
///////////////////////
#elif defined (__unix__)
    struct stat st;

    if (file->mod_time < st.st_mtime) {
        file->mod_time = st.st_mtime;
        file->modified_callback(file->name);
    }

    if (stat(file->name, &st) != 0) {
        file->missing_callback(file->name);
    }

#endif
}

#endif // WATCH_THIS_H

// WATCHING DIRECTORIES
//
// #define WT_FILE_LIST_MAX_CAPACITY UINT16_MAX
//
// typedef struct wtFileList {
//     size_t  length;
//     wtFile* data;
// } wtFileList;
//
// wtFileList wt_file_list_create(void) {
//     wtFileList file_list = {};
//     file_list.length = 0;
//     file_list.data = (wtFile*) calloc(WT_FILE_LIST_MAX_CAPACITY, sizeof(wtFile));
//
//     if (file_list.data == NULL) {
//         return (wtFileList) {};
//     }
//
//     return file_list;
// }
//
// void wt_file_list_destroy(wtFileList* file_list) {
//     free(file_list->data);
//     file_list->data = NULL;
// }
//
// typedef struct wtDirectory { // muliple files to watch
//     unsigned char file_count;
//     wtFile*       files;
// } wtDirectory;
//
// watch by extensions? recursive watch?
// wtDirectory watch_this_directory_create(const char* directory_path, wt_callback callback) {
//     wtDirectory watch_directory = {};
//
// #if defined (__win32__)
// ///////////////////////
// #elif defined (__unix__)
//     DIR* directory_stream = opendir(directory_path);
//
//     // NOTE:
//     //  is there a way we can load files into our struct
//     //  without winding and rewinding directory stream
//
//     struct dirent* dirent = NULL;
//     if (directory_stream) {
//         while ((dirent = readdir(directory_stream)) != NULL) {
//             if (strcmp(dirent->d_name, ".") != 0 && strcmp(dirent->d_name, "..") != 0) {
//                 watch_directory.file_count++;
//             }
//         }
//
//         rewinddir(directory_stream);
//
//         watch_directory.files = (wtFile*) calloc(watch_directory.file_count, sizeof(wtFile));
//         // assert plzz
//
//         unsigned char current_file = 0;
//         while ((dirent = readdir(directory_stream)) != NULL) {
//             if (strcmp(dirent->d_name, ".") != 0 && strcmp(dirent->d_name, "..") != 0) {
//                 char* file_name = (char*) calloc(UINT8_MAX, sizeof(char));
//                 if (file_name == NULL) {
//
//                 }
//
//                 strcat(file_name, directory_path);
//                 strcat(file_name, dirent->d_name);
//
//                 watch_directory.files[current_file++] = watch_this_file_create(file_name, callback);
//
//                 free(file_name);
//              }
//         }
//     }
//
// #endif
//
//     return watch_directory;
// }
//
// void watch_this_directory_destroy(wtDirectory* directory) {
//     for (int i = 0; i < directory->file_count; i++) {
//         watch_this_file_destroy(&directory->files[i]);
//     }
// }
//
// void watch_this_directory_update(wtDirectory* directory) {
//     if (directory == NULL) {
//         return;
//     }
//
//     for (int i = 0; i < directory->file_count; i++) {
//         watch_this_file_update(&directory->files[i]);
//     }
// }
