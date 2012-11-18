#ifndef _DIRECTORY_H
#define _DIRECTORY_H

#include "unixfilesystem.h"
#include "diskimg.h"
#include "direntv6.h"

#define FILENAME_LENGTH 14
#define DIRENTV6_IN_SECTOR DISKIMG_SECTOR_SIZE / sizeof(struct direntv6)

int directory_findname(struct unixfilesystem *fs, const char *name,
                       int dirinumber, struct direntv6 *dirEnt);

int search_for_filename(struct direntv6 filenames[], int validFiles, const char *name, struct direntv6 *dirEnt);
int compare_two_strings (const char *str1, char *str2);
void copy_two_strings (char to_string[], char from_string[]);

#endif /* _DIECTORY_H */
