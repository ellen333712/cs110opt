#ifndef _FILEOPS_H
#define _FILEOPS_H

#include <stdio.h>

void *Fileops_init(char *diskpath);
int Fileops_open(char *pathname);
int Fileops_read(int fd, char *buffer, int length);
int Fileops_getinumber(int fd, int *size);
int Fileops_getblockno(int fd);
int Fileops_getblock(int inum, int blockNo, unsigned char *buf);
int Fileops_getchar(int fd, int inum, int size, unsigned char *buf, int *bm);
int Fileops_tell(int fd);
int Fileops_close(int fd);
int Fileops_isfile(char *pathname);

void Fileops_dumpstats(FILE *file);

#endif /* _FILEOPS_H */
