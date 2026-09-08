# CS5204 - File System Checker

## Introduction

In this assignment, you will be developing a working file system checker. A checker that reads a file 
system image and verifies its consistency. When it isn't consistent, the checker will output the specific 
problem that causes the inconsistency. You will be developing the filesystem checker tool as part of a real 
but minimal operating system xv6.

There will be three main tasks as part of this assignment:

1) You will be expected to do some reading about the xv6 operating system, filesystem 
implementation, filesystem crash consistency and some more background content. The deliverable of the 
first task is to return a brief paragraph explaining what it means to have an inconsistency in the filesystem, 
on what occasions can this problem occur, how severe can they be and what are some mitigations? Write 
a brief paragraph with your own sentences.

2) You will get the xv6 operating system up and running. You will get the code, configure 
the development environment, install the necessary build tools, compile and run the xv6 operating 
system. At the end of the second task, you will acquire an actual filesystem image produced by xv6 itself. 
Your deliverables for this task is a screenshot showing your terminal with the boot screen of xv6 with your 
username visible.

3) You will be developing the filesystem checker program. This 
program will take a filesystem image file as input and will return 0 silently if the image is consistent. 
However, if there is inconsistency, it will return 1 and print the cause of the inconsistency to the stderr. 
Read the rest of this handout carefully for all the details you will need throughout this assignment.

## Task 1: Background Study [10 Points] 

Xv6 operating system is an educational operating system that was developed at MIT. In this assignment 
you will be developing your filesystem checker solely for xv6. Therefore, it will be necessary for you to 
understand its internals.

Thus, you should take the time to go through and read the below provided materials before you dive into 
the implementation tasks. Keep in mind that the reading list below contains a whole lot of materials. You 
shouldn’t wait to start with the rest of the tasks in this assignment until you finish reading everything. Read 
enough to have an understanding of the fundamentals and how to get going, you can always refer back to 
the materials as you work on the other tasks.

- [OSTEP: Crash Consistency: FSCK and Journaling](https://pages.cs.wisc.edu/~remzi/OSTEP/file-journaling.pdf) [REQUIRED]
- [XV6 Book](https://pdos.csail.mit.edu/6.1810/2024/xv6/book-riscv-rev4.pdf) [STRONGLY RECOMMENDED]
- [OSTEP: Files and Directories](https://pages.cs.wisc.edu/~remzi/OSTEP/file-intro.pdf) [STRONGLY RECOMMENDED]
- [OSTEP: Filesystem Implementation](https://pages.cs.wisc.edu/~remzi/OSTEP/file-implementation.pdf) [STRONGLY RECOMMENDED]
- [The Original FSCK Paper](https://docs-archive.freebsd.org/44doc/smm/03.fsck/paper.pdf) [RECOMMENDED]
- [GCC Cross Compiler Logic](https://wiki.osdev.org/GCC_Cross-Compiler) [RECOMMENDED]
- [OSTEP: Locality and The Fast File System](https://pages.cs.wisc.edu/~remzi/OSTEP/file-ffs.pdf) [OPTIONAL]

**As part of the first task of this assignment, you are required to read the “Crash Consistency: 
FSCK and Journaling" chapter of the book OSTEP (see above) and prepare a brief paragraph of 
what is crash consistency, why, when and how it may happen in your own words. There is no 
minimum word limit, so it can be very brief but it should convey your understanding of crash 
consistency. The paragraph should be in pdf format.**

## Task 2: Compile and Run XV6 [30 Points]

Currently, there are two versions of xv6 available to the public. One of them is xv6-public and the other is 
xv6-riscv. Unfortunately, xv6-public is no longer maintained, so we will be using xv6-riscv for this 
assignment. The xv6-riscv version targets the riscv architecture but don’t worry, there is almost no 
difference for you in this assignment. 

The code for xv6-riscv is provided by us and you should stick to the code we provide as it contains minor 
changes that we added for your convenience.

Once you downloaded the **xv6-riscv.tar.gz** and you are at the directory that contains the file, you can run 
the following command to unpack it’s contents: 

```bash
$ tar xzvf xv6-riscv.tar.gz
$ cd xv6-riscv 
```

Now that you have obtained the code and changed into the directory of xv6, it’s time to install the 
necessary tools and set up the development environment to be able to build and run xv6.

For this assignment, you'll need the RISC-V versions of QEMU 7.2+, GDB 8.3+, GCC, and Binutils.

**For Debian or Ubuntu [PREFERRED]:**
```bash
$ sudo apt-get install git build-essential gdb-multiarch \
    qemu-system-misc gcc-riscv64-linux-gnu binutils-riscv64-linux-gnu 
```

**You will need Ubuntu 24 (or later) in order for your package manager to install QEMU 7.2+. If you 
somehow cannot work on an Ubuntu 24, which you should have no reason not to, read below to 
install QEMU 7.2 for Ubuntu less than 24 as well.**

Install necessary QEMU 7.2 for ubuntu less than 24: 
```bash
$ wget https://download.qemu.org/qemu-7.2.0.tar.xz 
$ tar xvJf qemu-7.2.0.tar.xz 
$ cd qemu-7.2.0 
$ ./configure 
$ sudo apt-get install ninja-build 
$ sudo apt-get install zlib1g zlib1g-dev 
$ sudo apt-get install libglib2.0-dev 
$ sudo apt-get install libpixman-1-dev 
$ make 
$ make sudo install 
$ qemu-riscv64 --version
```
If you succeeded, you should see version 7.2.

**For macOS:**

First, install developer tools: 
```bash
$ xcode-select --install 
```
Next, install [Homebrew](https://brew.sh/), a package manager for macOS:
```bash
$ /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```
Next, install the [RISC-V compiler toolchain](https://github.com/riscv-software-src/homebrew-riscv): 
```bash
$ brew tap riscv/riscv 
$ brew install riscv-tools 
```
The brew formula may not link into /usr/local. You will need to update your shell's rc file (e.g. ~/.bashrc) to 
add the appropriate directory to $PATH.
```bash
PATH=$PATH:/usr/local/opt/riscv-gnu-toolchain/bin
```
Finally, install QEMU:
```bash
$ brew install qemu
```

**Testing your installation:**

To test your installation, you should now be able to compile and run xv6. 
You can also double check if your installation is correct by running the following:
```bash
$ qemu-system-riscv64 --version QEMU emulator version 7.2.0
```
And at least one of below RISC-V version of GCC:
```bash
$ riscv64-linux-gnu-gcc --version
riscv64-linux-gnu-gcc (Debian 10.3.0-8) 10.3.0
```
```bash
$ riscv64-unknown-elf-gcc --version
riscv64-unknown-elf-gcc (GCC) 10.1.0
```
```bash
$ riscv64-unknown-linux-gnu-gcc --version 
riscv64-unknown-linux-gnu-gcc (GCC) 10.1.0 
```
In case you have a problem or a special need to manually install the RISCV GNU Toolchain, here is how 
to do that: 

**RISC-V GNU Compiler Toolchain**

This is the RISC-V C and C++ cross-compiler. It supports two build modes: a generic ELF/Newlib 
toolchain and a more sophisticated Linux-ELF/glibc toolchain. 
```bash
$ git clone https://github.com/riscv/riscv-gnu-toolchain
```
**Warning**: git clone may take around 6.65 GB of disk and download size

Prerequisites Ubuntu: 
```bash
$ sudo apt-get install autoconf automake autotools-dev curl python3 python3-pip \
  python3-tomli libmpc-dev libmpfr-dev libgmp-dev gawk build-essential bison flex \
  texinfo gperf libtool patchutils bc zlib1g-dev libexpat-dev ninja-build git cmake \
  libglib2.0-dev libslirp-dev
```
Prerequisites macOS:, you can use Homebrew to install the dependencies:
```bash
$ brew install python3 gawk gnu-sed make gmp mpfr libmpc isl zlib expat texinfo \
  flock libslirp 
```

**Installation (Newlib)**

To build the Newlib cross-compiler, pick an install path (that is writeable). If you choose, say, /opt/riscv, 
then add **/opt/riscv/bin** to your PATH. 

After you added **/opt/riscv/bin** to your PATH, simply run the following command:
```bash
$ ./configure --prefix=/opt/riscv 
$ make -j`nproc` 
```
This may take some time…

Now we are ready to build xv6.

**Build and Run XV6**

First, we make sure we are inside the xv6-riscv directory.
```bash
$ cd <path-to-xv6-riscv>/xv6-riscv
```

To build and run xv6, run “make qemu” and you should see below:
```bash
$ make qemu
riscv64-unknown-elf-gcc    -c -o kernel/entry.o kernel/entry.S
riscv64-unknown-elf-gcc -Wall -Werror -O -fno-omit-frame-pointer -ggdb -gdwarf-2 -MD -mcmodel=medany -fno-common -nostdlib -fno-builtin-strncpy -fno-builtin-strncmp -fno-builtin-strlen -fno-builtin-memset -fno-builtin-memmove -fno-builtin-memcmp -fno-builtin-log -fno-builtin-bzero -fno-builtin-strchr -fno-builtin-exit -fno-builtin-malloc -fno-builtin-putc -fno-builtin-free -fno-builtin-memcpy -Wno-main -fno-builtin-printf -fno-builtin-fprintf -fno-builtin-vprintf -I. -fno-stack-protector -fno-pie -no-pie   -c -o kernel/start.o kernel/start.c
...
qemu-system-riscv64 -machine virt -bios none -kernel kernel/kernel -m 128M -smp 3 -nographic -global virtio-mmio.force-legacy=false -drive file=fs.img,if=none,format=raw,id=x0 -device virtio-blk-device,drive=x0,bus=virtio-mmio-bus.0

xv6 kernel is booting

hart 1 starting
hart 2 starting
init: starting sh
$ 
```

Now, if you type “ls” at the prompt, you should see output similar to the following:
```bash
$ ls
.              1 1 1024
..             1 1 1024
README         2 2 2292
cat            2 3 34680
echo           2 4 33584
forktest       2 5 15696
grep           2 6 38008
init           2 7 34000
kill           2 8 33568
ln             2 9 33368
ls             2 10 36576
mkdir          2 11 33640
rm             2 12 33624
sh             2 13 55816
stressfs       2 14 34344
usertests      2 15 180232
grind          2 16 49584
wc             2 17 35680
zombie         2 18 33024
console        3 19 0
```

These are the files that **mkfs** includes in the initial file system; most are programs you can run. You just
ran one of them: ls.

xv6 has no ps command, but, if you type **Ctrl-p**, the kernel will print information about each process. If
you try it now, you'll see two lines: one for init, and one for sh.

To quit QEMU, type: **Ctrl-a x** (press Ctrl and a at the same time, followed by x).

Now, if you check the xv6-riscv directory, you will find the **fs.img** filesystem image that is created by
default. You can either play around inside the xv6 to get a different image, which will most certainly be
consistent or you can alter this original **fs.img** to your liking to hack around and test your checker, which
you certainly should do.

**As part of the second task of the assignment, you will take a screenshot of your terminal which
clearly shows the booted xv6 and your username. For example, below is from my terminal.**
<img width="1380" height="692" alt="task2_example" src="https://github.com/user-attachments/assets/8aac497d-e34c-4c34-af26-d61ed739ff61" />


## Task 3: Implement Filesystem Checker [60 Points]

Now that you have a working xv6 and an actual valid filesystem image (**fs.img**), you can begin to develop
your filesystem checker. You will use the xv6 file system image, **fs.img**, as the basic image that you will
be reading and checking against.

The file **kernel/fs.h** includes the basic structures you need to understand, including the **struct**
**superblock**, on-disk inode format **struct dinode**, and directory entry format **struct dirent**. The tool
**mkfs/mkfs.c** will also be useful to look at, in order to see how an empty file-system image is created by
the OS. In addition, other files you may find useful are kernel/param.h, kernel/type.h and kernel/stat.h.

Much of this project will be puzzling out the exact on-disk format xv6 uses for its simple file system, and
then writing checks to see if various parts of that structure are consistent. Thus, reading through
mkfs/mkfs.c and the file system code itself will help you understand how xv6 uses the bits in the
filesystem image to record persistent information.

Your checker should read through the file system image and determine the consistency of a number of
things. The exact list is below. When a problem is detected, print the error message exactly as it’s shown
(shown below) to stderr and exit immediately with exit code 1 (i.e., call exit(1)).

1. [2 Points] - The superblock of the filesystem image must be consistent. That means checking the
fields of the **struct superblock** and verifying the correctness of their values. If the values don’t
match each other or any of the fields has an incorrect value (you need to find out what it means
for values to match each other and have the correct value), print **“ERROR: bad superblock”**.
2. [2 Points] - Among all the inodes, there must be an inode that represents the root directory of the
filesystem. That means checking for the inode at the right index for the root directory (you will
need to find out which index) and making sure it’s marked as a directory type. If not, print
**“ERROR: root directory does not exist”**.
3. [3 Points] - Each inode must be either unallocated **(inode->type == 0)** or one of the valid types
**(T_FILE, T_DIR, T_DEV)**. If not, print **“ERROR: bad inode”**.
4. [5 Points] - For in-use inodes **(non-zero valid inode->type)**, each address that is used by inode
must be valid (points to a valid datablock address within the boundaries of the filesystem image).
If the direct block address is used and is invalid, print **“ERROR: bad direct address in inode”**.
If the indirect block address is in use and is invalid, print **“ERROR: bad indirect address in inode”**.
5. [5 Points] - For in-use inodes, each address in use must be only used once. If a **direct block
address** is used more than once, print **“ERROR: direct address used more than once”**. If an
indirect block address is used more than once, print **“ERROR: indirect address used more
than once”**.
6. [5 Points] - Each directory inode must contain “.” and “..” entries (you will need to find out what
it means for a directory to have such entries and how to verify it), and the “.” entry points to the
inode block address of the directory itself. If not, print **“ERROR: directory not properly
formatted”**.
7. [7 Points] - For each block in the filesystem image, if they are in use, their block address must
also be marked as in use in the bitmap and if they are not in use, they must be marked as not in
use. You should count all the blocks until datablock start, as used. For the datablocks, you will
need to verify if they are used or not and compare them with the bitmap. If they are not in use but
bitmap marks them as used, print **“ERROR: bitmap marks block in use but it is not in use”**. If
they are used but bitmap marks them as not used, print **“ERROR: address used by inode but
marked free in bitmap”**.
8. [7 Points] - Reference counts (number of links) for file inodes must match the number of times a
file is referred to in directories (i.e., links work correctly). You will need to find out where the
filesystem is keeping the number of links and compare that with the actual number of references
to the file from directories. If not, print **“ERROR: bad reference count for file”**.
9. [7 Points] - Each inode that is marked in use must be referred to in at least one directory. That
means checking through directories to find an entry that points to the index of the in use inode. If
not, print **“ERROR: inode marked use but not found in a directory”**.
10. [7 Points] - Each inode that is referred to in a valid directory must be also marked in use. If not,
print **“ERROR: inode referred to in directory but marked free”**.
11. [10 Points] - Each directory must only appear in one other directory only. Note that a directory
can have multiple links through their child directories’ “..” (parent) entries. However, the same
directory cannot appear in another directory’s entries that are not “..”. If not, print **“ERROR:
directory appears more than once in file system”**.

**Other Specifications**

Your checker program, called xcheck, must be invoked exactly as follows:
```bash
$ ./xcheck <file_system_image>
```

If the checker detects any one of the inconsistencies described above, it should print the specific error to
standard error and exit with error code 1 immediately. If the checker detects none of the problems
listed, it should exit with a return code of 0 and not print anything.

**You will deliver the code for your filesystem checker along with xv6. There should be a Makefile
that will compile your program to produce a standalone executable or you can modify the existing
Makefile of xv6 to do that. You must have a README.md file separate for your filesystem checker
describing what it does, how it works and how it is used, including dependency installation,
compilation steps and running it. Turn in the directory that contains all of it as a tarball (.tar.gz).**

**Please use your own work as the (partial) solutions you find online may not be compatible with
the xv6 we provided.**

## Hints

It may be worth looking into using **mmap()** for the project. Like, seriously, use **mmap()** to access the
file-system image, it will make your life so much better.

When working on your filesystem checker, be extra careful about distinguishing **byte-level** and
**block-level** operations from each other.

The log block (journaling) content should be disregarded while checking the consistency of the filesystem
image. You should not disregard the existence of the log block, instead, you should disregard its internal
consistency only. You will still need to account for the log block when sizing up the blocks and everything.
But you do not necessarily need to extract what’s inside the log block and try to check its consistency, you
can safely ignore the parts that pertain to that.

Make sure to look at fs.img, which is a file system image created when you make xv6 by the tool
mkfs/mkfs.c. The output of this tool is the file fs.img and it is a consistent file-system image. The tests,
of course, will put inconsistencies into this image, but your tool should work over a consistent image as
well. Study mkfs/mkfs.c and its output to begin to make progress on this assignment.

While working on your filesystem checker, you will need some information defined inside the xv6 files
such as block size **BSIZE**. Instead of trying to re-define each piece of information in your own standalone
tool, try to integrate the header files into your checker. That way, you can re-use the information defined
by xv6 but you can still build and run your code as a separate executable.

Don’t hesitate to include headers both from the standard library as well as the xv6 such as **kernel/fs.h**
etc. You will find the standard library headers useful for tools like mmap and xv6 headers for already
defined structs.

Even though xv6 uses **riscv64-unknown-elf-gcc** as the compiler for bare-metal purposes, you don’t
need to stick with it. Feel free to compile your checker with something that allows standard library features
like gcc. You will actually need to use **gcc** (or similar) for **mmap**.

Definitely spend some time modifying (or writing a program that will modify it for you) the filesystem image
**fs.img** to make it inconsistent in different ways to test your program.
