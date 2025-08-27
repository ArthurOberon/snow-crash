# LEVEL13

## 1. Inspect The Home Directory

```bash
level13@SnowCrash:~$ ls -l
total 8
-rwsr-sr-x 1 flag13 level13 7303 Aug 30  2015 level13
```

The binary `level13` is owned by `flag13`, with the SUID bit set (`-rwsr-sr-x`), meaning it will run with the privileges of `flag13`.

```bash
level13@SnowCrash:~$ ./level13 
UID 2013 started us but we we expect 4242
level13@SnowCrash:~$ ./level13 test
UID 2013 started us but we we expect 4242
```

The program checks the `UID` using `getuid()`. Only UID `4242` is accepted; otherwise, the program exits.

## 2. Confirm the Hypothesis

```bash
level13@SnowCrash:~$ strings level13 
/lib/ld-linux.so.2
__gmon_start__
libc.so.6
_IO_stdin_used
exit
strdup
printf
getuid
__libc_start_main
GLIBC_2.0
PTRh`
UWVS
[^_]
0123456
UID %d started us but we we expect %d
boe]!ai0FB@.:|L6l@A?>qJ}I
your token is %s
;*2$"$
[...]
```

`strings` confirms the binary calls `getuid()`, and also contains what appears to be an encrypted token : `boe]!ai0FB@.:|L6l@A?>qJ}I`. However, this token cannot be directly used or decoded.

**Explanation:**
>- `strings`			: displays printable strings from a binary file.

```bash
level13@SnowCrash:~$ strace ./level13 
execve("./level13", ["./level13"], [/* 18 vars */]) = 0
brk(0)                                  = 0x804b000
access("/etc/ld.so.nohwcap", F_OK)      = -1 ENOENT (No such file or directory)
mmap2(NULL, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0xb7fdb000
access("/etc/ld.so.preload", R_OK)      = -1 ENOENT (No such file or directory)
open("/etc/ld.so.cache", O_RDONLY|O_CLOEXEC) = 3
fstat64(3, {st_mode=S_IFREG|0644, st_size=21440, ...}) = 0
mmap2(NULL, 21440, PROT_READ, MAP_PRIVATE, 3, 0) = 0xb7fd5000
close(3)                                = 0
access("/etc/ld.so.nohwcap", F_OK)      = -1 ENOENT (No such file or directory)
open("/lib/i386-linux-gnu/libc.so.6", O_RDONLY|O_CLOEXEC) = 3
read(3, "\177ELF\1\1\1\0\0\0\0\0\0\0\0\0\3\0\3\0\1\0\0\0000\226\1\0004\0\0\0"..., 512) = 512
fstat64(3, {st_mode=S_IFREG|0755, st_size=1730024, ...}) = 0
mmap2(NULL, 1739484, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0xb7e2c000
mmap2(0xb7fcf000, 12288, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x1a3) = 0xb7fcf000
mmap2(0xb7fd2000, 10972, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS, -1, 0) = 0xb7fd2000
close(3)                                = 0
mmap2(NULL, 4096, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0xb7e2b000
set_thread_area({entry_number:-1 -> 6, base_addr:0xb7e2b900, limit:1048575, seg_32bit:1, contents:0, read_exec_only:0, limit_in_pages:1, seg_not_present:0, useable:1}) = 0
mprotect(0xb7fcf000, 8192, PROT_READ)   = 0
mprotect(0x8049000, 4096, PROT_READ)    = 0
mprotect(0xb7ffe000, 4096, PROT_READ)   = 0
munmap(0xb7fd5000, 21440)               = 0
getuid32()                              = 2013
getuid32()                              = 2013
fstat64(1, {st_mode=S_IFCHR|0620, st_rdev=makedev(136, 0), ...}) = 0
mmap2(NULL, 4096, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0xb7fda000
write(1, "UID 2013 started us but we we ex"..., 42UID 2013 started us but we we expect 4242
) = 42
exit_group(1)                           = ?
```

`strace` traces all system calls and signals made by the program. The important output here is:
```
getuid32()                              = 2013
```

It's confirm that `getuid()` returns `2013`.

**Explanation:**
>- `strace`			: traces system calls and signals made by a process.

The UID could normally be changed by `root` using `chown` and the SUID bit:
```bash
root@SnowCrash:~$ sudo chown 4242:4242 level13
root@SnowCrash:~$ sudo chown u+s level13
```

Alternatively, a user with appropriate `sudo` rights could execute the program as a specific UID:
```bash
sudouser@SnowCrash:~$ sudo -u \#4242 ./level13
```

Howerver the user `slevel13` does not have sufficient permissions to perform either action.

**Explanation:**
>- `sudo`			: execute a command as another user, typically `root`.

>- `chown`			: change the owner and/or group of a file.
>- `XXXX:XXXX`		: specifies the new owner and group (`UID:GID`).
>- `u+s`			: sets the SUID bit, so the binary runs with the file owner’s privileges.

>- `sudo -u`		: run a command as a specified user

>- `\#XXXX`			: numeric UID of the user to run the command as.

## 3. Bypass The Protection

The UID check can be bypassed using a debugger (`gdb`) by modifying the return value of `getuid()` immediately before the program evaluates it.

```bash
level13@SnowCrash:~$ gdb ./level13
[...]
(gdb) b getuid
Breakpoint 1 at 0x8048380
(gdb) r
Starting program: /home/user/level13/level13 

Breakpoint 1, 0xb7ee4cc0 in getuid () from /lib/i386-linux-gnu/libc.so.6
(gdb) n
Single stepping until exit from function getuid,
which has no line number information.
0x0804859a in main ()
(gdb) set $eax=4242
(gdb) continue
Continuing.
your token is XXX
[Inferior 1 (process 2307) exited with code 050]
```

**Bypass explanation:**
>- `gdb ./level13`											: launch `gdb` on the binary `level13`.
>- `b getuid`												: set a breakpoint on the `getuid` function.
>- `r`														: run the program and stop at the `getuid` breakpoint.
>- `Breakpoint 1, 0xb7ee4cc0 in getuid ()`					: indicates the program has paused at `getuid()`.
>- `n`														: step to the next instruction after `getuid`.
>- `Single stepping until exit from function getuid,`		: indicates the program is executing `getuid()` step by step and is about to return.
>- `set $eax=4242`											: override the return value of `getuid()` to `4242` (instead of `2013`).
>- `continue`												: resume execution to reach the token output.

**Explanation:**
>- `gdb` 				: GNU Debugger, allows inspection and modification of running programs.

>- `b FUNCTION` 		: set a breakpoint at `FUNCTION`.
>- `r` 					: run the program in the debugger.
>- `n` 					: execute the next instruction (step over).
>- `set $VAR=VALUE` 	: set the CPU register or variable `VAR` to `VALUE`. 
>- `$eax` 				: x86 CPU register holding the return value of the last function call, here `getuid()`
>- `continue` 			: resume program execution until the next breakpoint or program end.
	