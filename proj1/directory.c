
#include "directory.h"
#include "inode.h"
#include "diskimg.h"
#include "file.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>


/*
 * Lookup the specified name (name) in the directory (dirinumber). If found, return the 
 * directory entry in dirEnt. Return 0 on success and something negative on failure. 
 */
int
directory_findname(struct unixfilesystem *fs, const char *name,
                   int dirinumber, struct direntv6 *dirEnt)
{
  struct inode directory;
  if (inode_iget(fs, dirinumber, &directory)) return -1;
  int mode = directory.i_mode;
  //Checks that directory is indeed a directory. 
  if ((mode & IFMT) != IFDIR) return -1;
  int dirSize = inode_getsize(&directory);
  int dirBlocks = dirSize / DISKIMG_SECTOR_SIZE;
  int check = 0;
  for (int i=0; i <= dirBlocks; i++)
  {
        struct direntv6 filenames[DIRENTV6_IN_SECTOR];
	int validBytes = file_getblock (fs, dirinumber, i, filenames);
	int validFiles = validBytes / sizeof(struct direntv6);
	check = search_for_filename(filenames, validFiles, name, dirEnt);
	if(check) break;
  }
  return check-1;
}

/*
 * Searches within the array of direntv6 to see if any of them correlate
 * with the desired file.
 */

int  search_for_filename(struct direntv6 filenames[], int validFiles, const char *name, struct direntv6 *dirEnt)
{
  int check = 0;
  for (int j = 0; j < validFiles; j++)
  {
	if (compare_two_strings(name, filenames[j].d_name)) 
	{	
		dirEnt->d_inumber = filenames[j].d_inumber;
		copy_two_strings (dirEnt->d_name, filenames[j].d_name);
		check = 1;
	}
	if(check) break;
  }
  return check;
}


/*
 * Compares two file names, returns 1 if they are the same, 0 otherwise.
 */

int
compare_two_strings(const char *str1, char *str2) 
{
  int check = 1;
  int index = 0;
  while(str1[index] != '\0')
  {
	if(str1[index] != str2[index]) check = 0;
	index++;
  }
  return check;
}

/*
 * Copies from_string to to_string
 */
void copy_two_strings (char to_string[], char from_string[])
{
  for (int i = 0; i < FILENAME_LENGTH; i++)
  {
	to_string[i] = from_string[i];
  }
}


