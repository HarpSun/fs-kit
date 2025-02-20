/*
  This file contains the code that will call the initialization routine
  for a file system (which in turn will initialize the file system). It
  also has to do a few other housekeeping chores to make sure that the
  file system is unmounted properly and all that.


  THIS CODE COPYRIGHT DOMINIC GIAMPAOLO.  NO WARRANTY IS EXPRESSED
  OR IMPLIED.  YOU MAY USE THIS CODE AND FREELY DISTRIBUTE IT FOR
  NON-COMMERCIAL USE AS LONG AS THIS NOTICE REMAINS ATTACHED.

  FOR COMMERCIAL USE, CONTACT DOMINIC GIAMPAOLO (dbg@be.com).

  Dominic Giampaolo
  dbg@be.com
*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>

#include "myfs.h"
#include "kprotos.h"


static int
get_value(char *str)
{
    char buff[128];

    printf("%s: ", str); fflush(stdout);
    fgets(buff, sizeof(buff), stdin);

    return strtol(buff, NULL, 0);
}


int
main(int argc, char **argv)
{
    // 初始化块大小，循环变量 i，竟然在这里初始化 i。。。这就是老程序员的写法吗
    int        block_size = 1024, i;
    // 磁盘名
    char      *disk_name = "big_file";
    // 卷名
    char      *volume_name = "untitled";
    // fs 相关信息，想不用管详情
    myfs_info  *myfs;

    // 参数处理，可以忽略，用默认的就好
    // i 是上面初始化的。。。
    for (i=1; i < argc; i++) {
        if (isdigit(argv[i][0])) {
            block_size = strtoul(argv[i], NULL, 0);
        } else if (disk_name == NULL) {
            disk_name = argv[i];
        } else {
            volume_name = argv[i];
        }
    }

    // 错误处理，忽略
    if (disk_name == NULL) {
        fprintf(stderr, "makefs error: you must specify a file name that\n");
        fprintf(stderr, "              will contain the file systemn");
        exit(5);
    }

    // 磁盘缓存，不知道有没有必要了解，暂且忽略
    init_block_cache(256, 0);

    // 初始化文件系统
    myfs = myfs_create_fs(disk_name, volume_name, block_size, NULL);
    if (myfs != NULL)
        printf("MYFS w/%d byte blocks successfully created on %s as %s\n",
               block_size, disk_name, volume_name);
    else {
        printf("!HOLA! FAILED to create a MYFS file system on %s\n", disk_name);
        exit(5);
    }

    myfs_unmount(myfs);

    shutdown_block_cache();

    return 0;
}
