#include <stddef.h>
#include <stdint.h>
#include "include/vfs.h"
#include "include/heap.h"
#include "include/mem.h"
#include "include/kprintf.h"

#define INITIAL_CAP 64

static vfs_inode_t inodes[VFS_MAX_FILES];
static vfs_fd_t    fds[VFS_FD_MAX];

void vfs_init(void) {
    kmemset(inodes, 0, sizeof(inodes));
    kmemset(fds,    0, sizeof(fds));
    kprintf("VFS      ok\n");
}

static vfs_inode_t *inode_find(const char *name) {
    for (int i = 0; i < VFS_MAX_FILES; i++)
        if (inodes[i].used && kstrlen(inodes[i].name) == kstrlen(name) &&
            __builtin_memcmp(inodes[i].name, name, kstrlen(name)) == 0)
            return &inodes[i];
    return NULL;
}

static vfs_inode_t *inode_alloc(const char *name) {
    for (int i = 0; i < VFS_MAX_FILES; i++) {
        if (!inodes[i].used) {
            kmemset(&inodes[i], 0, sizeof(vfs_inode_t));
            size_t nlen = kstrlen(name);
            if (nlen >= VFS_NAME_MAX) nlen = VFS_NAME_MAX - 1;
            kmemcpy(inodes[i].name, name, nlen);
            inodes[i].name[nlen] = '\0';
            inodes[i].data     = kmalloc(INITIAL_CAP);
            inodes[i].capacity = inodes[i].data ? INITIAL_CAP : 0;
            inodes[i].size     = 0;
            inodes[i].used     = 1;
            return &inodes[i];
        }
    }
    return NULL;
}

int vfs_open(const char *name) {
    vfs_inode_t *inode = inode_find(name);
    if (!inode) inode = inode_alloc(name);
    if (!inode) return -1;

    for (int i = 0; i < VFS_FD_MAX; i++) {
        if (!fds[i].used) {
            fds[i].inode  = inode;
            fds[i].offset = 0;
            fds[i].used   = 1;
            return i;
        }
    }
    return -1;
}

int vfs_read(int fd, char *buf, size_t len) {
    if (fd < 0 || fd >= VFS_FD_MAX || !fds[fd].used) return -1;
    vfs_inode_t *inode = fds[fd].inode;
    size_t avail = inode->size - fds[fd].offset;
    if (avail == 0) return 0;
    if (len > avail) len = avail;
    kmemcpy(buf, inode->data + fds[fd].offset, len);
    fds[fd].offset += len;
    return (int)len;
}

int vfs_write(int fd, const char *buf, size_t len) {
    if (fd < 0 || fd >= VFS_FD_MAX || !fds[fd].used) return -1;
    vfs_inode_t *inode = fds[fd].inode;
    size_t needed = fds[fd].offset + len;
    if (needed > inode->capacity) {
        size_t newcap = inode->capacity * 2;
        if (newcap < needed) newcap = needed;
        char *newdata = kmalloc(newcap);
        if (!newdata) return -1;
        if (inode->data) {
            kmemcpy(newdata, inode->data, inode->size);
            kfree(inode->data);
        }
        inode->data     = newdata;
        inode->capacity = newcap;
    }
    kmemcpy(inode->data + fds[fd].offset, buf, len);
    fds[fd].offset += len;
    if (fds[fd].offset > inode->size)
        inode->size = fds[fd].offset;
    return (int)len;
}

void vfs_close(int fd) {
    if (fd >= 0 && fd < VFS_FD_MAX)
        fds[fd].used = 0;
}

void vfs_list(void (*cb)(const char *name, size_t size)) {
    for (int i = 0; i < VFS_MAX_FILES; i++)
        if (inodes[i].used)
            cb(inodes[i].name, inodes[i].size);
}
