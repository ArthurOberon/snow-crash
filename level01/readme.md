# LEVEL01

## 1. Find all the files that contains `flag01`

```bash
> find / -type f -exec grep -H "flag01" {} + 2>/dev/null
/etc/group:flag:x:1001:flag00,flag01,flag02,flag03,flag04,flag05,flag06,flag07,flag08,flag09,flag10,flag11,flag12,flag13,flag14
/etc/group:flag01:x:3001:
/etc/passwd:flag01:42hDRfypTqqnw:3001:3001::/home/flag/flag01:/bin/bash
Binary file /proc/3096/task/3096/cmdline matches
Binary file /proc/3096/cmdline matches
/rofs/etc/group:flag:x:1001:flag00,flag01,flag02,flag03,flag04,flag05,flag06,flag07,flag08,flag09,flag10,flag11,flag12,flag13,flag14
/rofs/etc/group:flag01:x:3001:
/rofs/etc/passwd:flag01:42hDRfypTqqnw:3001:3001::/home/flag/flag01:/bin/bash
```

**Explanation:**
>- `find`								: command used to search for files.
>- `/`									: search starting point (root directory).
>- `-type f`							: search onlyfor files.
>- `-exec grep -H "flag01" {}`			: exec \`grep -H "flag01"\` on each file found.
>- `\+` 								: terminates the exec command to run it efficiently on multiple files.
>- `2>/dev/null`						: redirects error messages to \`/dev/null\` to avoid clutter from permission errors.

## 2. Inspect output

two files contain potentials tokens : `/etc/passwd` and `/rofs/etc/passwd`.

```
/etc/passwd:flag01:42hDRfypTqqnw:3001:3001::/home/flag/flag01:/bin/bash

/rofs/etc/passwd:flag01:42hDRfypTqqnw:3001:3001::/home/flag/flag01:/bin/bash
```

The potential token is embedded in the second field:
```
42hDRfypTqqnw
```

## 3. Recover the token

```bash
> scp -P 4243 level01@127.0.0.1:/etc/passwd .
```

Securely copies the /etc/passwd file from the VM to the host machine.

**Explanation:**
>- `scp` 								: secure copy command, used to transfer files between systems over SSH.
>- `-P 4243`							: specify the custom SSH port (4243).
>- `level01@127.0.0.1:/etc/passwd`	 	: path to the source file on the VM.
>- `.`									: destination directory on the host.


## 4. Crack the token

```bash
> docker run --rm -it --name snow-crash-flag01 -v $(pwd):/snow-scrach debian bash
root@d89462df5ce6:/# apt update; apt install -y john
[...]
root@d89462df5ce6:/# john --show snow-scrach/passwd
[...]
flag01:abcdefg:3001:3001::/home/flag/flag01:/bin/bash
```

Use Docker to create an isolated environment to run John the Ripper.
John the Ripper is a high-performance password-cracking tool.

**Explanation:**
> - `docker run	`							: run a new Docker container.
> - `--rm`									: automatically remove the container after it exits.
> - `-it`									: run in interactive terminal mode.
>- `--name snow-crash-flag01`				: assign a name to the container.
>- `-v $(pwd):/snow-scrach`					: mount current directory into `/snow-crash` in the container.
>- `debian bash`							: run a Debian image with a bash shell

>- `apt update; apt install -y john`		: install John the Ripper inside the container.

>- `john --show snow-scrash/passwd`	 		: cracked passwords and display it.

The command successfully reveals:
```
flag01:abcdefg:3001:3001::/home/flag/flag01:/bin/bash
```

From this output we can extract the token:
```
abcdefg
```

## 5. Get the flag

```bash
> su flag01
Password: 
Don't forget to launch getflag !

> getflag
Check flag.Here is your token : XXX
```
