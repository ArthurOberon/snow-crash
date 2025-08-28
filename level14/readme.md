# LEVEL14

## 1. Inspect The Home Directory

```bash
level14@SnowCrash:~$ ls -l
total 0
```

The home directory is empty.

```bash
command check find for user flag14, etc.
```

No files are accessible with flag14 rights.
The solution lies directly inside the `/bin/getflag` binary

## 2 Analyse The Binary

```bash
objdump /bin/getflag
```

The binary check the UID and return the token (or error msg) depending on the result of `getuid()`. Just like `level13` binary.
So it can aslo be bypass with a debugger (`gdb`) by override the return value of `getuid`.

## 3.a Bypass `getuid()` With `gdb`

```bash
test avoid only getuid() check with gdb (like level13)
```

But it the pogram check bypass method and block it. Using `strace` we can see which function it's used.

However, this time the program also detects debuggers by using ptrace().


However, it's doesn't work. `getflag` may also detects debuggers. Using `strace`, we can learn some more.

```bash
strace result
```

Indeed, there a `ptrace()` call that see the debugger:
```
~ptrace(PTRACE_TRACEME, 0, 0, 0) = -1 EPERM~
```

This indicates clearly the binary is protecting itself against debugging.
To bypass it, both `ptrace()` and `getuid()` need to be intercepted inside gdb.

## 4 Bypass Protection With `gdb`

```bash
flag14:x:3014:3014::/home/flag/flag14:/bin/bash
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
Check flag.Here is your token : 
[Inferior 1 (process 2294) exited normally]
```

TEXT

**Bypass explanation:**
>- `gdb ./level14`											: launch `gdb` on the binary `level14`.
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
>- `-q` 				: 

>- `b FUNCTION` 		: set a breakpoint at `FUNCTION`.
>- `r` 					: run the program in the debugger.
>- `n` 					: execute the next instruction (step over).
>- `set $VAR=VALUE` 	: set the CPU register or variable `VAR` to `VALUE`. 
>- `$eax` 				: x86 CPU register holding the return value of the last function call, here `getuid()`
>- `c`		 			: alias of `continue` - resume program execution until the next breakpoint or program end.
	

## 3.b Bypass With `gdb` - Jump To The Address

Alternativly, gdb can be also use to jump directly to the print of the token.

```bash
level14@SnowCrash:~$ gdb getflag
[...]
Reading symbols from /bin/getflag...(no debugging symbols found)...done.
(gdb) b main
Breakpoint 1 at 0x804894a
(gdb) disassemble main
Dump of assembler code for function main:
   0x08048946 <+0>:	push   %ebp
   0x08048947 <+1>:	mov    %esp,%ebp
   0x08048949 <+3>:	push   %ebx
   0x0804894a <+4>:	and    $0xfffffff0,%esp
   0x0804894d <+7>:	sub    $0x120,%esp
	[...]
   0x08048da3 <+1117>:	mov    0x804b060,%eax
   0x08048da8 <+1122>:	mov    %eax,%ebx
   0x08048daa <+1124>:	movl   $0x80491ea,(%esp)
   0x08048db1 <+1131>:	call   0x8048604 <ft_des>
   0x08048db6 <+1136>:	mov    %ebx,0x4(%esp)
   0x08048dba <+1140>:	mov    %eax,(%esp)
   0x08048dbd <+1143>:	call   0x8048530 <fputs@plt>
   0x08048dc2 <+1148>:	jmp    0x8048e2f <main+1257>
   0x08048dc4 <+1150>:	mov    0x804b060,%eax
   0x08048dc9 <+1155>:	mov    %eax,%ebx
   0x08048dcb <+1157>:	movl   $0x8049204,(%esp)
   0x08048dd2 <+1164>:	call   0x8048604 <ft_des>
   0x08048dd7 <+1169>:	mov    %ebx,0x4(%esp)
   0x08048ddb <+1173>:	mov    %eax,(%esp)
   0x08048dde <+1176>:	call   0x8048530 <fputs@plt>
   0x08048de3 <+1181>:	jmp    0x8048e2f <main+1257>
   0x08048de5 <+1183>:	mov    0x804b060,%eax
   0x08048dea <+1188>:	mov    %eax,%ebx
   0x08048dec <+1190>:	movl   $0x8049220,(%esp)
   0x08048df3 <+1197>:	call   0x8048604 <ft_des>
   0x08048df8 <+1202>:	mov    %ebx,0x4(%esp)
   0x08048dfc <+1206>:	mov    %eax,(%esp)
   0x08048dff <+1209>:	call   0x8048530 <fputs@plt>
   0x08048e04 <+1214>:	jmp    0x8048e2f <main+1257>
	[...]
   0x08048eca <+1412>:	leave  
   0x08048ecb <+1413>:	ret    
End of assembler dump.

[...]
```

By disassemble the main function the all of `ft_des` can be found.
ft_des stand for DES that is a encryption method. Probably the function to decrypt the code. The last one must be for the last level.

**Explanation:**
>- `disassemble`

```bash
[...]
(gdb) r
Starting program: /bin/getflag 

Breakpoint 1, 0x0804894a in main ()
(gdb) jump *0x08048de5
Continuing at 0x8048de5.
XXX
*** stack smashing detected ***: /bin/getflag terminated

```

Know the address of the last ft_des and print, we can jump to the start of the segment and continue the code to do the decryption and print.

**Bypass explanation:**
>- `gdb ./level13`											: launch `gdb` on the binary `level13`.
>- `b main`													: set a breakpoint on the `main` function.
>- `r`														: run the program and stop at the `getuid` breakpoint.
>- `Breakpoint 1, 0x0804894a in main ()`					: indicates the program has paused at `main()`.
>- `jump  *0x08048de5`										: jump to the address `0x08048de5`.

**Explanation:**
>- `gdb` 				: GNU Debugger, allows inspection and modification of running programs.

>- `b FUNCTION` 		: set a breakpoint at `FUNCTION`.
>- `r` 					: run the program in the debugger.
>- `jump`				: jump to ???.
	