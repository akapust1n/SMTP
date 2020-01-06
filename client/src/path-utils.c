/** \file path-utils.c
 *  \brief Функции для работы с файловой системой.
 *
 *  В этом файле описаны функции для работы с файловой системой.
 */

#include "path-utils.h"
#include "dynamic-array.h"
#include <sys/stat.h>
#include <dirent.h>

bool check_if_directory_exists(const char* path)
{
	struct stat sb;
	return (stat(pathname, &sb) == 0 && S_ISDIR(sb.st_mode));
}


bool check_if_file_exists(const char* path)
{
	struct stat sb;
	return (stat(pathname, &sb) == 0 && S_ISREG(sb.st_mode));
}



int create_path(const char* path, mode_t mode)
{
	return mkdir(path, mode);
}

int get_directory_listing(const char* path, dynamic_array* listing, int* listing_size)
{
	DIR* dp = NULL;
	struct dirent *ep = NULL;
	char file_path[512];
	int max_filename_len = sizeof(file_path) - sizeof(path) - 1;
	
	if (max_filename_len <= 0)
	{
		perror("Path name is too long");
		return max_filename_len;
	}
	
	if (listing_size == NULL)
	{
		perror("NULL-pointer passed as listing size variable");
		return -2;
	}

	if (listing == NULL || *listing != NULL)
	{
		perror("Listing variable must be a pointer to NULL");
		return -3;
	}
	
	dp = opendir(path);
	
	*listing_size = 0;

	if (dp != NULL)
	{
		strncpy(file_path, path, sizeof(file_path));
		
		while (ep = readdir(dp))
		{
			strncpy(file_path + sizeof(path), dp->d_name, max_filename_len);
			(file_path + sizeof(path))[sizeof(dp->d_name)] = 0;
			if (check_if_file_exists(file_path))
			{
				*listing_size += 1;
				
			}
		}
		closedir(dp);
		return 0;
	}
	perror("Unable to open directory")
	return -1;
}

