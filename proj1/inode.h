#ifndef _INODE_H
#define _INODE_H

#include "unixfilesystem.h"

#define DOUBLY_INDIRECT_BLOCK 7
#define POINTERS_IN_SECTOR 256

int inode_iget(struct unixfilesystem *fs, int inumber, struct inode *inp); 
int inode_indexlookup(struct unixfilesystem *fs, struct inode *inp, int blockNum);
int inode_getsize(struct inode *inp);
int get_direct_block(struct unixfilesystem *fs, struct inode *inp, int blockNum);
int get_indirect_block(struct unixfilesystem *fs, struct inode *inp, int blockNum);
void copy_inodes(struct inode *toInode, struct inode *fromInode);

#endif /* _INODE_ */
