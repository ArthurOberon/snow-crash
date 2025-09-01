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

The binary requires one argument, but when we give it the `token` file, the output is garbled and not the real password. The file `token` is owned by `flag09` and only readable by `level09` (and `flag09`).

```bash
level09@SnowCrash:~$ ./level09 test
tfuw
level09@SnowCrash:~$ ./level09 aaaa
abcd
level09@SnowCrash:~$ ./level09 1111
1234
```

From these tests, it can be deduced that the program adds the position index of each character to its ASCII code.
The encoding logic is a Caesar-like cipher, likely implemented as:
```c
i = 0;
while (str[i])
{
	printf("%c", str[i] + i);
	++i;
}
```

**Explanation of logic (encoding):**
- Each character of the input string is incremented by its position index (0-based).
- Loop through the input string until the null terminator.

## 2. Create Reverse Program

Since the binary applies an encoding (each character shifted by its index), a small C program can reverse the operation:

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

**Explanation of logic (decoding):**
- Each character of the input string is decremented by its position index (0-based).
- Loop through the input string until the null terminator.

## 3.a Reverse The Token From The VM

```bash
level09@SnowCrash:~$ cd /tmp/
level09@SnowCrash:~$ vim reverse.c
level09@SnowCrash:~$ gcc /tmp/reverse.c -o /tmp/reverse
level09@SnowCrash:/tmp$ ./reverse $(cat ~/token)
f3iji1ju5yuevaus41q1afiuq
```

**Explanation:**
- `vim`				: create/edit the source code.
- `gcc` 				: compile the C file into a binary.
- `$(cat token)` 		: command substitution, replaces the expression with the contents of `token`.


## 3.b Reverse The Token From The Host

Alternatively, the `token` file can also be transferred to the host and decoded locally.

```bash
host@pc:> scp -P 4243 level09@127.0.0.1:~/token .
host@pc:> chmod +x token
host@pc:> code reverse.c
host@pc:> gcc reverse.c -o reverse
host@pc:> ./reverse $(cat token)
f3iji1ju5yuevaus41q1afiuq
```

**Explanation:**
- `scp` 								: secure copy command, used to transfer files between systems over SSH.
- `-P 4243`								: specify the custom SSH port (4243).
- `level09@127.0.0.1:~/token`	 		: path to the source file on the VM.
- `.`									: destination directory on the host.

- `code`								: open the file with VS Code.
- `chmod +x file`						: makes the file executable.

- `gcc` 								: compile the program.
- `gcc FILE -o EXECUTABLE` 				: compile the source file `FILE` into an executable named `EXECUTABLE`.

- `$(cat token)` 						: command substitution, replaces the expression with the contents of `token`.

## 4. Get The Flag

```bash
> su flag09
Password: 
Don't forget to launch getflag !

> getflag
Check flag.Here is your token : XXX
```
