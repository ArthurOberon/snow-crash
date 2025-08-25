# LEVEL09

## 1. Inspect The Home Directory

```bash
level09@SnowCrash:~$ ls -l
total 12
-rwsr-sr-x 1 flag09 level09 7640 Mar  5  2016 level09
----r--r-- 1 flag09 level09   26 Mar  5  2016 token
level09@SnowCrash:~$ ./level09 
You need to provied only one arg.
level09@SnowCrash:~$ cat token 
f4kmm6p|=�p�n��DB�Du{��
level09@SnowCrash:~$ ./level09 token 
tpmhr
```

## 2. Create Reverse Program


```c
#include "stdio.h"

int main(int argc, char **argv)
{
	if (argc != 2 || argv[1] == NULL)
		return 1;

	int i = -1;
	while (argv[1][++i])
	{
		printf("%c", argv[1][i] - i);
	}
	return 0;
	printf("\n");
}
```


## 3.a Reverse The Token from the VM

```bash
level09@SnowCrash:~$ cd /tmp/
level09@SnowCrash:~$ vim reverse.c
level09@SnowCrash:~$ gcc /tmp/reverse.c -o /tmp/reverse
level09@SnowCrash:/tmp$ ./reverse $(cat ~/token)
f3iji1ju5yuevaus41q1afiuq
```

**Explanation:**
>- `vim`				: 
>- `gcc` 				: 
>- `$(cat token)` 		: 



## 3.b Reverse The Token from the host

```bash
host@pc:> scp -P 4243 level09@127.0.0.1:~/token .
host@pc:> chmod +x token
host@pc:> code reverse.c
host@pc:> gcc reverse.c -o reverse
host@pc:> ./reverse $(cat token)
f3iji1ju5yuevaus41q1afiuq
```

**Explanation:**
>- `scp` 								: secure copy command, used to transfer files between systems over SSH.
>- `-P 4243`							: specify the custom SSH port (4243).
>- `level01@127.0.0.1:/etc/passwd`	 	: path to the source file on the VM.
>- `.`									: destination directory on the host.

>- `chmod +x file`						: makes the file executable.

>- `gcc` 								: 
>- `$(cat token)` 						: 

## 4. Get the flag

```bash
> su flag09
Password: 
Don't forget to launch getflag !

> getflag
Check flag.Here is your token : XXX
```
