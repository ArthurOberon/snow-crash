# LEVEL01

## 1. Find all the files that contains `flag01`

```bash
>find / -type f -exec grep -H "flag01" {} + 2>/dev/null
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
- find							: command used to search for files.
- /								: search starting point (root directory).
- -type f						: search onlyfor files.
- -exec grep -H "flag01" {}		: exec \`grep -H "flag01"\` on each file found.
- \+ 							: terminates the exec command to run it efficiently on multiple files.
- 2>/dev/null					: redirects error messages to \`/dev/null\` to avoid clutter from permission errors.

## 2. Inspect output

We found two files, `/etc/passwd` and `/rofs/etc/passwd`, which contain strings with potentials tokens.

```
/etc/passwd:flag01:42hDRfypTqqnw:3001:3001::/home/flag/flag01:/bin/bash

/rofs/etc/passwd:flag01:42hDRfypTqqnw:3001:3001::/home/flag/flag01:/bin/bash
```

From these strings we can extract the token:
```
42hDRfypTqqnw
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
> su flagXX
Password: 
Don't forget to launch getflag !

> getflag
Check flag.Here is your token : XXX
```
