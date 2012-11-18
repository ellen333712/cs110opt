
#include "pathname.h"
#include "directory.h"
#include "inode.h"
#include "diskimg.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>



/*
 * Return the inumber associated with the specified pathname. This need only
 * handle absolute paths. Return a negative number if an error is encountered.
 */
int
pathname_lookup(struct unixfilesystem *fs, const char *pathname)
{
 if(pathname[0] != '/') 
  {
	 fprintf(stderr, "%s is not a valid absolute path\n", pathname);
	return -1;
  }
 else return path_to_inode_number(fs, pathname + 1, ROOT_INUMBER);
}

/*
 * Recursive function that moves through the directory inodes to find the
 * inumber of the desired file.
 */
int path_to_inode_number(struct unixfilesystem *fs, const char *pathname, int dir)
{
  if(pathname[0] == 0) return 1;
  else if  (is_filename(pathname)) 
  {
return name_to_inode_number(fs, pathname, dir);
  }
  else
  {
	char directoryName[MAX_NAME];
	const char* newPath = get_directory_name(pathname, directoryName);
	dir = name_to_inode_number (fs, directoryName, dir);

	return path_to_inode_number(fs, newPath, dir);
  }
}


/*
 * Wrapper function to call directory_findname to get the inumber for the
 * given directory name.
 */
int name_to_inode_number(struct unixfilesystem *fs, const char *name, int dir)
{
  struct direntv6 file;
  int check = directory_findname(fs, name, dir, &file); 

  if(check == 0) return file.d_inumber;
  else return -1;
}

/*
 * Checks to see whether the pathname is just a file or a series of directories
 * as well. Searches for '/' which indicates still path, not file.
 */
int is_filename(const char *name)
{
  int check = 1;
  int index = 0;
  while(name[index] != '\0' && name[index] != '/' && index < MAX_NAME)
  {
	index++;
	if(name[index] == '/') check = 0;
  }
  return check;
}

/*
 * Updates the directory name at the beginning of the pathname and puts it 
 * in char *dir. Also returns pointer to new pathname without the beginning
 * directory.
 */
const char*  get_directory_name(const char *path, char dir[])
{
  int index = 0;
  while (path[index] != '/') 
  {
	dir[index] = path[index];
	index++;
  }
  //Makes it so that the string ends after the directory name.
  dir[index]='\0';
  return  path + index + 1;
}
