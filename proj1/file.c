#include <stdio.h>
#include <assert.h>

#include "file.h"
#include "inode.h"
#include "diskimg.h"

/*
 * Fetch the specified file block from the specified inode.
 * Return the number of valid bytes in the block, -1 on error.
 */
int
file_getblock(struct unixfilesystem *fs, int inumber, int blockNum, void *buf)
{
  struct inode inode;
  inode_iget (fs, inumber, &inode);
  int sectorNumber = inode_indexlookup (fs, &inode, blockNum);
  diskimg_readsector(fs->dfd, sectorNumber, buf);
  int validBytes;
  int bytesLeft = inode_getsize(&inode) - DISKIMG_SECTOR_SIZE * blockNum;
  if(bytesLeft < DISKIMG_SECTOR_SIZE) validBytes = inode_getsize(&inode) % DISKIMG_SECTOR_SIZE;
  else validBytes = DISKIMG_SECTOR_SIZE;
  return validBytes;  
}
