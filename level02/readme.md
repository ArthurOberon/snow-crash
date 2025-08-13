# LEVEL02

## 1. Inspect the file

```bash
> ls -l
total 12
----r--r-- 1 flag02 level02 8302 Aug 30  2015 level02.pcap
```

There is a `.pcap` file, which contains captured network traffic.

## 2. Copy the file to the host

```bash
> scp -P 4243 level02@127.0.0.1:~/level02.pcap .
```

**Explanation:**
>- `scp` 								: secure copy command, used to transfer files between systems over SSH.
>- `-P 4243`							: specify the custom SSH port (4243).
>- `level02@127.0.0.1:~/level02.pcap`	 	: path to the source file on the VM.
>- `.`									: destination directory on the host.

## 3. Create Docker environment

```bash
> docker run --rm -it --name snow-crash-flag02 -v $(pwd):/snow-scrach debian bash
root@40d275155c42:/# apt update; apt install -y tshark xxd
[...]
```

**Explanation:**
>- `docker run	`							: run a new Docker container.
>- `--rm`									: automatically remove the container after it exits.
>- `-it`									: run in interactive terminal mode.
>- `--name snow-crash-flag02`				: assign a name to the container.
>- `-v $(pwd):/snow-scrach`					: mount current directory into `/snow-crash` in the container.
>- `debian bash`							: run a Debian image with a bash shell

>- `apt update; apt install -y tshark xxd`	: install `tshark` for packet analysis and `xxd` for hex-to-binary conversion.

## 4. Extract raw TCP payload

```bash
root@9ca1591ece3e:/# tshark -r /snow-scrach/level02.pcap -T fields -e tcp.payload | xxd -r -p
[...]

wwwbugs login: lleevveellXX
Password: ft_wandrNDRelL0L

Login incorrect
```
This command reads the capture file with `tshark`, extracts the TCP payload in hexadecimal, then converts it to ASCII with `xxd`.
The output reveals an interactive login attempt.

**Explanation:**
>- `tshark`							:
>- ` -r /snow-scrach/level02.pcap`	:
>- `-T fields`						:
>- `-e tcp.payload`					:
>- `xxd -r -p`						:


## 5. Step-by-step payload reconstruction

```bash
root@9ca1591ece3e:/# tshark -r /snow-scrach/level02.pcap -T fields -e ip.src -e ip.dst -e tcp.payload
[...]
59.233.235.223	59.233.235.218	000d0a50617373776f72643a20
59.233.235.218	59.233.235.223	
59.233.235.218	59.233.235.223	66
59.233.235.223	59.233.235.218	
59.233.235.218	59.233.235.223	74
59.233.235.223	59.233.235.218	
59.233.235.218	59.233.235.223	5f
59.233.235.223	59.233.235.218	
59.233.235.218	59.233.235.223	77
59.233.235.223	59.233.235.218	
59.233.235.218	59.233.235.223	61
59.233.235.223	59.233.235.218	
59.233.235.218	59.233.235.223	6e
59.233.235.223	59.233.235.218	
59.233.235.218	59.233.235.223	64
59.233.235.223	59.233.235.218	
59.233.235.218	59.233.235.223	72
59.233.235.223	59.233.235.218	
59.233.235.218	59.233.235.223	7f
59.233.235.223	59.233.235.218	
59.233.235.218	59.233.235.223	7f
59.233.235.223	59.233.235.218	
59.233.235.218	59.233.235.223	7f
59.233.235.223	59.233.235.218	
59.233.235.218	59.233.235.223	4e
59.233.235.223	59.233.235.218	
59.233.235.218	59.233.235.223	44
59.233.235.223	59.233.235.218	
59.233.235.218	59.233.235.223	52
59.233.235.223	59.233.235.218	
59.233.235.218	59.233.235.223	65
59.233.235.223	59.233.235.218	
59.233.235.218	59.233.235.223	6c
59.233.235.223	59.233.235.218	
59.233.235.218	59.233.235.223	7f
59.233.235.223	59.233.235.218	
59.233.235.218	59.233.235.223	4c
59.233.235.223	59.233.235.218	
59.233.235.218	59.233.235.223	30
59.233.235.223	59.233.235.218	
59.233.235.218	59.233.235.223	4c
59.233.235.223	59.233.235.218	
59.233.235.218	59.233.235.223	0d
59.233.235.223	59.233.235.218	
59.233.235.223	59.233.235.218	000d0a
59.233.235.218	59.233.235.223	
59.233.235.223	59.233.235.218	01
59.233.235.218	59.233.235.223	
59.233.235.223	59.233.235.218	000d0a4c6f67696e20696e636f72726563740d0a77777762756773206c6f67696e3a20
[...]
```

**Explanation:**
>- `tshark`
>- ` -r /snow-scrach/level02.pcap`	:
>- `-T fields`						:
>- `-e ip.src`						:
>- `-e ip.dst`						:
>- `-e tcp.payload`					:
>-

By following input by input, it's possible to reconstruct the password manually. With a convertor [rapidtable for example](https://www.rapidtables.com/convert/number/hex-to-ascii.html) or the [ASCII table](https://media.geeksforgeeks.org/wp-content/uploads/20240304094301/ASCII-Table.png) convert the hexa value of the inputs.

```
000d0a50617373776f72643a20	| 	Password:		| ---
66 							| 	f				|	f
74 							| 	t				|	ft
5f 							| 	_				|	ft_
77 							| 	w				|	ft_w
61 							| 	a				|	ft_wa
6e 							| 	n				|	ft_wan
64 							| 	d				|	ft_wand
72 							| 	r				|	ft_wandr
7f 							| 	[DEL]			|	ft_wand
7f 							| 	[DEL]			|	ft_wan
7f 							| 	[DEL]			|	ft_wa
4e 							| 	N				|	ft_waN
44 							| 	D				|	ft_waND
52 							| 	R				|	ft_waNDR
65 							| 	e				|	ft_waNDRe
6c 							| 	l				|	ft_waNDRel
7f 							| 	[DEL]			|	ft_waNDRe
4c 							| 	L				|	ft_waNDReL
30 							| 	0				|	ft_waNDReL0
4c 							| 	L				|	ft_waNDReL0L
0d 							| 	\r				|	[ENTER]
000d0a 						| 	NULL\r\n		|	[ENTER]
```

The password is :
```
ft_waNDReL0L
```


## 6. Get the flag

```bash
> su flag02
Password: 
Don't forget to launch getflag !

> getflag
Check flag.Here is your token : XXX
```
