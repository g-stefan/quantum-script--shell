# Quantum Script Extension Shell — Documentation

`quantum-script--shell` is the **operating system extension of Quantum
Script**. Loaded with `Script.requireExtension("Shell")`, it adds a `Shell`
object to scripts with the everyday work of a shell script or a build
script, one API for Windows and Linux:

- **Files**: read or write a whole file (`fileGetContents`,
  `filePutContents`, and `Buffer` variants), test (`fileExists`,
  `getFileSize`, `compareLastWriteTime`), copy, rename, remove, touch.
- **Directories**: create (`mkdirRecursivelyIfNotExists`, `mkdirFilePath`),
  list with wildcards (`getFileList`, `getDirList`), copy and remove whole
  trees (`copyDirRecursively`, `removeDirRecursively`, `...Force`).
- **Path names**: split a name into folder, file name and extension
  (`getFilePath`, `getFileName`, `getFileExtension`, ...), the current
  directory (`getcwd`, `chdir`), absolute paths (`realPath`).
- **Processes**: run a command line through the system shell
  (`system`), run a program and wait (`execute`), start in the background
  (`executeNoWait`) and watch or stop it (`isProcessTerminated`,
  `terminateProcess`).
- **Environment**: `getenv`, `setenv`, `hasEnv`, `isEnv`.

Every function is a thin wrapper over `XYO::System::Shell` from
`xyo-system`. Arguments are converted with `toString` / `toNumber`; failures
are **return values** (`false`, `undefined`, `-1`, `0`), never exceptions.

```
scripts: fabricare build scripts, quantum-script .js, ...
quantum-script--shell      <-- this extension: the Shell object
quantum-script--shellfind  (ShellFind, directory iterator, loaded automatically)
quantum-script--buffer     (Buffer, for fileGetContentsBuffer / filePutContentsBuffer, loaded automatically)
quantum-script             (Executive, Variable, Context)
xyo-system                 (XYO::System::Shell, ShellFind, File)
xyo-encoding, xyo-multithreading, xyo-data-structures, xyo-managed-memory, xyo-platform
```

## Why it exists

Quantum Script has no file system or process access of its own. `Shell` is
what makes it usable as a scripting language for builds, installers and
tools: `fabricare` build scripts (`make.js`, `install.js`, `release.js`, ...)
are mostly `Shell` calls.

| Need | What `Shell` gives |
|------|--------------------|
| Read a config, write a generated source | `fileGetContents` / `filePutContents` |
| Prepare output folders | `mkdirRecursivelyIfNotExists`, `mkdirFilePath` |
| Copy build results into an SDK / release folder | `copyFile`, `copyFilesToDirectory`, `copyDirRecursively` |
| Clean | `removeFile`, `removeDirRecursively`, `removeDirRecursivelyForce` |
| Rebuild only when sources are newer | `compareLastWriteTime`, `fileExists` |
| Run a compiler, `git`, `7z`, ... and check the exit code | `system`, `execute` |
| Start a server, wait, stop it | `executeNoWait`, `isProcessTerminated`, `terminateProcess` |
| Pass settings to child processes | `setenv`, `getenv`, `hasEnv` |

For streaming a file (line by line, appending, seeking) use `File` from
`quantum-script--file`; for walking a directory with file attributes use
`ShellFind` from `quantum-script--shellfind`.

## Concepts at a glance

| Need | Use | Notes |
|------|-----|-------|
| Load the extension | `Script.requireExtension("Shell");` | also loads `Buffer` and `ShellFind` |
| Whole file as text | `Shell.fileGetContents(file)` | `undefined` if it cannot be read |
| Write a file | `Shell.filePutContents(file, text)` | creates / truncates; the folder must exist |
| Make sure a folder exists | `Shell.mkdirRecursivelyIfNotExists(path)` | `true` if it exists already |
| Folder of a file to write | `Shell.mkdirFilePath(file)` | then `filePutContents` |
| Copy one file | `Shell.copyFile(src, dst)` | creates the folder of `dst`, overwrites |
| List | `Shell.getFileList("dir/*.cpp")`, `Shell.getDirList("dir/*")` | not recursive, `[]` if none |
| Remove a tree | `Shell.removeDirRecursively(dir)` | `...Force` also removes read-only entries |
| Shell command line | `Shell.system("git status > out.txt")` | exit code; pipes, redirection, built-ins |
| Program, no shell | `Shell.execute("program arg")` | exit code; `127` if it cannot start (Windows) |
| Background | `var id = Shell.executeNoWait(cmd);` | `0` if it cannot start |
| Environment | `Shell.getenv(name)`, `Shell.setenv(name, value)` | `""` when not set: test with `hasEnv` |
| Path parts | `getFilePath`, `getFileName`, `getFileExtension`, `getFileBasename` | string operations only |
| Which platform | `Shell.is("win")`, `Shell.is("unix")` | |
| Fill a template | `Shell.fileReplaceText(in, out, [["$NAME", "value"]], 0)` | line by line, every occurrence; `out` must differ from `in` |

## Contents

| Document | What it covers |
|----------|----------------|
| [Getting started](getting-started.md) | Build and install, load the extension from a script, fabricare scripts, register it in a C++ host, static builds, threads |
| [Files and directories](files-and-directories.md) | Path names, relative paths, what counts as a file, reading and writing, copying, removing, listing with wildcards, links, read-only entries, platform differences |
| [Processes and environment](processes-and-environment.md) | `system` versus `execute`, exit codes, background processes, stopping them, environment variables |
| [Script API](script-api.md) | Every function: arguments, exact behavior, edge cases, return values |
| [Recipes](recipes.md) | Generated files, clean, copy to an SDK, incremental builds, running tools, servers, templates |
| [C++ API](cpp-api.md) | `registerInternalExtension`, `initExecutive`, the `XYO::System::Shell` function behind each script function, notes for maintainers |
| [API reference](reference.md) | Every function on one page |

Quantum Script itself (the language, `Script.requireExtension`, embedding,
writing extensions) is documented in the `quantum-script` repository,
`docs/`; the underlying `XYO::System::Shell` in the `xyo-system` repository,
`docs/files.md` and `docs/processes.md`.

## Fixed issues

Builds up to **5.10.0 build 7** have two broken functions. They are fixed
in the sources; an SDK installed before the fix still behaves the old way
until it is rebuilt and installed again:

| Function | Old behavior | Fix |
|----------|--------------|-----|
| `Shell.is(what)` | always `false`: it tested the macros `XYO_OS_TYPE_WIN` / `XYO_OS_TYPE_UNIX`, which nothing defines | tests `XYO_PLATFORM_OS_WINDOWS` / `XYO_PLATFORM_OS_LINUX` |
| `Shell.fileReplaceText(fileIn, fileOut, textInOut, lineMaxLength)` | **never returned**: the line size was read from the third argument (the array), became `0`, and the line loop did not end | reads `lineMaxLength`; `0` or missing means `32768` |

Scripts that must also run on an older SDK can detect Windows with
`Shell.hasEnv("WINDIR")` and replace text with `fileGetContents`,
`String.prototype.replace` and `filePutContents` (see
[Recipes](recipes.md#replace-text-in-a-template)).

## Source map

```
source/XYO/QuantumScript.Extension/Shell.hpp            umbrella header, include this from C++
source/XYO/QuantumScript.Extension/Shell.Amalgam.cpp    the whole extension in one translation unit
source/XYO/QuantumScript.Extension/Shell/
    Dependency.hpp                                      <XYO/QuantumScript.hpp>, export macro
    Library[.hpp/.cpp]                                  initExecutive, registerInternalExtension,
                                                        every native function
    Copyright / License / Version                       library metadata
test/test.01.cpp                                        C++ host registering Console, Buffer, Shell
                                                        and ShellFind as internal extensions
test/test.01.js                                         loads Console and Shell
```

## AI assistant skill

A Claude Code skill describing how to use this extension lives in
[`.claude/skills/quantum-script--shell/`](../.claude/skills/quantum-script--shell/SKILL.md).
It is picked up automatically inside this repository; copy the folder to
`~/.claude/skills/` to have it available in the projects that use `Shell`
(fabricare scripts, Quantum Script tools, other extensions).
