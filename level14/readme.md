# LEVEL14

## 1. 

```bash
level14@SnowCrash:~$ ls -l
total 0
```

TEXT

**Explanation:**
>- `` 				: 
>- `` 				: 
>- `` 				: 
>- `` 				: 

## 2. 

```bash
level14@SnowCrash:~$ strings /bin/getflag
/lib/ld-linux.so.2
__gmon_start__
libc.so.6
_IO_stdin_used
__stack_chk_fail
strdup
stdout
fputc
fputs
getenv
stderr
getuid
ptrace
fwrite
open
__libc_start_main
GLIBC_2.4
GLIBC_2.0
PTRh@
QVhF
UWVS
[^_]
0123456
You should not reverse this
LD_PRELOAD
Injection Linked lib detected exit..
/etc/ld.so.preload
/proc/self/maps
/proc/self/maps is unaccessible, probably a LD_PRELOAD attempt exit..
libc
Check flag.Here is your token : 
You are root are you that dumb ?
I`fA>_88eEd:=`85h0D8HE>,D
7`4Ci4=^d=J,?>i;6,7d416,7
<>B16\AD<C6,G_<1>^7ci>l4B
B8b:6,3fj7:,;bh>D@>8i:6@D
?4d@:,C>8C60G>8:h:Gb4?l,A
G8H.6,=4k5J0<cd/D@>>B:>:4
H8B8h_20B4J43><8>\ED<;j@3
78H:J4<4<9i_I4k0J^5>B1j`9
bci`mC{)jxkn<"uD~6%g7FK`7
Dc6m~;}f8Cj#xFkel;#&ycfbK
74H9D^3ed7k05445J0E4e;Da4
70hCi,E44Df[A4B/J@3f<=:`D
8_Dw"4#?+3i]q&;p6 gtw88EC
boe]!ai0FB@.:|L6l@A?>qJ}I
g <t61:|4_|!@IF.-62FH&G~DCK/Ekrvvdwz?v|
Nope there is no token here for you sorry. Try again :)
00000000 00:00 0
LD_PRELOAD detected through memory maps exit ..
;*2$"$
```

## 3. Bypass with `gdb`

```bash
```

gdb getflag -> break sur uid (et d'autres protection).

## 4. Understand The Partner And Decrypt

Remarque que meme lettre crypte meme lettre decrypte (`I` 1er caracter du 00 et dernier du 13 -> `x`).
Code program cree tableau de ca, avec option de reverse.

## 5. 

```
```
