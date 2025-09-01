# LEVEL14

## 1. Inspect The Home Directory

```bash
level14@SnowCrash:~$ ls -la
total 12
dr-x------ 1 level14 level14  100 Mar  5  2016 .
d--x--x--x 1 root    users    340 Aug 30  2015 ..
-r-x------ 1 level14 level14  220 Apr  3  2012 .bash_logout
-r-x------ 1 level14 level14 3518 Aug 30  2015 .bashrc
-r-x------ 1 level14 level14  675 Apr  3  2012 .profile
```

The home directory is essentially empty of any useful files related to the challenge.

```bash
level14@SnowCrash:~$ find / -type f -exec grep -H "flag14" {} + 2>/dev/null
/etc/group:flag:x:1001:flag00,flag01,flag02,flag03,flag04,flag05,flag06,flag07,flag08,flag09,flag10,flag11,flag12,flag13,flag14
/etc/group:flag14:x:3014:
/etc/passwd:flag14:x:3014:3014::/home/flag/flag14:/bin/bash
Binary file /proc/2076/task/2076/cmdline matches
Binary file /proc/2076/cmdline matches
/rofs/etc/group:flag:x:1001:flag00,flag01,flag02,flag03,flag04,flag05,flag06,flag07,flag08,flag09,flag10,flag11,flag12,flag13,flag14
/rofs/etc/group:flag14:x:3014:
/rofs/etc/passwd:flag14:x:3014:3014::/home/flag/flag14:/bin/bash
level14@SnowCrash:~$ find / -user level14 2>/dev/null
/dev/pts/0
/proc/1905
[only proc file...]
```

And there is no relevant files related to `flag14` or owned by `level14`. All other matches are system or process files unrelated to the challenge.
The solution lies directly inside the `/bin/getflag` binary.

## 2 Analyse The Binary

```bash
level14@SnowCrash:~$ strings /bin/getflag
/lib/ld-linux.so.2
__gmon_start__
libc.so.6
_IO_stdin_used
__stack_chk_fail
strdup
stdout
fputc
fputs
getenv
stderr
getuid
ptrace
fwrite
open
__libc_start_main
GLIBC_2.4
GLIBC_2.0
PTRh@
QVhF
UWVS
[^_]
0123456
You should not reverse this
LD_PRELOAD
Injection Linked lib detected exit..
/etc/ld.so.preload
/proc/self/maps
/proc/self/maps is unaccessible, probably a LD_PRELOAD attempt exit..
libc
Check flag.Here is your token : 
You are root are you that dumb ?
I`fA>_88eEd:=`85h0D8HE>,D
7`4Ci4=^d=J,?>i;6,7d416,7
<>B16\AD<C6,G_<1>^7ci>l4B
B8b:6,3fj7:,;bh>D@>8i:6@D
?4d@:,C>8C60G>8:h:Gb4?l,A
G8H.6,=4k5J0<cd/D@>>B:>:4
H8B8h_20B4J43><8>\ED<;j@3
78H:J4<4<9i_I4k0J^5>B1j`9
bci`mC{)jxkn<"uD~6%g7FK`7
Dc6m~;}f8Cj#xFkel;#&ycfbK
74H9D^3ed7k05445J0E4e;Da4
70hCi,E44Df[A4B/J@3f<=:`D
8_Dw"4#?+3i]q&;p6 gtw88EC
boe]!ai0FB@.:|L6l@A?>qJ}I
g <t61:|4_|!@IF.-62FH&G~DCK/Ekrvvdwz?v|
Nope there is no token here for you sorry. Try again :)
00000000 00:00 0
LD_PRELOAD detected through memory maps exit ..
;*2$"$
```

Three elements stand out from this `strings` output:
```
getuid

ptrace

I`fA>_88eEd:=`85h0D8HE>,D
[...]
g <t61:|4_|!@IF.-62FH&G~DCK/Ekrvvdwz?v|
```

A call to `getuid`, indicating the binary checks the UID, similar to level13, probably to verify the current user and provide the associated token.
A call to `ptrace`, suggesting the program may attempts to dectect debuggers, which protects against dynamic analysis.
15 encrypted-looking strings, which are likely the flag obfuscated flag.

```bash
level14@SnowCrash:~$ strace getflag
execve("/bin/getflag", ["getflag"], [/* 18 vars */]) = 0
brk(0)                                  = 0x804c000
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
mprotect(0x804a000, 4096, PROT_READ)    = 0
mprotect(0xb7ffe000, 4096, PROT_READ)   = 0
munmap(0xb7fd5000, 21440)               = 0
ptrace(PTRACE_TRACEME, 0, 0x1, 0)       = -1 EPERM (Operation not permitted)
fstat64(1, {st_mode=S_IFCHR|0620, st_rdev=makedev(136, 0), ...}) = 0
mmap2(NULL, 4096, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0xb7fda000
write(1, "You should not reverse this\n", 28You should not reverse this
) = 28
exit_group(1)                           = ?
```

This `strace` output confirms the use of `ptrace` to prevent debugging or external manipulation.
```
ptrace(PTRACE_TRACEME, 0, 0x1, 0)       = -1 EPERM (Operation not permitted)
```

`ptrace` is a system call that allows one process to observe or control the execution of another process, and examine or modify its memory and registers. It can also be used to block debugging by preventing other processes from attaching to it.

```bash
level14@SnowCrash:~$ objdump -d /bin/getflag
[...]
48946 <main>:
 8048946:	55                   	push   %ebp
 8048947:	89 e5                	mov    %esp,%ebp
 8048949:	53                   	push   %ebx
[...]
 8048afd:	e8 ae f9 ff ff       	call   80484b0 <getuid@plt>
 8048b02:	89 44 24 18          	mov    %eax,0x18(%esp)
 8048b06:	8b 44 24 18          	mov    0x18(%esp),%eax
 8048b0a:	3d be 0b 00 00       	cmp    $0xbbe,%eax
 8048b0f:	0f 84 b6 01 00 00    	je     8048ccb <main+0x385>
 8048b15:	3d be 0b 00 00       	cmp    $0xbbe,%eax
 8048b1a:	77 4c                	ja     8048b68 <main+0x222>
 8048b1c:	3d ba 0b 00 00       	cmp    $0xbba,%eax
 8048b21:	0f 84 14 01 00 00    	je     8048c3b <main+0x2f5>
 8048b27:	3d ba 0b 00 00       	cmp    $0xbba,%eax
 8048b2c:	77 1f                	ja     8048b4d <main+0x207>
 8048b2e:	3d b8 0b 00 00       	cmp    $0xbb8,%eax
 8048b33:	0f 84 ba 00 00 00    	je     8048bf3 <main+0x2ad>
 8048b39:	3d b8 0b 00 00       	cmp    $0xbb8,%eax
 8048b3e:	0f 87 d3 00 00 00    	ja     8048c17 <main+0x2d1>
 8048b44:	85 c0                	test   %eax,%eax
 8048b46:	74 7e                	je     8048bc6 <main+0x280>
 8048b48:	e9 b9 02 00 00       	jmp    8048e06 <main+0x4c0>
 8048b4d:	3d bc 0b 00 00       	cmp    $0xbbc,%eax
 8048b52:	0f 84 2b 01 00 00    	je     8048c83 <main+0x33d>
 8048b58:	3d bc 0b 00 00       	cmp    $0xbbc,%eax
 8048b5d:	0f 87 44 01 00 00    	ja     8048ca7 <main+0x361>
 8048b63:	e9 f7 00 00 00       	jmp    8048c5f <main+0x319>
 8048b68:	3d c2 0b 00 00       	cmp    $0xbc2,%eax
 8048b6d:	0f 84 e8 01 00 00    	je     8048d5b <main+0x415>
 8048b73:	3d c2 0b 00 00       	cmp    $0xbc2,%eax
 8048b78:	77 1b                	ja     8048b95 <main+0x24f>
 8048b7a:	3d c0 0b 00 00       	cmp    $0xbc0,%eax
 8048b7f:	0f 84 8e 01 00 00    	je     8048d13 <main+0x3cd>
 8048b85:	3d c0 0b 00 00       	cmp    $0xbc0,%eax
 8048b8a:	0f 87 a7 01 00 00    	ja     8048d37 <main+0x3f1>
 8048b90:	e9 5a 01 00 00       	jmp    8048cef <main+0x3a9>
 8048b95:	3d c4 0b 00 00       	cmp    $0xbc4,%eax
 8048b9a:	0f 84 03 02 00 00    	je     8048da3 <main+0x45d>
 8048ba0:	3d c4 0b 00 00       	cmp    $0xbc4,%eax
 8048ba5:	0f 82 d4 01 00 00    	jb     8048d7f <main+0x439>
 8048bab:	3d c5 0b 00 00       	cmp    $0xbc5,%eax
 8048bb0:	0f 84 0e 02 00 00    	je     8048dc4 <main+0x47e>
 8048bb6:	3d c6 0b 00 00       	cmp    $0xbc6,%eax
 8048bbb:	0f 84 24 02 00 00    	je     8048de5 <main+0x49f>
 8048bc1:	e9 40 02 00 00       	jmp    8048e06 <main+0x4c0>
 8048bc6:	a1 60 b0 04 08       	mov    0x804b060,%eax
 8048bcb:	89 c2                	mov    %eax,%edx
 8048bcd:	b8 90 90 04 08       	mov    $0x8049090,%eax
 8048bd2:	89 54 24 0c          	mov    %edx,0xc(%esp)
 8048bd6:	c7 44 24 08 21 00 00 	movl   $0x21,0x8(%esp)
 8048bdd:	00 
 8048bde:	c7 44 24 04 01 00 00 	movl   $0x1,0x4(%esp)
 8048be5:	00 
 8048be6:	89 04 24             	mov    %eax,(%esp)
 8048be9:	e8 d2 f8 ff ff       	call   80484c0 <fwrite@plt>
 8048bee:	e9 3c 02 00 00       	jmp    8048e2f <main+0x4e9>
 8048bf3:	a1 60 b0 04 08       	mov    0x804b060,%eax
 8048bf8:	89 c3                	mov    %eax,%ebx
 8048bfa:	c7 04 24 b2 90 04 08 	movl   $0x80490b2,(%esp)
 8048c01:	e8 fe f9 ff ff       	call   8048604 <ft_des>
 8048c06:	89 5c 24 04          	mov    %ebx,0x4(%esp)
 8048c0a:	89 04 24             	mov    %eax,(%esp)
 8048c0d:	e8 1e f9 ff ff       	call   8048530 <fputs@plt>
 8048c12:	e9 18 02 00 00       	jmp    8048e2f <main+0x4e9>
 8048c17:	a1 60 b0 04 08       	mov    0x804b060,%eax
 8048c1c:	89 c3                	mov    %eax,%ebx
 8048c1e:	c7 04 24 cc 90 04 08 	movl   $0x80490cc,(%esp)
 8048c25:	e8 da f9 ff ff       	call   8048604 <ft_des>
 8048c2a:	89 5c 24 04          	mov    %ebx,0x4(%esp)
 8048c2e:	89 04 24             	mov    %eax,(%esp)
 8048c31:	e8 fa f8 ff ff       	call   8048530 <fputs@plt>
 8048c36:	e9 f4 01 00 00       	jmp    8048e2f <main+0x4e9>
 8048c3b:	a1 60 b0 04 08       	mov    0x804b060,%eax
 8048c40:	89 c3                	mov    %eax,%ebx
 8048c42:	c7 04 24 e6 90 04 08 	movl   $0x80490e6,(%esp)
 8048c49:	e8 b6 f9 ff ff       	call   8048604 <ft_des>
 8048c4e:	89 5c 24 04          	mov    %ebx,0x4(%esp)
 8048c52:	89 04 24             	mov    %eax,(%esp)
 8048c55:	e8 d6 f8 ff ff       	call   8048530 <fputs@plt>
 8048c5a:	e9 d0 01 00 00       	jmp    8048e2f <main+0x4e9>
 8048c5f:	a1 60 b0 04 08       	mov    0x804b060,%eax
 8048c64:	89 c3                	mov    %eax,%ebx
 8048c66:	c7 04 24 00 91 04 08 	movl   $0x8049100,(%esp)
 8048c6d:	e8 92 f9 ff ff       	call   8048604 <ft_des>
 8048c72:	89 5c 24 04          	mov    %ebx,0x4(%esp)
 8048c76:	89 04 24             	mov    %eax,(%esp)
 8048c79:	e8 b2 f8 ff ff       	call   8048530 <fputs@plt>
 8048c7e:	e9 ac 01 00 00       	jmp    8048e2f <main+0x4e9>
 8048c83:	a1 60 b0 04 08       	mov    0x804b060,%eax
 8048c88:	89 c3                	mov    %eax,%ebx
 8048c8a:	c7 04 24 1a 91 04 08 	movl   $0x804911a,(%esp)
 8048c91:	e8 6e f9 ff ff       	call   8048604 <ft_des>
 8048c96:	89 5c 24 04          	mov    %ebx,0x4(%esp)
 8048c9a:	89 04 24             	mov    %eax,(%esp)
 8048c9d:	e8 8e f8 ff ff       	call   8048530 <fputs@plt>
 8048ca2:	e9 88 01 00 00       	jmp    8048e2f <main+0x4e9>
 8048ca7:	a1 60 b0 04 08       	mov    0x804b060,%eax
 8048cac:	89 c3                	mov    %eax,%ebx
 8048cae:	c7 04 24 34 91 04 08 	movl   $0x8049134,(%esp)
 8048cb5:	e8 4a f9 ff ff       	call   8048604 <ft_des>
 8048cba:	89 5c 24 04          	mov    %ebx,0x4(%esp)
 8048cbe:	89 04 24             	mov    %eax,(%esp)
 8048cc1:	e8 6a f8 ff ff       	call   8048530 <fputs@plt>
 8048cc6:	e9 64 01 00 00       	jmp    8048e2f <main+0x4e9>
 8048ccb:	a1 60 b0 04 08       	mov    0x804b060,%eax
 8048cd0:	89 c3                	mov    %eax,%ebx
 8048cd2:	c7 04 24 4e 91 04 08 	movl   $0x804914e,(%esp)
 8048cd9:	e8 26 f9 ff ff       	call   8048604 <ft_des>
 8048cde:	89 5c 24 04          	mov    %ebx,0x4(%esp)
 8048ce2:	89 04 24             	mov    %eax,(%esp)
 8048ce5:	e8 46 f8 ff ff       	call   8048530 <fputs@plt>
 8048cea:	e9 40 01 00 00       	jmp    8048e2f <main+0x4e9>
 8048cef:	a1 60 b0 04 08       	mov    0x804b060,%eax
 8048cf4:	89 c3                	mov    %eax,%ebx
 8048cf6:	c7 04 24 68 91 04 08 	movl   $0x8049168,(%esp)
 8048cfd:	e8 02 f9 ff ff       	call   8048604 <ft_des>
 8048d02:	89 5c 24 04          	mov    %ebx,0x4(%esp)
 8048d06:	89 04 24             	mov    %eax,(%esp)
 8048d09:	e8 22 f8 ff ff       	call   8048530 <fputs@plt>
 8048d0e:	e9 1c 01 00 00       	jmp    8048e2f <main+0x4e9>
 8048d13:	a1 60 b0 04 08       	mov    0x804b060,%eax
 8048d18:	89 c3                	mov    %eax,%ebx
 8048d1a:	c7 04 24 82 91 04 08 	movl   $0x8049182,(%esp)
 8048d21:	e8 de f8 ff ff       	call   8048604 <ft_des>
 8048d26:	89 5c 24 04          	mov    %ebx,0x4(%esp)
 8048d2a:	89 04 24             	mov    %eax,(%esp)
 8048d2d:	e8 fe f7 ff ff       	call   8048530 <fputs@plt>
 8048d32:	e9 f8 00 00 00       	jmp    8048e2f <main+0x4e9>
 8048d37:	a1 60 b0 04 08       	mov    0x804b060,%eax
 8048d3c:	89 c3                	mov    %eax,%ebx
 8048d3e:	c7 04 24 9c 91 04 08 	movl   $0x804919c,(%esp)
 8048d45:	e8 ba f8 ff ff       	call   8048604 <ft_des>
 8048d4a:	89 5c 24 04          	mov    %ebx,0x4(%esp)
 8048d4e:	89 04 24             	mov    %eax,(%esp)
 8048d51:	e8 da f7 ff ff       	call   8048530 <fputs@plt>
 8048d56:	e9 d4 00 00 00       	jmp    8048e2f <main+0x4e9>
 8048d5b:	a1 60 b0 04 08       	mov    0x804b060,%eax
 8048d60:	89 c3                	mov    %eax,%ebx
 8048d62:	c7 04 24 b6 91 04 08 	movl   $0x80491b6,(%esp)
 8048d69:	e8 96 f8 ff ff       	call   8048604 <ft_des>
 8048d6e:	89 5c 24 04          	mov    %ebx,0x4(%esp)
 8048d72:	89 04 24             	mov    %eax,(%esp)
 8048d75:	e8 b6 f7 ff ff       	call   8048530 <fputs@plt>
 8048d7a:	e9 b0 00 00 00       	jmp    8048e2f <main+0x4e9>
 8048d7f:	a1 60 b0 04 08       	mov    0x804b060,%eax
 8048d84:	89 c3                	mov    %eax,%ebx
 8048d86:	c7 04 24 d0 91 04 08 	movl   $0x80491d0,(%esp)
 8048d8d:	e8 72 f8 ff ff       	call   8048604 <ft_des>
 8048d92:	89 5c 24 04          	mov    %ebx,0x4(%esp)
 8048d96:	89 04 24             	mov    %eax,(%esp)
 8048d99:	e8 92 f7 ff ff       	call   8048530 <fputs@plt>
 8048d9e:	e9 8c 00 00 00       	jmp    8048e2f <main+0x4e9>
 8048da3:	a1 60 b0 04 08       	mov    0x804b060,%eax
 8048da8:	89 c3                	mov    %eax,%ebx
 8048daa:	c7 04 24 ea 91 04 08 	movl   $0x80491ea,(%esp)
 8048db1:	e8 4e f8 ff ff       	call   8048604 <ft_des>
 8048db6:	89 5c 24 04          	mov    %ebx,0x4(%esp)
 8048dba:	89 04 24             	mov    %eax,(%esp)
 8048dbd:	e8 6e f7 ff ff       	call   8048530 <fputs@plt>
 8048dc2:	eb 6b                	jmp    8048e2f <main+0x4e9>
 8048dc4:	a1 60 b0 04 08       	mov    0x804b060,%eax
 8048dc9:	89 c3                	mov    %eax,%ebx
 8048dcb:	c7 04 24 04 92 04 08 	movl   $0x8049204,(%esp)
 8048dd2:	e8 2d f8 ff ff       	call   8048604 <ft_des>
 8048dd7:	89 5c 24 04          	mov    %ebx,0x4(%esp)
 8048ddb:	89 04 24             	mov    %eax,(%esp)
 8048dde:	e8 4d f7 ff ff       	call   8048530 <fputs@plt>
 8048de3:	eb 4a                	jmp    8048e2f <main+0x4e9>
 8048de5:	a1 60 b0 04 08       	mov    0x804b060,%eax
 8048dea:	89 c3                	mov    %eax,%ebx
 8048dec:	c7 04 24 20 92 04 08 	movl   $0x8049220,(%esp)
 8048df3:	e8 0c f8 ff ff       	call   8048604 <ft_des>
 8048df8:	89 5c 24 04          	mov    %ebx,0x4(%esp)
 8048dfc:	89 04 24             	mov    %eax,(%esp)
 8048dff:	e8 2c f7 ff ff       	call   8048530 <fputs@plt>
 8048e04:	eb 29                	jmp    8048e2f <main+0x4e9>
 8048e06:	a1 60 b0 04 08       	mov    0x804b060,%eax
 8048e0b:	89 c2                	mov    %eax,%edx
 8048e0d:	b8 48 92 04 08       	mov    $0x8049248,%eax
 8048e12:	89 54 24 0c          	mov    %edx,0xc(%esp)
 8048e16:	c7 44 24 08 38 00 00 	movl   $0x38,0x8(%esp)
 8048e1d:	00 
 8048e1e:	c7 44 24 04 01 00 00 	movl   $0x1,0x4(%esp)
 8048e25:	00 
[...]
 8048eca:	c9                   	leave  
 8048ecb:	c3                   	ret    
 8048ecc:	90                   	nop
 8048ecd:	90                   	nop
 8048ece:	90                   	nop
 8048ecf:	90                   	nop

[...]
```

The main function can be roughly divided into four parts:
- Protection checks (not displayed here)
- `getuid` call - at `8048afd`
- Comparisons of the `getuid` result, ending in conditional jumps - from `0x8048b0a` to `0x8048bc1`
- Groups of `ft_des` and `fputs` calls - from `0x8048bf3` to `0x8048e04`

The function `ft_des` is likely an encryption routine, `DES` being a known encryption algorithm, and it is related to each token output.

In summary :
The binary checks the UID and returns either a token or an error message depending on the `getuid()` result. It's protected against external manipulation via `ptrace()`, a the token is decrypted using `ft_des` before being printed.

From this analysis, there is three ways to retrieve the final token:
- Bypass the protections using a debugger.
- Use a debugger to jump directly to the address of the desired token is printed.
- Reverse-engineer `ft_des` to decrypt the stored ciphertext and obtain the token.

**Explanation:**
>- `strings`		: displays printable strings from a binary file.
>- `ptrace`			: allows a process to observe or control another process
>- `objdump`		: disassembles a binary to show its assembly instructions

## 3. Bypass Protections With `gdb`

The UID sequence is logical, but confirming the UID needed for flag14 is straightforward:
```bash
level14@SnowCrash:~$ cat /etc/passwd | grep flag14
flag14:x:3014:3014::/home/flag/flag14:/bin/bash
```
The required UID is `3014`. 

To bypass the protections, both `ptrace()` and `getuid()` must be intercepted in `gdb`:
- Set the return value of `ptrace` to `0` to bypass the anti-debug check.
- Set the return value of `getuid` to `3014` so the binary believes it is running as `flag14`.

```bash
level14@SnowCrash:~$ gdb -q getflag
Reading symbols from /bin/getflag...(no debugging symbols found)...done.
(gdb) b ptrace
Breakpoint 1 at 0x8048540
(gdb) b getuid
Breakpoint 2 at 0x80484b0
(gdb) r
Starting program: /bin/getflag 

Breakpoint 1, 0xb7f146d0 in ptrace () from /lib/i386-linux-gnu/libc.so.6
(gdb) n
Single stepping until exit from function ptrace,
which has no line number information.
0x0804898e in main ()
(gdb) set $eax=0
(gdb) c
Continuing.

Breakpoint 2, 0xb7ee4cc0 in getuid () from /lib/i386-linux-gnu/libc.so.6
(gdb) n
Single stepping until exit from function getuid,
which has no line number information.
0x08048b02 in main ()
(gdb) set $eax=3014
(gdb) c
Continuing.
Check flag.Here is your token : XXX
[Inferior 1 (process 2294) exited normally]
```

**Bypass explanation:**
>- `gdb getflag`											: launch `gdb` on the binary `getflag`.
>- `b ptrace`												: set a breakpoint on the `ptrace` function.
>- `b getuid`												: set a breakpoint on the `getuid` function.
>- `r`														: run the program and stop at the `getuid` breakpoint.
>- `Breakpoint 1, 0xb7f146d0 in ptrace ()`					: indicates the program has paused at `ptrace()`.
>- `n`														: step to the next instruction after `ptrace`.
>- `set $eax=0`												: override the return value of `ptrace()` to `0` (instead of `-1`).
>- `c`														: resume execution to reach the next breakpoint on `getuid`.
>- `Breakpoint 2, 0xb7ee4cc0 in getuid ()`					: indicates the program has paused at `getuid()`.
>- `n`														: step to the next instruction after `getuid`.
>- `Single stepping until exit from function getuid,`		: indicates the program is executing `getuid()` step by step and is about to return.
>- `set $eax=3014`											: override the return value of `getuid()` to `3014` (instead of `2014`).
>- `c`														: resume execution to reach the token output.

**Explanation:**
>- `gdb` 				: GNU Debugger, allows inspection and modification of running programs.
>- `-q` 				: quiet mode, suppresses the introductory messages.

>- `b FUNCTION` 		: set a breakpoint at `FUNCTION`.
>- `r` 					: run the program in the debugger.
>- `n` 					: execute the next instruction (step over).
>- `set $VAR=VALUE` 	: set the CPU register or variable `VAR` to `VALUE`. 
>- `$eax` 				: x86 CPU register holding the return value of the last function call.
>- `c`		 			: alias of `continue` - resumes program execution until the next breakpoint or program end.

## 4.a Bypass With `gdb` - Jump To The Address

The seconde method is to jump directly to the point in the code where the token is printed.
This can be done also with `gdb`.

First, the target address needs to be identified. In the disassembly, the last `cmp` of the `getuid` is:

```bash
 8048bb6:	3d c6 0b 00 00       	cmp    $0xbc6,%eax
 8048bbb:	0f 84 24 02 00 00    	je     8048de5 <main+0x49f>
```
It compares `%eax` (the return value of `getuid`) with `$0xbc6` and jumps to `0x8048de5` if the comparison is true (i.e., there are equal).
The value `0xbc6` is hexadecimal, converted to decimal, it is `3014`.

```bash
 8048de5:	a1 60 b0 04 08       	mov    0x804b060,%eax
 8048dea:	89 c3                	mov    %eax,%ebx
 8048dec:	c7 04 24 20 92 04 08 	movl   $0x8049220,(%esp)
 8048df3:	e8 0c f8 ff ff       	call   8048604 <ft_des>
 8048df8:	89 5c 24 04          	mov    %ebx,0x4(%esp)
 8048dfc:	89 04 24             	mov    %eax,(%esp)
 8048dff:	e8 2c f7 ff ff       	call   8048530 <fputs@plt>
```

This sequence loads a string (likely the encrypted token `g <t61:|4_|!@IF.-62FH&G~DCK/Ekrvvdwz?v|`) into a register, passes it as an argument to `ft_des`, and then prints the result with `fputs`.

By forcing the jump `je     8048de5`, it is possible to skip the UID check and directly print the token without needing the correct UID.

```bash
level14@SnowCrash:~$ gdb getflag
[...]
(gdb) b main
Breakpoint 1 at 0x804894a
(gdb) r
Starting program: /bin/getflag 

Breakpoint 1, 0x0804894a in main ()
(gdb) jump *0x08048de5
Continuing at 0x8048de5.
XXX
*** stack smashing detected ***: /bin/getflag terminated

```

By finding the correct segment, it is possible to jump directly directly to its starting address of the segment. Then continue the execution to retrieve the token.

**Bypass explanation:**
>- `gdb getflag`											: launch `gdb` on the binary `getflag`.
>- `b main`													: set a breakpoint on the `main` function.
>- `r`														: run the program and stop at the `getuid` breakpoint.
>- `Breakpoint 1, 0x0804894a in main ()`					: indicates the program has paused at `main()`.
>- `jump  *0x08048de5`										: jump to the address `0x08048de5`.

**Explanation:**
>- `gdb` 				: GNU Debugger, allows inspection and modification of running programs.

>- `b FUNCTION` 		: set a breakpoint at `FUNCTION`.
>- `r` 					: run the program in the debugger.
>- `jump *ADDRESS`		: jumps the execution directly to the instruction at `ADDRESS`.

## 4.b Jump directly To The Address

Another method would be to break directly at the target address instead of breaking on `main` and then jumping.

```bash
level14@SnowCrash:~$ gdb getflag
[...]
(gdb) b *0x8048de5
Breakpoint 1 at 0x8048de5
(gdb) r
Starting program: /bin/getflag 
You should not reverse this
[Inferior 1 (process 2470) exited with code 01]
```

However, this approach does not work because the `ptrace` function protects the binary against external intervention. Setting the breakpoint at `main` first allows the program to pass the `ptrace` check before performing the jump.

## 5. Reverse-Engineer `ft_des`

The last method consists of analyzing the function `ft_des` in order to decrypt the stored token `g <t61:|4_|!@IF.-62FH&G~DCK/Ekrvvdwz?v|`.


To achieve this, the function `ft_des` must first be disassembled and studied, in the same way as the `main` function was previously analyzed. This step makes it possible to understand its logic and reconstruct the decryption routine.

> In addition to `objdump`, another common tool for disassembling a program is `gdb`.

```bash
level14@SnowCrash:~$ gdb -q getflag
Reading symbols from /bin/getflag...(no debugging symbols found)...done.
(gdb) disassemble ft_des
Dump of assembler code for function ft_des:
   0x08048604 <+0>:	push   %ebp
   0x08048605 <+1>:	mov    %esp,%ebp
   0x08048607 <+3>:	push   %edi
   0x08048608 <+4>:	sub    $0x44,%esp
   0x0804860b <+7>:	mov    0x8(%ebp),%eax
   0x0804860e <+10>:	mov    %eax,(%esp)
   0x08048611 <+13>:	call   0x8048490 <strdup@plt>
   0x08048616 <+18>:	mov    %eax,-0xc(%ebp)
   0x08048619 <+21>:	movl   $0x0,-0x18(%ebp)
   0x08048620 <+28>:	movl   $0x0,-0x1c(%ebp)
   0x08048627 <+35>:	jmp    0x80486eb <ft_des+231>
   0x0804862c <+40>:	cmpl   $0x6,-0x18(%ebp)
   0x08048630 <+44>:	jne    0x8048639 <ft_des+53>
   0x08048632 <+46>:	movl   $0x0,-0x18(%ebp)
   0x08048639 <+53>:	mov    -0x1c(%ebp),%eax
   0x0804863c <+56>:	and    $0x1,%eax
   0x0804863f <+59>:	test   %al,%al
   0x08048641 <+61>:	je     0x804868f <ft_des+139>
   0x08048643 <+63>:	movl   $0x0,-0x14(%ebp)
   0x0804864a <+70>:	jmp    0x804867a <ft_des+118>
   0x0804864c <+72>:	mov    -0x1c(%ebp),%eax
   0x0804864f <+75>:	mov    -0xc(%ebp),%edx
   0x08048652 <+78>:	add    %edx,%eax
   0x08048654 <+80>:	movzbl (%eax),%edx
   0x08048657 <+83>:	add    $0x1,%edx
   0x0804865a <+86>:	mov    %dl,(%eax)
   0x0804865c <+88>:	mov    -0x1c(%ebp),%eax
   0x0804865f <+91>:	mov    -0xc(%ebp),%edx
   0x08048662 <+94>:	add    %edx,%eax
   0x08048664 <+96>:	movzbl (%eax),%eax
   0x08048667 <+99>:	cmp    $0x7f,%al
   0x08048669 <+101>:	jne    0x8048676 <ft_des+114>
   0x0804866b <+103>:	mov    -0x1c(%ebp),%eax
   0x0804866e <+106>:	mov    -0xc(%ebp),%edx
   0x08048671 <+109>:	add    %edx,%eax
   0x08048673 <+111>:	movb   $0x20,(%eax)
   0x08048676 <+114>:	addl   $0x1,-0x14(%ebp)
   0x0804867a <+118>:	mov    -0x18(%ebp),%eax
   0x0804867d <+121>:	add    $0x8048fa0,%eax
   0x08048682 <+126>:	movzbl (%eax),%eax
   0x08048685 <+129>:	movsbl %al,%eax
   0x08048688 <+132>:	cmp    -0x14(%ebp),%eax
   0x0804868b <+135>:	jg     0x804864c <ft_des+72>
   0x0804868d <+137>:	jmp    0x80486e3 <ft_des+223>
   0x0804868f <+139>:	mov    -0x1c(%ebp),%eax
   0x08048692 <+142>:	and    $0x1,%eax
   0x08048695 <+145>:	test   %eax,%eax
   0x08048697 <+147>:	jne    0x80486e3 <ft_des+223>
   0x08048699 <+149>:	movl   $0x0,-0x10(%ebp)
   0x080486a0 <+156>:	jmp    0x80486d0 <ft_des+204>
   0x080486a2 <+158>:	mov    -0x1c(%ebp),%eax
   0x080486a5 <+161>:	mov    -0xc(%ebp),%edx
   0x080486a8 <+164>:	add    %edx,%eax
   0x080486aa <+166>:	movzbl (%eax),%edx
   0x080486ad <+169>:	sub    $0x1,%edx
   0x080486b0 <+172>:	mov    %dl,(%eax)
   0x080486b2 <+174>:	mov    -0x1c(%ebp),%eax
   0x080486b5 <+177>:	mov    -0xc(%ebp),%edx
   0x080486b8 <+180>:	add    %edx,%eax
   0x080486ba <+182>:	movzbl (%eax),%eax
   0x080486bd <+185>:	cmp    $0x1f,%al
   0x080486bf <+187>:	jne    0x80486cc <ft_des+200>
   0x080486c1 <+189>:	mov    -0x1c(%ebp),%eax
   0x080486c4 <+192>:	mov    -0xc(%ebp),%edx
   0x080486c7 <+195>:	add    %edx,%eax
   0x080486c9 <+197>:	movb   $0x7e,(%eax)
   0x080486cc <+200>:	addl   $0x1,-0x10(%ebp)
   0x080486d0 <+204>:	mov    -0x18(%ebp),%eax
   0x080486d3 <+207>:	add    $0x8048fa0,%eax
   0x080486d8 <+212>:	movzbl (%eax),%eax
   0x080486db <+215>:	movsbl %al,%eax
   0x080486de <+218>:	cmp    -0x10(%ebp),%eax
   0x080486e1 <+221>:	jg     0x80486a2 <ft_des+158>
   0x080486e3 <+223>:	addl   $0x1,-0x1c(%ebp)
   0x080486e7 <+227>:	addl   $0x1,-0x18(%ebp)
   0x080486eb <+231>:	mov    -0xc(%ebp),%eax
   0x080486ee <+234>:	movl   $0xffffffff,-0x2c(%ebp)
   0x080486f5 <+241>:	mov    %eax,%edx
   0x080486f7 <+243>:	mov    $0x0,%eax
   0x080486fc <+248>:	mov    -0x2c(%ebp),%ecx
   0x080486ff <+251>:	mov    %edx,%edi
   0x08048701 <+253>:	repnz scas %es:(%edi),%al
   0x08048703 <+255>:	mov    %ecx,%eax
   0x08048705 <+257>:	not    %eax
   0x08048707 <+259>:	sub    $0x1,%eax
   0x0804870a <+262>:	cmp    -0x1c(%ebp),%eax
   0x0804870d <+265>:	ja     0x804862c <ft_des+40>
   0x08048713 <+271>:	mov    -0xc(%ebp),%eax
   0x08048716 <+274>:	add    $0x44,%esp
   0x08048719 <+277>:	pop    %edi
   0x0804871a <+278>:	pop    %ebp
   0x0804871b <+279>:	ret    
End of assembler dump.
```

The function `ft_des` is fully disassembled, but its logic is rather complex to analyze directly in assembly.
A much simpler approach is to decompile the binary using a reverse-engineering tool.

**Explanation:**
>- `gdb` 					: GNU Debugger, allows inspection and modification of running programs.
>- `-q` 					: quiet mode, suppresses the introductory messages.

>- `disassemble FUNCTION`	: displays the assembly instructions of the given `FUNCTION`

A decompiler reconstructs a C-like representation of the function, making it much easier to understand the decryption algorithm.
It can be downloaded (or run in a Docker container), but there are also online tools, such as [Dogbolt](https://dogbolt.org/), which provides multiple decompilers at the same time.

Here is the `Hex-Rays` decompiler result:
```c
char * ft_des(char *s)
{
  unsigned int i; // [esp+2Ch] [ebp-1Ch]
  int v3; // [esp+30h] [ebp-18h]
  int j; // [esp+34h] [ebp-14h]
  int k; // [esp+38h] [ebp-10h]
  char *v6; // [esp+3Ch] [ebp-Ch]

  v6 = strdup(s);
  v3 = 0;
  for ( i = 0; strlen(v6) > i; ++i )
  {
    if ( v3 == 6 )
      v3 = 0;
    if ( (i & 1) != 0 )
    {
      for ( j = 0; *(char *)(v3 + 134516640) > j; ++j )
      {
        if ( ++v6[i] == 127 )
          v6[i] = 32;
      }
    }
    else
    {
      for ( k = 0; *(char *)(v3 + 134516640) > k; ++k )
      {
        if ( --v6[i] == 31 )
          v6[i] = 126;
      }
    }
    ++v3;
  }
  return v6;
}
```

As a reminder, the decompiled code only provides pseudo-code, not the actual running code. Therefore, it must be understood and reimplemented.

**Analyze The Function**
The decompiled code shows that `ft_des` performs a character-wise transformation of the input string:
- It duplicates the input string using `strdup`.
- It iterates over each character, applying a transformation depending on the character’s index (even or odd).
- Odd-indexed characters are incremented in a loop, wrapping from `127` (`[DEL]`) back to `32` (`SPACE`).
- Even-indexed characters are decremented in a loop, wrapping from `31` (`[UNIT SEPARATOR]`) back to `126` (`~`).
- A counter `v3` cycles from `0` to `6`, and appears to influence the number of iterations in the inner loops.
- The transformed string is returned, representing the decrypted token.

The unclear part is `*(char *)(v3 + 134516640)` in the inner loops.
This appears to access a table from index `v3`. `Hex-Rays` could not resolve this array (likely located outside of the analyzed file) and shows it as a raw memory value. This table is likely the key used in the encryption, and since `v3` iterates only from `0` to `6`, the key is probably of size `7`.

In the `strings` output of `getflag`, there is a notable 7-character numeric sequence:
```bash
level14@SnowCrash:~$ strings /bin/getflag
[...]
0123456
[...]
```

This sequence likely corresponds to the key used in the `ft_des` function. This suggests that `0123456` could represent either the actual key values or a placeholder mapping for the inner loops of the decryption routine.

Now that the pseudo-code is fully understood, it needs to be cleaned up, the key 0123456 inserted, made runnable (by adding the necessary libraries) and tested with the token.

```c

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

char * ft_des(char *s)
{
	unsigned int	i;
	int	key_i;
	int	j;
	int	k;
	int	len;
	char	*str;
	char	key[] = "0123456";	

	str = strdup(s);
	len = strlen(str);
	key_i = 0;
	for ( i = 0; len > i; ++i )
	{
		if ( key_i == 6 )
			key_i = 0;
		if ( (i & 1) != 0 )
		{
			for ( j = 0; key[key_i] > j; ++j )
			{
				if ( ++str[i] == 127 )
				str[i] = 32;
			}
		}
		else
		{
			for ( k = 0; key[key_i] > k; ++k )
			{
			if ( --str[i] == 31 )
				str[i] = 126;
			}
		}
		++key_i;
	}
	return str;
}

int	main(void)
{
	printf("%s", ft_des("g <t61:|4_|!@IF.-62FH&G~DCK/Ekrvvdwz?v|"));
}
```


Then, running the program produces:
```bash
host@pc:> gcc ft_des.c
host@pc:> ./a.out 
XXX                    
```

The output is identical across all three methods.

## 6. Get The Flag

```bash
level14@SnowCrash:~$ su flag14
Password: 
Congratulation. Type getflag to get the key and send it to me the owner of this livecd :)
flag14@SnowCrash:~$ getflag
Check flag.Here is your token : XXX
```