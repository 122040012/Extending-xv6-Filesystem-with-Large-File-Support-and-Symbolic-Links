struct file {
  enum { FD_NONE, FD_PIPE, FD_INODE, FD_DEVICE } type;
  int ref; // reference count
  char readable;
  char writable;
  struct pipe *pipe; // FD_PIPE
  struct inode *ip;  // FD_INODE and FD_DEVICE
  uint off;          // FD_INODE
  short major;       // FD_DEVICE
};

#define major(dev)  ((dev) >> 16 & 0xFFFF)
#define minor(dev)  ((dev) & 0xFFFF)
#define	mkdev(m,n)  ((uint)((m)<<16| (n)))

// TODO: Follow instruction to make change of structure
// in-memory copy of an inode
struct inode {
  uint dev;           // Device number
  uint inum;          // Inode number
  int ref;            // Reference count
  struct sleeplock lock; // Protects everything below here
  int valid;          // Inode has been read from disk?

  short type;         // Copy of disk inode
  short major;
  short minor;
  short nlink;
  uint size;
  uint addrs[NDIRECT + 2];  // 11 direct, 1 singly-indirect, 1 doubly-indirect
  uint tags;               // Tags block pointer (if implementing tags)
  uint tCounter;           // Number of tags allocated (if implementing tags)
  char symlink;            // Flag indicating if inode is a symbolic link
};

// Define min macro for use in multiple files
#define min(a, b) ((a) < (b) ? (a) : (b))

// map major device number to device functions.
struct devsw {
  int (*read)(int, uint64, int);
  int (*write)(int, uint64, int);
};

extern struct devsw devsw[];

#define CONSOLE 1
