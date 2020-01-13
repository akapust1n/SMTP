/** \file path-utils.c
 *  \brief Функции для работы с файловой системой.
 *
 *  В этом файле описаны функции для работы с файловой системой.
 */

#include "path-utils.h"

bool check_if_directory_exists(const char* path)
{
	struct stat sb;
	return (stat(path, &sb) == 0 && S_ISDIR(sb.st_mode));
}


bool check_if_file_exists(const char* path)
{
	struct stat sb;
	return (stat(path, &sb) == 0 && S_ISREG(sb.st_mode));
}



int create_path(const char* path, mode_t mode)
{
	return mkdir(path, mode);
}

int create_subdirectory(const char* path, const char* directory, mode_t mode)
{
	char full_path[0x200];
	strncpy(full_path, path, 0x200);
	strcat(full_path, directory);
	return create_path(full_path, mode);
}

bool get_directory_listing(const char *path, struct linked_list *listing)
{
	DIR* dp = NULL;
	struct dirent *ep = NULL;
	char file_path[128];
	char *buffer_ptr = NULL;
	int max_filename_len = sizeof(file_path) - sizeof(path) - 1;
	
	if (max_filename_len <= 0)
	{
		perror("Path name is too long");
		return false;
	}

	if (listing == NULL)
	{
		perror("Listing variable must be a pointer to NULL");
		return false;
	}
	
	dp = opendir(path);
	
	listing = linked_list_create();

	if (dp != NULL)
	{
		strncpy(file_path, path, sizeof(file_path));
		
		while ((ep = readdir(dp)) != NULL)
		{
			strncpy(file_path + sizeof(path), ep->d_name, max_filename_len);
			(file_path + sizeof(path))[sizeof(ep->d_name)] = 0;
			if (check_if_file_exists(file_path))
			{
				linked_list_push(listing, file_path, strlen(file_path));
			}
		}
		closedir(dp);
		return true;
	}
	perror("Unable to open directory");
	return false;
}

int free_listing(struct linked_list* listing)
{
	linked_list_free(listing);
}
