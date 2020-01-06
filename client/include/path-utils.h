#ifndef _PATH_UTILS_H_
#define _PATH_UTILS_H_

bool check_if_directory_exists(const char* path);
bool check_if_file_exists(const char* path);
int create_path(const char* path);
int free_listing(dynamic_array* listing);
int get_directory_listing(const char* path, dynamic_array* listing);


#endif

