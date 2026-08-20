// Host-directory virtio-9p backend for box. See fs_host.h for the rationale.
//
// Faithful Darwin/POSIX port of tinyemu fs_disk.c @ _pm_/tinyemu/2019-12-21
// (MIT, © 2016 Fabrice Bellard). Logic is unchanged; only three Linux-isms are
// platform-shimmed, each with a real Darwin equivalent (no faked headers):
//   1. fs_statfs  : <sys/statfs.h>    -> <sys/mount.h>   (same struct statfs fields)
//   2. fs_mknod   : <sys/sysmacros.h> -> <sys/types.h>   (makedev/major/minor)
//   3. fs_stat    : st_atim.tv_sec    -> st_atimespec.tv_sec
#include "fs_host.h"

#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>

#if defined(__APPLE__)
#include <sys/param.h>
#include <sys/mount.h>   // struct statfs + statfs()
#include <sys/types.h>   // makedev/major/minor
#define ST_ATIM(st) (st).st_atimespec
#define ST_MTIM(st) (st).st_mtimespec
#define ST_CTIM(st) (st).st_ctimespec
#else
#include <sys/statfs.h>
#include <sys/sysmacros.h>
#define ST_ATIM(st) (st).st_atim
#define ST_MTIM(st) (st).st_mtim
#define ST_CTIM(st) (st).st_ctim
#endif

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

// box's FSFile: the global ::FSFile that fs.h forward-declares (opaque to fs.c).
struct FSFile {
    uint32_t uid;
    char*    path;       // complete host path
    BOOL     is_opened;
    BOOL     is_dir;
    union {
        int  fd;
        DIR* dirp;
    } u;
};

// The vtable callbacks carry C language linkage so their pointer types match
// the extern "C" FSDevice members under -Werror.
extern "C" {

typedef struct {
    FSDevice common;
    char*    root_path;
} FSDeviceHost;

static void boxfs_close(FSDevice* fs, FSFile* f);

static void boxfs_delete(FSDevice* fs, FSFile* f) {
    if (f->is_opened)
        boxfs_close(fs, f);
    free(f->path);
    free(f);
}

// warning: path is owned by the returned FSFile
static FSFile* fid_create(FSDevice*, char* path, uint32_t uid) {
    FSFile* f = (FSFile*)mallocz(sizeof(*f));
    f->path = path;
    f->uid  = uid;
    return f;
}

static int errno_table[][2] = {
    { P9_EPERM, EPERM },     { P9_ENOENT, ENOENT },   { P9_EIO, EIO },
    { P9_EEXIST, EEXIST },   { P9_EINVAL, EINVAL },   { P9_ENOSPC, ENOSPC },
    { P9_ENOTEMPTY, ENOTEMPTY }, { P9_EPROTO, EPROTO }, { P9_ENOTSUP, ENOTSUP },
};

static int errno_to_p9(int err) {
    if (err == 0)
        return 0;
    for (int i = 0; i < int(countof(errno_table)); i++)
        if (err == errno_table[i][1])
            return errno_table[i][0];
    return P9_EINVAL;
}

static int open_flags[][2] = {
    { P9_O_CREAT, O_CREAT },     { P9_O_EXCL, O_EXCL },
    { P9_O_TRUNC, O_TRUNC },     { P9_O_APPEND, O_APPEND },
    { P9_O_NONBLOCK, O_NONBLOCK }, { P9_O_DSYNC, O_DSYNC },
    { P9_O_NOFOLLOW, O_NOFOLLOW }, { P9_O_SYNC, O_SYNC },
};

static int p9_flags_to_host(int flags) {
    int ret = (flags & P9_O_NOACCESS);
    for (int i = 0; i < int(countof(open_flags)); i++)
        if (flags & open_flags[i][0])
            ret |= open_flags[i][1];
    return ret;
}

static void stat_to_qid(FSQID* qid, const struct stat* st) {
    if (S_ISDIR(st->st_mode))
        qid->type = P9_QTDIR;
    else if (S_ISLNK(st->st_mode))
        qid->type = P9_QTSYMLINK;
    else
        qid->type = P9_QTFILE;
    qid->version = 0;          // no caching on client
    qid->path    = st->st_ino;
}

static void boxfs_statfs(FSDevice* fs1, FSStatFS* st) {
    FSDeviceHost* fs = (FSDeviceHost*)fs1;
    struct statfs st1;
    statfs(fs->root_path, &st1);
    st->f_bsize  = st1.f_bsize;
    st->f_blocks = st1.f_blocks;
    st->f_bfree  = st1.f_bfree;
    st->f_bavail = st1.f_bavail;
    st->f_files  = st1.f_files;
    st->f_ffree  = st1.f_ffree;
}

static char* compose_path(const char* path, const char* name) {
    int   path_len = strlen(path);
    int   name_len = strlen(name);
    char* d = (char*)malloc(path_len + 1 + name_len + 1);
    memcpy(d, path, path_len);
    d[path_len] = '/';
    memcpy(d + path_len + 1, name, name_len + 1);
    return d;
}

static int boxfs_attach(FSDevice* fs1, FSFile** pf, FSQID* qid, uint32_t uid,
                        const char*, const char*) {
    FSDeviceHost* fs = (FSDeviceHost*)fs1;
    struct stat st;
    if (lstat(fs->root_path, &st) != 0) {
        *pf = NULL;
        return -errno_to_p9(errno);
    }
    *pf = fid_create(fs1, strdup(fs->root_path), uid);
    stat_to_qid(qid, &st);
    return 0;
}

static int boxfs_walk(FSDevice* fs, FSFile** pf, FSQID* qids,
                      FSFile* f, int n, char** names) {
    char*       path = strdup(f->path);
    struct stat st;
    int         i;
    for (i = 0; i < n; i++) {
        char* path1 = compose_path(path, names[i]);
        if (lstat(path1, &st) != 0) {
            free(path1);
            break;
        }
        free(path);
        path = path1;
        stat_to_qid(&qids[i], &st);
    }
    *pf = fid_create(fs, path, f->uid);
    return i;
}

static int boxfs_mkdir(FSDevice*, FSQID* qid, FSFile* f,
                       const char* name, uint32_t mode, uint32_t) {
    char*       path = compose_path(f->path, name);
    struct stat st;
    if (mkdir(path, mode) < 0) { free(path); return -errno_to_p9(errno); }
    if (lstat(path, &st) != 0) { free(path); return -errno_to_p9(errno); }
    free(path);
    stat_to_qid(qid, &st);
    return 0;
}

static int boxfs_open(FSDevice* fs, FSQID* qid, FSFile* f, uint32_t flags,
                      FSOpenCompletionFunc*, void*) {
    struct stat st;
    boxfs_close(fs, f);
    if (stat(f->path, &st) != 0)
        return -errno_to_p9(errno);
    stat_to_qid(qid, &st);
    if (flags & P9_O_DIRECTORY) {
        DIR* dirp = opendir(f->path);
        if (!dirp)
            return -errno_to_p9(errno);
        f->is_opened = TRUE;
        f->is_dir    = TRUE;
        f->u.dirp    = dirp;
    } else {
        int fd = open(f->path, p9_flags_to_host(flags) & ~O_CREAT);
        if (fd < 0)
            return -errno_to_p9(errno);
        f->is_opened = TRUE;
        f->is_dir    = FALSE;
        f->u.fd      = fd;
    }
    return 0;
}

static int boxfs_create(FSDevice* fs, FSQID* qid, FSFile* f, const char* name,
                        uint32_t flags, uint32_t mode, uint32_t) {
    struct stat st;
    boxfs_close(fs, f);
    char* path = compose_path(f->path, name);
    int   fd   = open(path, p9_flags_to_host(flags) | O_CREAT, mode);
    if (fd < 0) { free(path); return -errno_to_p9(errno); }
    if (lstat(path, &st) != 0) { free(path); close(fd); return -errno_to_p9(errno); }
    free(f->path);
    f->path      = path;
    f->is_opened = TRUE;
    f->is_dir    = FALSE;
    f->u.fd      = fd;
    stat_to_qid(qid, &st);
    return 0;
}

static int boxfs_readdir(FSDevice*, FSFile* f, uint64_t offset,
                         uint8_t* buf, int count) {
    if (!f->is_opened || !f->is_dir)
        return -P9_EPROTO;
    if (offset == 0)
        rewinddir(f->u.dirp);
    else
        seekdir(f->u.dirp, offset);
    int pos = 0;
    for (;;) {
        struct dirent* de = readdir(f->u.dirp);
        if (de == NULL)
            break;
        int name_len = strlen(de->d_name);
        int len      = 13 + 8 + 1 + 2 + name_len;
        if ((pos + len) > count)
            break;
        offset     = telldir(f->u.dirp);
        int d_type = de->d_type;
        if (d_type == DT_UNKNOWN) {
            char*       path = compose_path(f->path, de->d_name);
            struct stat st;
            d_type = (lstat(path, &st) == 0) ? (st.st_mode >> 12) : DT_REG;
            free(path);
        }
        int type = (d_type == DT_DIR) ? P9_QTDIR
                 : (d_type == DT_LNK) ? P9_QTSYMLINK
                                      : P9_QTFILE;
        buf[pos++] = type;
        put_le32(buf + pos, 0);            pos += 4;   // version
        put_le64(buf + pos, de->d_ino);    pos += 8;
        put_le64(buf + pos, offset);       pos += 8;
        buf[pos++] = d_type;
        put_le16(buf + pos, name_len);     pos += 2;
        memcpy(buf + pos, de->d_name, name_len);
        pos += name_len;
    }
    return pos;
}

static int boxfs_read(FSDevice*, FSFile* f, uint64_t offset,
                      uint8_t* buf, int count) {
    if (!f->is_opened || f->is_dir)
        return -P9_EPROTO;
    int ret = pread(f->u.fd, buf, count, offset);
    return ret < 0 ? -errno_to_p9(errno) : ret;
}

static int boxfs_write(FSDevice*, FSFile* f, uint64_t offset,
                       const uint8_t* buf, int count) {
    if (!f->is_opened || f->is_dir)
        return -P9_EPROTO;
    int ret = pwrite(f->u.fd, buf, count, offset);
    return ret < 0 ? -errno_to_p9(errno) : ret;
}

static void boxfs_close(FSDevice*, FSFile* f) {
    if (!f->is_opened)
        return;
    if (f->is_dir)
        closedir(f->u.dirp);
    else
        close(f->u.fd);
    f->is_opened = FALSE;
}

static int boxfs_stat(FSDevice*, FSFile* f, FSStat* st) {
    struct stat st1;
    if (lstat(f->path, &st1) != 0)
        return -P9_ENOENT;
    stat_to_qid(&st->qid, &st1);
    st->st_mode       = st1.st_mode;
    st->st_uid        = st1.st_uid;
    st->st_gid        = st1.st_gid;
    st->st_nlink      = st1.st_nlink;
    st->st_rdev       = st1.st_rdev;
    st->st_size       = st1.st_size;
    st->st_blksize    = st1.st_blksize;
    st->st_blocks     = st1.st_blocks;
    st->st_atime_sec  = ST_ATIM(st1).tv_sec;
    st->st_atime_nsec = ST_ATIM(st1).tv_nsec;
    st->st_mtime_sec  = ST_MTIM(st1).tv_sec;
    st->st_mtime_nsec = ST_MTIM(st1).tv_nsec;
    st->st_ctime_sec  = ST_CTIM(st1).tv_sec;
    st->st_ctime_nsec = ST_CTIM(st1).tv_nsec;
    return 0;
}

static int boxfs_setattr(FSDevice*, FSFile* f, uint32_t mask,
                         uint32_t mode, uint32_t uid, uint32_t gid,
                         uint64_t size, uint64_t atime_sec, uint64_t atime_nsec,
                         uint64_t mtime_sec, uint64_t mtime_nsec) {
    BOOL ctime_updated = FALSE;
    if (mask & (P9_SETATTR_UID | P9_SETATTR_GID)) {
        if (lchown(f->path, (mask & P9_SETATTR_UID) ? uid : (uid_t)-1,
                   (mask & P9_SETATTR_GID) ? gid : (gid_t)-1) < 0)
            return -errno_to_p9(errno);
        ctime_updated = TRUE;
    }
    if (mask & P9_SETATTR_MODE) {   // after uid change, for suid
        if (chmod(f->path, mode) < 0)
            return -errno_to_p9(errno);
        ctime_updated = TRUE;
    }
    if (mask & P9_SETATTR_SIZE) {
        if (truncate(f->path, size) < 0)
            return -errno_to_p9(errno);
        ctime_updated = TRUE;
    }
    if (mask & (P9_SETATTR_ATIME | P9_SETATTR_MTIME)) {
        struct timespec ts[2];
        if (mask & P9_SETATTR_ATIME) {
            if (mask & P9_SETATTR_ATIME_SET) { ts[0].tv_sec = atime_sec; ts[0].tv_nsec = atime_nsec; }
            else                             { ts[0].tv_sec = 0; ts[0].tv_nsec = UTIME_NOW; }
        } else                               { ts[0].tv_sec = 0; ts[0].tv_nsec = UTIME_OMIT; }
        if (mask & P9_SETATTR_MTIME) {
            if (mask & P9_SETATTR_MTIME_SET) { ts[1].tv_sec = mtime_sec; ts[1].tv_nsec = mtime_nsec; }
            else                             { ts[1].tv_sec = 0; ts[1].tv_nsec = UTIME_NOW; }
        } else                               { ts[1].tv_sec = 0; ts[1].tv_nsec = UTIME_OMIT; }
        if (utimensat(AT_FDCWD, f->path, ts, AT_SYMLINK_NOFOLLOW) < 0)
            return -errno_to_p9(errno);
        ctime_updated = TRUE;
    }
    if ((mask & P9_SETATTR_CTIME) && !ctime_updated) {
        if (lchown(f->path, (uid_t)-1, (gid_t)-1) < 0)
            return -errno_to_p9(errno);
    }
    return 0;
}

static int boxfs_link(FSDevice*, FSFile* df, FSFile* f, const char* name) {
    char* path = compose_path(df->path, name);
    if (link(f->path, path) < 0) { free(path); return -errno_to_p9(errno); }
    free(path);
    return 0;
}

static int boxfs_symlink(FSDevice*, FSQID* qid, FSFile* f,
                         const char* name, const char* symgt, uint32_t) {
    char*       path = compose_path(f->path, name);
    struct stat st;
    if (symlink(symgt, path) < 0) { free(path); return -errno_to_p9(errno); }
    if (lstat(path, &st) != 0)    { free(path); return -errno_to_p9(errno); }
    free(path);
    stat_to_qid(qid, &st);
    return 0;
}

static int boxfs_mknod(FSDevice*, FSQID* qid, FSFile* f, const char* name,
                       uint32_t mode, uint32_t major, uint32_t minor, uint32_t) {
    char*       path = compose_path(f->path, name);
    struct stat st;
    if (mknod(path, mode, makedev(major, minor)) < 0) { free(path); return -errno_to_p9(errno); }
    if (lstat(path, &st) != 0)                        { free(path); return -errno_to_p9(errno); }
    free(path);
    stat_to_qid(qid, &st);
    return 0;
}

static int boxfs_readlink(FSDevice*, char* buf, int buf_size, FSFile* f) {
    int ret = readlink(f->path, buf, buf_size - 1);
    if (ret < 0)
        return -errno_to_p9(errno);
    buf[ret] = '\0';
    return 0;
}

static int boxfs_renameat(FSDevice*, FSFile* f, const char* name,
                          FSFile* new_f, const char* new_name) {
    char* path     = compose_path(f->path, name);
    char* new_path = compose_path(new_f->path, new_name);
    int   ret      = rename(path, new_path);
    free(path);
    free(new_path);
    return ret < 0 ? -errno_to_p9(errno) : 0;
}

static int boxfs_unlinkat(FSDevice*, FSFile* f, const char* name) {
    char* path = compose_path(f->path, name);
    int   ret  = remove(path);
    free(path);
    return ret < 0 ? -errno_to_p9(errno) : 0;
}

static int boxfs_lock(FSDevice*, FSFile* f, const FSLock* lock) {
    if (!f->is_opened || f->is_dir)
        return -P9_EPROTO;
    struct flock fl;
    fl.l_type   = lock->type;
    fl.l_whence = SEEK_SET;
    fl.l_start  = lock->start;
    fl.l_len    = lock->length;
    int ret = fcntl(f->u.fd, F_SETLK, &fl);
    if (ret == 0)
        return P9_LOCK_SUCCESS;
    if (errno == EAGAIN || errno == EACCES)
        return P9_LOCK_BLOCKED;
    return -errno_to_p9(errno);
}

static int boxfs_getlock(FSDevice*, FSFile* f, FSLock* lock) {
    if (!f->is_opened || f->is_dir)
        return -P9_EPROTO;
    struct flock fl;
    fl.l_type   = lock->type;
    fl.l_whence = SEEK_SET;
    fl.l_start  = lock->start;
    fl.l_len    = lock->length;
    int ret = fcntl(f->u.fd, F_GETLK, &fl);
    if (ret < 0)
        return -errno_to_p9(errno);
    lock->type   = fl.l_type;
    lock->start  = fl.l_start;
    lock->length = fl.l_len;
    return ret;
}

static void boxfs_end(FSDevice* fs1) {
    FSDeviceHost* fs = (FSDeviceHost*)fs1;
    free(fs->root_path);
}

}  // extern "C"

namespace box {

FSDevice* make_fs(const std::string& root) {
    struct stat st;
    if (lstat(root.c_str(), &st) != 0 || !S_ISDIR(st.st_mode))
        return nullptr;

    FSDeviceHost* fs = (FSDeviceHost*)mallocz(sizeof(*fs));
    fs->common.fs_end      = boxfs_end;
    fs->common.fs_delete   = boxfs_delete;
    fs->common.fs_statfs   = boxfs_statfs;
    fs->common.fs_attach   = boxfs_attach;
    fs->common.fs_walk     = boxfs_walk;
    fs->common.fs_mkdir    = boxfs_mkdir;
    fs->common.fs_open     = boxfs_open;
    fs->common.fs_create   = boxfs_create;
    fs->common.fs_stat     = boxfs_stat;
    fs->common.fs_setattr  = boxfs_setattr;
    fs->common.fs_close    = boxfs_close;
    fs->common.fs_readdir  = boxfs_readdir;
    fs->common.fs_read     = boxfs_read;
    fs->common.fs_write    = boxfs_write;
    fs->common.fs_link     = boxfs_link;
    fs->common.fs_symlink  = boxfs_symlink;
    fs->common.fs_mknod    = boxfs_mknod;
    fs->common.fs_readlink = boxfs_readlink;
    fs->common.fs_renameat = boxfs_renameat;
    fs->common.fs_unlinkat = boxfs_unlinkat;
    fs->common.fs_lock     = boxfs_lock;
    fs->common.fs_getlock  = boxfs_getlock;
    fs->root_path = strdup(root.c_str());
    return (FSDevice*)fs;
}

void free_fs(FSDevice* fs) {
    if (!fs)
        return;
    fs->fs_end(fs);
    free(fs);
}

}  // namespace box
