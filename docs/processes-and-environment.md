# Processes and environment

## Four ways to run a command

| Function | Through the shell | Waits | Returns |
|----------|-------------------|-------|---------|
| `Shell.system(cmd)` | **yes**, both platforms | yes | exit code |
| `Shell.execute(cmd)` / `Shell.executeHidden(cmd)` | Windows: **no**; Linux: yes | yes | exit code |
| `Shell.executeNoWait(cmd)` / `Shell.executeHiddenNoWait(cmd)` | **no**, both platforms | no | process id, `0` if it cannot start |

### `Shell.system(cmd)`

The C `system()`: the command line goes to `cmd.exe /c` on Windows and to
`/bin/sh -c` on Linux. Everything a shell does works: pipes,
redirection, `&&` / `||`, built-in commands (`echo`, `dir`, `cd`, `copy`),
`.cmd` / `.bat` scripts, variable expansion (`%VAR%` / `$VAR`).

```javascript
if (Shell.system("git rev-parse HEAD > temp/commit.txt") != 0) {
	throw "git failed";
};
var commit = Shell.fileGetContents("temp/commit.txt").trim();
```

The quoting rules are the ones of that shell, so command lines that use
shell syntax are not portable; keep them simple or branch on the platform.

### `Shell.execute(cmd)`

- **Windows**: `CreateProcess` directly, **no shell**. The first word is the
  program (found next to the executable, in the current folder, the system
  folders and `PATH`; `.exe` is added when there is no extension), the rest
  of the line is passed to it as is. Built-ins, redirection, pipes and
  `.cmd` / `.bat` scripts need `cmd /c ...` (or `Shell.system`). Returns
  `127` when the program cannot be started.
- **Linux**: the same as `Shell.system`.

`executeHidden` is the same with the window hidden (Windows `SW_HIDE`),
useful for programs that open their own window; on Linux it is `execute`.

Use `execute` on Windows to avoid the `cmd.exe` quoting rules when running a
program with quoted arguments, or to start a GUI program and wait for it.

### Exit codes

- The process exit code, `0` usually meaning success.
- Linux: a process killed by a signal gives `128 + signal` (like a shell);
  `system` returns `-1` if the shell itself cannot be started.
- Windows `execute`: `127` if the program cannot be started.
- A command not found by the shell gives the shell's code: `127` on Linux,
  `1` from `cmd.exe`.

### Output

The child shares the console (standard input, output and error) of the
script. To capture its output, redirect to a file with `Shell.system` and
read it back (`fileGetContents`), or use `ProcessInteractive`
(`quantum-script--processinteractive`) for pipes.

The script's own `Console` output may be buffered while the child writes
directly, so when the output is redirected the lines can appear out of
order.

## Background processes

```javascript
var id = Shell.executeNoWait("my-server --port 8080");
if (id == 0) {
	throw "cannot start my-server";
};
// ... work with the server ...
Shell.terminateProcess(id, 3000);
```

- `executeNoWait(cmd)` starts the program and returns its **process id**,
  or `0` if it could not be started. There is **no shell** on either
  platform: Windows uses `CreateProcess` (as `execute`), Linux splits the
  line into arguments (double quotes group words) and runs the program with
  `posix_spawnp`, searching `PATH`. Use `cmd /c ...` / `sh -c "..."` when
  you need a shell.
- `executeHiddenNoWait(cmd)`: the same with the window hidden (Windows);
  on Linux it is `executeNoWait`.
- The exit code of a background process is not available.

### `Shell.isProcessTerminated(id)`

`true` when the process has ended, also for `0` and for ids that are not
running. On Linux it also collects the ended child (no zombie is left);
call it until it returns `true` for every child you started. On Windows
the handle of a process started by `executeNoWait` is kept until its end is
seen, so its id cannot be reused by another process in the meantime.

```javascript
var id = Shell.executeHiddenNoWait("long-job");
while (!Shell.isProcessTerminated(id)) {
	CurrentThread.sleep(100);    // quantum-script--thread, or do other work
};
```

### `Shell.terminateProcess(id, timeout)`

Asks the process to stop, waits up to `timeout` milliseconds, then kills it:

- **Windows**: `WM_CLOSE` to its top level windows, wait, then
  `TerminateProcess`. A console program has no window, so it is killed when
  the time is up.
- **Linux**: `SIGTERM`, wait, then `SIGKILL`.
- `timeout` `0` kills at once.

Returns `true` when the process has ended (or was not running), `false` if
it cannot be stopped (no permission). Only that process is stopped, not the
processes **it** started: stopping the `cmd.exe` of `executeNoWait("cmd /c x")`
leaves `x` running.

## Environment variables

| Function | Result |
|----------|--------|
| `Shell.getenv(name)` | the value, **`""` when not set** |
| `Shell.hasEnv(name)` | `true` if the variable is set (even to `""` on Linux) |
| `Shell.isEnv(name, value)` | `true` if set and exactly equal to `value` (case-sensitive) |
| `Shell.setenv(name, value)` | sets it for this process; `true` / `false` |

- `getenv` cannot tell "not set" from "empty": test with `hasEnv`.
- Variable names are case-insensitive on Windows (`getenv("path")` is
  `PATH`), case-sensitive on Linux.
- `setenv` changes the environment of the **whole process**; processes
  started afterwards (`system`, `execute`, `executeNoWait`) inherit it.
  It is the way to pass settings to tools:

```javascript
Shell.setenv("PATH", Shell.getenv("PATH") + (Shell.is("win") ? ";" : ":") + Shell.realPath("bin"));
Shell.setenv("CC", "gcc");
Shell.system("make");
```

- On Windows `setenv(name, "")` **removes** the variable
  (`hasEnv` becomes `false`); on Linux it sets it to an empty value.

## Which platform

```javascript
if (Shell.is("win")) {          // Windows, MinGW builds too
	Shell.system("cmd /c ver");
};
if (Shell.is("unix")) {         // Linux, Emscripten builds too
	Shell.system("uname -a");
};
```

The answer is fixed when the extension is compiled. Builds up to 5.10.0
build 7 always return `false` (see [Script API](script-api.md#shelliswhat));
for scripts that must run there too, `Shell.hasEnv("WINDIR")` (set by
Windows for every process) is a portable test. In fabricare scripts
`OS.isWindows()`, `OS.isLinux()`, `OS.isMinGW()`, `OS.isEmscripten()` also
tell the toolchains apart.
