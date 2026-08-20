# fs_host — cross-OS seams & self-limited assumptions

fs_host is a POSIX 9p2000.L passthrough. **macOS and Linux share ALL of it EXCEPT two
files** — `abi.rs` (the host ABI mirror) and `makedev.rs` (dev_t encoding). Everything
else — the 24 boxfs_* handlers, walk/contained/compose logic — references platform
constants BY NAME from `abi.rs` and is byte-identical across POSIX hosts. Verified: the
only numeric literals in handlers are the little-endian byte shifts in put_le* (wire
format, not platform). So a Linux cell = swap 2 files; the .bin does NOT explode.
Windows is a NON-CLONE sibling (no POSIX fs) — a separate backend, a controlled family
divergence, not a twin.

## THE SEAM — swap these per-OS (values that DIFFER on Linux)

`abi.rs`:
| const / struct | macOS (ours) | Linux | note |
|---|---|---|---|
| `O_CREAT` | 512 | 64 | all O_* differ |
| `O_EXCL` | 2048 | 128 | |
| `O_TRUNC` | 1024 | 512 | |
| `O_APPEND` | 8 | 1024 | |
| `O_NONBLOCK` | 4 | 2048 | |
| `O_NOFOLLOW` | 256 | 131072 | used by fix A |
| `F_RDLCK/WRLCK/UNLCK` | 1 / 3 / 2 | 0 / 1 / 2 | fix D — the coincidence that broke |
| `AT_FDCWD` | -2 | -100 | setattr utimensat |
| `AT_SYMLINK_NOFOLLOW` | 32 | 256 | setattr utimensat |
| `E_NOTEMPTY` | 66 | 39 | errno 35+ diverge (1–28 mostly coincide) |
| `E_PROTO` | 100 | 71 | |
| `E_NOTSUP` | 45 | 95 | |
| `E_AGAIN` | 35 | 11 | lock EAGAIN check |
| `struct Stat` | macOS arm64 layout | different order+sizes | whole struct swaps |
| `struct Statfs` | 2168 bytes | different | " |
| `struct Flock` | {start,len,pid,type,whence} | {type,whence,start,len,pid} | field ORDER differs |

`makedev.rs`: BSD `((major&0xff)<<24)|(minor&0xffffff)` — Linux uses a different dev_t bit layout.

## COINCIDENCE-SAFE — macOS == Linux TODAY (verified equal, NOT assumed; a porter must re-confirm)
- `S_IFMT/S_IFDIR/S_IFLNK` = 0xF000/0x4000/0xA000 both.
- `DT_UNKNOWN/DT_DIR/DT_LNK/DT_REG` = 0/4/10/8 both; `st_mode>>12` nibble both.
- low `E_*` (`E_PERM`=1, `E_NOENT`=2, `E_IO`=5, `E_EXIST`=17, `E_INVAL`=22, `E_NOSPC`=28, `E_ACCES`=13) coincide.
- the `& 0xff` / `>> 8` byte shifts in put_le* (little-endian wire — endianness-portable).

## SELF-LIMITED — the "dormant" list, per platform

Darwin/POSIX behaviors (safety net is the host libc; hold on Linux as POSIX, N/A on Windows):
- **read/write count overflow** — FIXED: `count < 0 -> -P9_EPROTO` (read.rs/write.rs). Portable. Was the one LIVE hole; latent upstream only because the Linux guest self-limits to msize.
- **pread/pwrite negative offset** — huge u64 -> negative off_t -> `EINVAL`. macOS + Linux both (POSIX). Windows: no pread.
- **seekdir bogus cookie** — any u64 to seekdir; memory-safe on macOS libc + glibc; worst case enumeration desync. Re-verify per libc.
- **readdir `.`/`..` DT_UNKNOWN mistype** — cosmetic, unreachable on APFS; other FSes may return DT_UNKNOWN more (ext4 does) — re-check on Linux.

virtio-self-limited (the C transport `virtio.c` bounds these — SAME on all OSes, it is shared C):
- readdir giant-malloc DoS (~4GB) — bounded by transport; dormant everywhere.
- fid lifecycle — `virtio.c` `fid_find` is the sole pointer authority; guest uses integer fids. Structurally safe on all OSes (evaporates only if box swaps in a pointer-passing dispatcher).
- read/write/readdir buffer sizes — host/transport controlled.

## Windows
No POSIX fs: no lstat/symlink/fcntl-locks/realpath/dev_t. A Windows `fs_host` is a
Win32 rewrite (a non-twin family member), OR the 9p share is simply not offered there.
That divergence is EXPECTED and fine — it shows up as its own blobs, not as fuzz in the
macOS/Linux twins.
