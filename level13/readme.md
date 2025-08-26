# LEVEL13

## 1. 

```bash
level13@SnowCrash:~$ ls -l
total 8
-rwsr-sr-x 1 flag13 level13 7303 Aug 30  2015 level13
```

**Explanation:**
>- `` 				: 
>- `` 				: 
>- `` 				: 
>- `` 				: 
## 2. 

```bash
level13@SnowCrash:~$ gdb ./level13
GNU gdb (Ubuntu/Linaro 7.4-2012.04-0ubuntu2.1) 7.4-2012.04
Copyright (C) 2012 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
and "show warranty" for details.
This GDB was configured as "i686-linux-gnu".
For bug reporting instructions, please see:
<http://bugs.launchpad.net/gdb-linaro/>...
Reading symbols from /home/user/level13/level13...(no debugging symbols found)...done.
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
(gdb) n
Single stepping until exit from function main,
which has no line number information.
your token is XXX
0xb7e454d3 in __libc_start_main () from /lib/i386-linux-gnu/libc.so.6

```


## 3. 

```bash
```

## 4. 


## 5. 

```
```

## 6. Get The Flag

```bash
> su flag13
Password: 
Don't forget to launch getflag !

> getflag
Check flag.Here is your token : XXX
```
