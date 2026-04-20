#pragma once
#include <stddef.h>
#include <stdint.h>

#define VFS_MAX_FILES   32
#define VFS_NAME_MAX    32
#define VFS_FD_MAX      16

typedef struct {
    char     name[VFS_NAME_MAX];
    char    *data;
    size_t   size;
    size_t   capacity;
    int      used;
} vfs_inode_t;

typedef struct {
    vfs_inode_t *inode;
    size_t       offset;
    int          used;
} vfs_fd_t;

/** @brief Initialises the in-memory filesystem. */
void vfs_init(void);

/**
 * @brief Opens (or creates) a file by name.
 * @return File descriptor >= 0, or -1 on error.
 */
int vfs_open(const char *name);

/** @brief Reads up to @p len bytes from fd into @p buf. Returns bytes read. */
int vfs_read(int fd, char *buf, size_t len);

/** @brief Writes @p len bytes from @p buf to fd. Returns bytes written. */
int vfs_write(int fd, const char *buf, size_t len);

/** @brief Closes a file descriptor. */
void vfs_close(int fd);

/** @brief Calls @p cb(name) for every existing file. */
void vfs_list(void (*cb)(const char *name, size_t size));
