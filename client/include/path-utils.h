#ifndef _PATH_UTILS_H_
#define _PATH_UTILS_H_

#include "linked-list.h"
#include <sys/stat.h>
#include <dirent.h>

bool check_if_directory_exists(const char* path);
bool check_if_file_exists(const char* path);
int create_path(const char* path);
int free_listing(linked_list* listing);
int get_directory_listing(const char* path, linked_list* listing);


#endif

