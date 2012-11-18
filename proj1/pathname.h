#ifndef _PATHNAME_H
#define _PATHNAME_H

#include "unixfilesystem.h"

#define MAX_NAME 14

int pathname_lookup(struct unixfilesystem *fs, const char *pathname);
int path_to_inode_number(struct unixfilesystem *fs, const char *pathname, int dir);
int name_to_inode_number(struct unixfilesystem *fs, const char *name, int dir);
int is_filename(const char *name);
const char* get_directory_name(const char *path, char dir[]);

#endif /* pathname.h */
