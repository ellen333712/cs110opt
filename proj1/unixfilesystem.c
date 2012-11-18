#include <stdio.h>
#include <stdlib.h>
#include "unixfilesystem.h"
#include "diskimg.h" 

/*
 * Allocate and initialize a struct unixfilesystem give a open disk image. 
 * Return NULL on error. 
 */
struct unixfilesystem *
unixfilesystem_init(int dfd) 
{			

  /* validate the bootblock. Hopefully this will catch passing 
   * something other than a diskimg to us. 
   */  
  uint16_t bootblock[256];
  if (diskimg_readsector(dfd, BOOTBLOCK_SECTOR, bootblock) != DISKIMG_SECTOR_SIZE) {
    fprintf(stderr, "Error reading bootblock\n");
    return NULL;
  }
  if (bootblock[0] != BOOTBLOCK_MAGIC_NUM) {
    fprintf(stderr, "Bad magic number on disk(0x%x)\n", bootblock[0]);
    return NULL;
  }

  if (sizeof(struct filsys) != DISKIMG_SECTOR_SIZE) { 
    fprintf(stderr, "Warning: Superblock structure size (%zu) != SECTOR_SIZE\n",
            sizeof(struct filsys));
  }

  struct unixfilesystem *fs = malloc(sizeof(struct unixfilesystem));
  if (fs == NULL) {
    fprintf(stderr,"Out of memory.\n");
    return NULL;
  }
  fs->dfd = dfd;
  
  if (diskimg_readsector(dfd, SUPERBLOCK_SECTOR, &(fs->superblock)) != DISKIMG_SECTOR_SIZE) {
    fprintf(stderr, "Error reading superblock\n");
    free(fs);
    return NULL;
  }
  return fs;
}
