# LEVEL10

## 1. 

```bash
level10@SnowCrash:~$ ls -l
total 16
-rwsr-sr-x+ 1 flag10 level10 10817 Mar  5  2016 level10
-rw-------  1 flag10 flag10     26 Mar  5  2016 token
level10@SnowCrash:~$ ./level10 
./level10 file host
	sends file to host if you have access to it
level10@SnowCrash:~$ cat token 
cat: token: Permission denied
level10@SnowCrash:~$ ./level10 token 
./level10 file host
	sends file to host if you have access to it
level10@SnowCrash:~$ ./level10 token test
You don't have access to token
```



**Explanation:**
>- `` 				: 
>- `` 				: 
>- `` 				: 
>- `` 				: 

## 2. 

```bash
level10@SnowCrash:~$ strings ./level10 
/lib/ld-linux.so.2
__gmon_start__
libc.so.6
_IO_stdin_used
socket
fflush
exit
htons
connect
puts
__stack_chk_fail
printf
__errno_location
read
stdout
inet_addr
open
access
strerror
__libc_start_main
write
GLIBC_2.4
GLIBC_2.0
PTRh
UWVS
[^_]
%s file host
	sends file to host if you have access to it
Connecting to %s:6969 .. 
Unable to connect to host %s
.*( )*.
Unable to write banner to host %s
Connected!
Sending file .. 
Damn. Unable to open file
Unable to read from file: %s
wrote file!
You don't have access to %s
;*2$"
[...]
```

```
Connecting to %s:6969 .. 
```


## 3. 

```bash
```

## 4. 


## 5. 

```
```

## 6. Get the flag

```bash
> su flag10
Password: 
Don't forget to launch getflag !

> getflag
Check flag.Here is your token : XXX
```
