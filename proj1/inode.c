#include <stdio.h>
#include <assert.h>

#include "inode.h"
#include "diskimg.h"


/*
 * Fetch the specified inode from the filesystem. 
 * Return 0 on success, -1 on error.  
 */
int
inode_iget(struct unixfilesystem *fs, int inumber, struct inode *inp)
{
  int index = inumber - 1;
  struct filsys sector[1];
  int numOfInodesToSector = DISKIMG_SECTOR_SIZE / sizeof(struct inode);
  int sectorNumber = INODE_START_SECTOR + index/numOfInodesToSector;
  int inodeInSector = index % numOfInodesToSector;
  int check = diskimg_readsector(fs->dfd, sectorNumber, sector);
  if (check == -1) return -1;
  struct inode *inode = (struct inode*)sector + inodeInSector;  
  copy_inodes(inp, inode);
  int size = inode_getsize(inp);
  return 0;
}

/*
 * Get the location of the specified file block of the specified inode.
 * Return the disk block number on success, -1 on error.  
 */
int
inode_indexlookup(struct unixfilesystem *fs, struct inode *inp, int blockNum)
{
  int mode = inp->i_mode;
  int diskSector;
  if((mode & ILARG) == 0) 
  {
	diskSector = get_direct_block (fs, inp, blockNum);
  }
  else diskSector = get_indirect_block(fs, inp, blockNum);
  return diskSector;
}

/* 
 * Compute the size of an inode from its size0 and size1 fields.
 */
int
inode_getsize(struct inode *inp) 
{
  return ((inp->i_size0 << 16) | inp->i_size1); 
}

/*
 * Copies the information stored in the fromInode  pointer to the 
 * memory pointed to by the toInode pointer.
 */
void copy_inodes(struct inode *toInode, struct inode *fromInode)
{
  toInode->i_mode = fromInode->i_mode;
  toInode->i_nlink = fromInode->i_nlink;
  toInode->i_uid = fromInode->i_uid;
  toInode->i_gid = fromInode->i_gid;
  toInode->i_size0 = fromInode->i_size0;
  toInode->i_size1 = fromInode->i_size1;
  toInode->i_addr[0] = fromInode->i_addr[0];
  toInode->i_addr[1] = fromInode->i_addr[1];
  toInode->i_addr[2] = fromInode->i_addr[2];
  toInode->i_addr[3] = fromInode->i_addr[3];
  toInode->i_addr[4] = fromInode->i_addr[4];
  toInode->i_addr[5] = fromInode->i_addr[5];
  toInode->i_addr[6] = fromInode->i_addr[6];
  toInode->i_addr[7] = fromInode->i_addr[7];
  toInode->i_atime[0] = fromInode->i_atime[0];
  toInode->i_atime[1] = fromInode->i_atime[1];
  toInode->i_mtime[0] = fromInode->i_mtime[0];
  toInode->i_mtime[1] = fromInode->i_mtime[1];
}

/* 
 * Returns the blockNum entry in the inp inode addr array. This value
 * refers to a sector block number.
 */
int get_direct_block (struct unixfilesystem *fs, struct inode *inp, int blockNum)
{
  return inp->i_addr[blockNum];
}

/*
 * Returns the blockNum entry in the inp addr array. This value
 * refers to a sector block number. Entries 0-6 are sector numbers
 * for indirect blocks and entry 7 is the sector number for a 
 * doubly indirect block.
 */
int get_indirect_block (struct unixfilesystem *fs, struct inode *inp, int blockNum)
{
  int numberOfBlocksToSector = DISKIMG_SECTOR_SIZE / sizeof(uint16_t);
  int indirectBlock = blockNum / numberOfBlocksToSector;
  int directBlock = blockNum % numberOfBlocksToSector;
  if (indirectBlock < DOUBLY_INDIRECT_BLOCK) 
  {
	uint16_t direct[POINTERS_IN_SECTOR];
	diskimg_readsector(fs->dfd, inp->i_addr[indirectBlock], direct);
	return direct[directBlock];
  }
  else 
  {
	int blocksLeft = blockNum - DOUBLY_INDIRECT_BLOCK*numberOfBlocksToSector;
        int indirectBlock2 = blocksLeft / numberOfBlocksToSector;
	int directBlock = blocksLeft  % numberOfBlocksToSector;
	uint16_t indirect[POINTERS_IN_SECTOR]; 
	diskimg_readsector(fs->dfd, inp->i_addr[DOUBLY_INDIRECT_BLOCK], indirect);
	uint16_t direct[POINTERS_IN_SECTOR];
	diskimg_readsector(fs->dfd, indirect[indirectBlock2], direct);
	return direct[directBlock];
  }
}
