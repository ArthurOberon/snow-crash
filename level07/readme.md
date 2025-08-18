# LEVELXX

## 1. 

```bash
level07@SnowCrash:~$ ls -l
total 12
-rwsr-sr-x 1 flag07 level07 8805 Mar  5  2016 level07
level07@SnowCrash:~$ ./level07 
level07
level07@SnowCrash:~$ ./level07 a
level07
level07@SnowCrash:~$ strings level07 
/lib/ld-linux.so.2
zE&9qU
__gmon_start__
libc.so.6
_IO_stdin_used
setresgid
asprintf
getenv
setresuid
system
getegid
geteuid
__libc_start_main
GLIBC_2.0
PTRh 
UWVS
[^_]
LOGNAME
/bin/echo %s 
;*2$"
GCC: (Ubuntu/Linaro 4.6.3-1ubuntu5) 4.6.3
	gid
	uid
/home/user/level07
/usr/include/i386-linux-gnu/bits
/usr/include
level07.c
types.h
unistd.h
long long int
/home/user/level07/level07.c
__uid_t
envp
long long unsigned int
setresuid
getenv
setresgid
unsigned char
system
GNU C 4.6.3
argc
__gid_t
short unsigned int
main
asprintf
short int
buffer
argv
.symtab
.strtab
.shstrtab
.interp
.note.ABI-tag
.note.gnu.build-id
.gnu.hash
.dynsym
.dynstr
.gnu.version
.gnu.version_r
.rel.dyn
.rel.plt
.init
.text
.fini
.rodata
.eh_frame_hdr
.eh_frame
.ctors
.dtors
.jcr
.dynamic
.got
.got.plt
.data
.bss
.comment
.debug_aranges
.debug_info
.debug_abbrev
.debug_line
.debug_str
.debug_loc
crtstuff.c
__CTOR_LIST__
__DTOR_LIST__
__JCR_LIST__
__do_global_dtors_aux
completed.6159
dtor_idx.6161
frame_dummy
__CTOR_END__
__FRAME_END__
__JCR_END__
__do_global_ctors_aux
level07.c
__init_array_end
_DYNAMIC
__init_array_start
_GLOBAL_OFFSET_TABLE_
__libc_csu_fini
setresuid@@GLIBC_2.0
__i686.get_pc_thunk.bx
data_start
_edata
_fini
geteuid@@GLIBC_2.0
getegid@@GLIBC_2.0
__DTOR_END__
getenv@@GLIBC_2.0
__data_start
system@@GLIBC_2.0
__gmon_start__
__dso_handle
_IO_stdin_used
__libc_start_main@@GLIBC_2.0
__libc_csu_init
_end
_start
_fp_hw
asprintf@@GLIBC_2.0
__bss_start
main
_Jv_RegisterClasses
setresgid@@GLIBC_2.0
_init
level07@SnowCrash:~$ env
SHELL=/bin/bash
TERM=xterm-256color
SSH_CLIENT=10.0.2.2 41478 4242
SSH_TTY=/dev/pts/0
USER=level07
LS_COLORS=rs=0:di=01;34:ln=01;36:mh=00:pi=40;33:so=01;35:do=01;35:bd=40;33;01:cd=40;33;01:or=40;31;01:su=37;41:sg=30;43:ca=30;41:tw=30;42:ow=34;42:st=37;44:ex=01;32:*.tar=01;31:*.tgz=01;31:*.arj=01;31:*.taz=01;31:*.lzh=01;31:*.lzma=01;31:*.tlz=01;31:*.txz=01;31:*.zip=01;31:*.z=01;31:*.Z=01;31:*.dz=01;31:*.gz=01;31:*.lz=01;31:*.xz=01;31:*.bz2=01;31:*.bz=01;31:*.tbz=01;31:*.tbz2=01;31:*.tz=01;31:*.deb=01;31:*.rpm=01;31:*.jar=01;31:*.war=01;31:*.ear=01;31:*.sar=01;31:*.rar=01;31:*.ace=01;31:*.zoo=01;31:*.cpio=01;31:*.7z=01;31:*.rz=01;31:*.jpg=01;35:*.jpeg=01;35:*.gif=01;35:*.bmp=01;35:*.pbm=01;35:*.pgm=01;35:*.ppm=01;35:*.tga=01;35:*.xbm=01;35:*.xpm=01;35:*.tif=01;35:*.tiff=01;35:*.png=01;35:*.svg=01;35:*.svgz=01;35:*.mng=01;35:*.pcx=01;35:*.mov=01;35:*.mpg=01;35:*.mpeg=01;35:*.m2v=01;35:*.mkv=01;35:*.webm=01;35:*.ogm=01;35:*.mp4=01;35:*.m4v=01;35:*.mp4v=01;35:*.vob=01;35:*.qt=01;35:*.nuv=01;35:*.wmv=01;35:*.asf=01;35:*.rm=01;35:*.rmvb=01;35:*.flc=01;35:*.avi=01;35:*.fli=01;35:*.flv=01;35:*.gl=01;35:*.dl=01;35:*.xcf=01;35:*.xwd=01;35:*.yuv=01;35:*.cgm=01;35:*.emf=01;35:*.axv=01;35:*.anx=01;35:*.ogv=01;35:*.ogx=01;35:*.aac=00;36:*.au=00;36:*.flac=00;36:*.mid=00;36:*.midi=00;36:*.mka=00;36:*.mp3=00;36:*.mpc=00;36:*.ogg=00;36:*.ra=00;36:*.wav=00;36:*.axa=00;36:*.oga=00;36:*.spx=00;36:*.xspf=00;36:
PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games
MAIL=/var/mail/level07
PWD=/home/user/level07
LANG=en_US.UTF-8
HOME=/home/user/level07
SHLVL=2
LOGNAME=level07
SSH_CONNECTION=10.0.2.2 41478 10.0.2.15 4242
LESSOPEN=| /usr/bin/lesspipe %s
LESSCLOSE=/usr/bin/lesspipe %s %s
OLDPWD=/home/user/level06
_=/usr/bin/env
level07@SnowCrash:~$ LOGNAME='$(getflag)'
level07@SnowCrash:~$ ./level07 
Check flag.Here is your token : XXX
```

**Explanation:**
- 				: 
- 				: 
- 		: 
- 		:

## 2. 

```bash

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
