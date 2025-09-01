# LEVEL00

## 1. Find All The Files Owned By `flag00`

```bash
level00@SnowCrash:~$ find / -user flag00 2>/dev/null
/usr/sbin/john
/rofs/usr/sbin/john
```

**Explanation:**
- `find`				: command used to search for files.
- `/`					: search starting point (root directory).
- `-user flag00`		: filters results to only files owned by \`flag00\`.
- `2>/dev/null`			: redirects error messages to \`/dev/null\` to avoid clutter from permission errors.

## 2. Inspect The Files

```bash
level00@SnowCrash:~$ ls -l /usr/sbin/john /rofs/usr/sbin/john 
----r--r-- 1 flag00 flag00 15 Mar  5  2016 /usr/sbin/john
----r--r-- 1 flag00 flag00 15 Mar  5  2016 /rofs/usr/sbin/john
```

Both files are owned by \`flag00\`.

## 3. Read The Files

```bash
level00@SnowCrash:~$ cat /usr/sbin/john /rofs/usr/sbin/john
cdiiddwpgswtgt
cdiiddwpgswtgt
```

Both files contain the same encrypted string:
```bash
cdiiddwpgswtgt
```

## 4. Analyze The Code

Using the [Dcode's cipher identifier](https://www.dcode.fr/identification-chiffrement) the encryption is likely an **Affine cipher** or a **Caesar cipher**. Both are **shift ciphers**, and in this case, the key corresponds to a **shift of 15**.

## 5. Decrypt The Code

Using [Affine cipher decryption](https://www.dcode.fr/chiffre-affine) or [Caesar cipher decryption](https://www.dcode.fr/chiffre-cesar) with a shift of 15, the encrypted text decrypts to :

```
nottoohardhere
```

## 6. Get The Flag

```bash
level00@SnowCrash:~$ su flag00
Password: 
Don't forget to launch getflag !

flag00@SnowCrash:~$ getflag
Check flag.Here is your token : XXX
```
