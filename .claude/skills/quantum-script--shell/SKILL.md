---
name: quantum-script--shell
description: >-
  How to use the Quantum Script Shell extension (quantum-script--shell), the
  operating system object loaded with Script.requireExtension("Shell")
  (which also loads Buffer and ShellFind), used by every fabricare build
  script: whole files (fileGetContents / filePutContents, the Buffer
  variants), tests (fileExists, directoryExists, isEmptyDir, getFileSize,
  compareLastWriteTime), path names (getFileName, getFileExtension,
  getFileBasename which keeps the folder, getFilePath, getFilePathX,
  realPath, getcwd, chdir), files (copy, copyFile, rename, remove,
  removeFile, removeFileForce, touch), directories (mkdir,
  mkdirRecursivelyIfNotExists, mkdirFilePath, rmdir, removeDirRecursively,
  removeDirRecursivelyForce, removeEmptyDirRecursively,
  removeFileRecursively, copyDirRecursively, copyFilesToDirectory,
  getFileList / getDirList with * ? wildcards), processes (system through
  the shell, execute without a shell on Windows, executeNoWait,
  isProcessTerminated, terminateProcess, exit codes), environment (getenv
  returns "" when unset, setenv, hasEnv, isEnv), Shell.is("win" / "unix"),
  template filling with Shell.fileReplaceText (both broken up to 5.10.0
  build 7), and the C++ side (registerInternalExtension,
  initExecutive, the XYO::System::Shell functions behind it). Use when
  writing or reviewing Quantum Script or fabricare .js code that touches
  files, folders, processes or environment variables through Shell, C++
  code that includes <XYO/QuantumScript.Extension/Shell.hpp>, a
  fabricare.json depending on "quantum-script--shell", or when working
  inside the quantum-script--shell repository.
---

# quantum-script--shell

Operating system extension of Quantum Script (see the `quantum-script` skill
for the language and its differences from JavaScript, the `xyo-system`
skill for the underlying `XYO::System::Shell`, and the `fabricare` skill for
build scripts; their rules apply). Purpose: **let scripts do what a shell
or build script does** — read and write whole files, create, copy, list and
remove files and folder trees, split path names, run programs and check
their exit codes, start and stop background processes, read and set
environment variables — one API for Windows and Linux.

Full documentation: `docs/` in the quantum-script--shell repository
(`X:\Storage\XYO\Gitea\CPP\quantum-script--shell\docs` on this machine):
README, getting-started, **files-and-directories** (names, tests, copy,
remove, listing, links, platform differences), **processes-and-environment**
(`system` vs `execute`, exit codes, background processes, env),
**script-api** (exact behavior of every function), **recipes**, cpp-api,
reference. Read the matching page when you need more than this summary.
When in doubt read `source/XYO/QuantumScript.Extension/Shell/Library.cpp`
(~580 lines, each function a one-line wrapper) and `xyo-system`
`source/XYO/System/Shell.cpp`, `Shell-OS-Windows.cpp`, `Shell-OS-Linux.cpp`.

## Script API

```javascript
Script.requireExtension("Shell");        // also Buffer + ShellFind; fabricare registers it as internal

// whole files (binary, no line end translation)
Shell.fileGetContents(file);             // String, undefined if unreadable ("" for an empty file)
Shell.filePutContents(file, text);       // create / TRUNCATE; folder must exist -> Boolean
Shell.fileGetContentsBuffer(file);       // Buffer (length = size), undefined
Shell.filePutContentsBuffer(file, buf);  // Boolean; undefined if buf is not a Buffer
Shell.fileReplaceText(fileIn, fileOut, [["$A", "x"], ["$B", "y"]], 0);   // line by line, pairs in order,
                                         // every occurrence; 0 = 32768 line size; fileOut != fileIn

// tests
Shell.fileExists(name);                  // regular file (links followed); false for directories
Shell.directoryExists(name);             // directory; "C:" -> "C:\"
Shell.isEmptyDir(name);                  // TRUE ALSO FOR A MISSING DIRECTORY
Shell.getFileSize(file);                 // bytes, -1 missing (Windows: -1 for a directory)
Shell.compareLastWriteTime(a, b);        // -1 a older / 0 / 1; a missing file is older
Shell.is("win"); Shell.is("unix");       // compile time: Windows (+MinGW) / Linux (+Emscripten)

// path names (string only; Windows accepts / and \)
Shell.getFileName("a/b/c.tar.gz");       // "c.tar.gz"
Shell.getFileExtension("a/b/c.tar.gz");  // "gz"     (".gitignore" -> "gitignore")
Shell.getFileBasename("a/b/c.tar.gz");   // "a/b/c.tar"  FOLDER KEPT
Shell.getFilePath("a/b/c.tar.gz");       // "a/b"    ("" if none)
Shell.getFilePathX("a/b/c.tar.gz");      // "a/b/"   ("" if none)
Shell.realPath(p);                       // absolute; Linux: undefined if missing
Shell.getcwd(); Shell.chdir(path);       // chdir is PROCESS WIDE

// files
Shell.copy(src, dst);                    // overwrites; folder of dst must exist
Shell.copyFile(src, dst);                // creates the folder of dst, then copy
Shell.rename(src, dst);                  // Windows: FAILS if dst exists; Linux: replaces
Shell.remove(name);                      // file or link (Linux: also an empty dir)
Shell.removeFile(file); Shell.removeFileForce(file);   // only if fileExists; Force clears read-only
Shell.removeFileAndDirectoryIfEmpty(file);             // + its folder if now empty (one level)
Shell.touch(file); Shell.touchIfExists(file);          // touch creates, never truncates

// directories
Shell.mkdir(path);                       // one level, false if it exists
Shell.mkdirRecursively(path);            // all levels, true if exists
Shell.mkdirRecursivelyIfNotExists(path); // THE ONE TO USE; false if path is a file
Shell.mkdirFilePath(file);               // folder that will hold file; true if none
Shell.rmdir(path); Shell.rmdirForce(path); Shell.removeEmptyDir(dir);
Shell.removeEmptyDirRecursively(path);   // all empty sub folders, then path; true if path removed
Shell.removeDirContentRecursively(path); // keeps path; true if path missing
Shell.removeDirRecursively(path);        // false if path missing
Shell.removeDirContentRecursivelyForce(path); Shell.removeDirRecursivelyForce(path);   // + read-only
Shell.removeFileRecursively(dir, pattern);             // matching files in the whole tree
Shell.copyDirRecursively(src, dst);      // CONTENT of src into dst, merge, overwrite
Shell.copyFilesToDirectory(src, dst);    // files of "dir" or "dir/*.dll"; not recursive
Shell.getFileList("dir/*.cpp");          // ["dir/a.cpp", ...] files; not recursive; [] if none
Shell.getDirList("dir/*");               // directories without . ..; getDirList("dir") -> ["dir"]

// processes
Shell.system(cmd);                       // cmd.exe /c | /bin/sh -c; waits; exit code
Shell.execute(cmd);                      // Windows: CreateProcess, NO SHELL, 127 = cannot start; Linux = system
Shell.executeHidden(cmd);                // execute with hidden window (Windows)
var id = Shell.executeNoWait(cmd);       // no shell (both); process id, 0 = cannot start
Shell.executeHiddenNoWait(cmd);
Shell.isProcessTerminated(id);           // true when ended / unknown id; Linux reaps the child
Shell.terminateProcess(id, timeoutMs);   // WM_CLOSE | SIGTERM, wait, then kill; true if ended

// environment
Shell.getenv(name);                      // "" WHEN NOT SET -> use hasEnv
Shell.setenv(name, value);               // process wide, inherited by children; Windows: "" removes
Shell.hasEnv(name); Shell.isEnv(name, value);
```

## Hard rules

1. **Check return values**: nothing throws for I/O or process failures;
   functions return `false`, `undefined`, `-1`, `0` or `[]`.
2. **Pass every argument**: a missing one becomes the string
   `"undefined"` (`Shell.mkdir()` creates a folder named `undefined`).
3. **Old SDKs (up to 5.10.0 build 7) have two broken functions**:
   `Shell.is` always returns `false`, `Shell.fileReplaceText` never returns.
   Fixed in the sources; an installed DLL / fabricare built before the fix
   still has them. Code that must run there: detect Windows with
   `Shell.hasEnv("WINDIR")` (fabricare: `OS.isWindows()`, `OS.isLinux()`,
   `OS.isMinGW()`, `OS.isEmscripten()`, which also tell the toolchains
   apart), replace text with `fileGetContents` → `text.replace(a, b)` (every
   occurrence) → `mkdirFilePath` + `filePutContents`.
4. **`fileReplaceText`: `fileOut` must differ from `fileIn`** (the output
   is truncated before the input is read: the file ends up empty). Pairs
   apply in order to each line, so later pairs see earlier replacements;
   a search text longer than `lineMaxLength`, or split by it, is not found;
   non-array entries and empty search texts are ignored.
5. **Relative names use the process current directory**, not the script
   folder. `chdir` and `setenv` are process wide (all threads).
6. **Create folders first**: `filePutContents` and `copy` need an existing
   folder; `copyFile`, `copyDirRecursively`, `copyFilesToDirectory`
   create theirs; `mkdirFilePath(file)` before writing.
7. **`getFileBasename` keeps the folder** (`"a/b/c.txt"` → `"a/b/c"`); the
   stem is `getFileBasename(getFileName(name))`.
8. **Listing is not recursive**, wildcards only in the last part, results
   are `folderAsWritten + name`, order unspecified on Linux (sort). Without a
   wildcard the pattern is a single name test: `getDirList("dir")` is
   `["dir"]` — list sub folders with `"dir/*"`.
9. **`copyFilesToDirectory` with a pattern needs a folder part**:
   `"./*.txt"`, not `"*.txt"` (fails).
10. **`copyDirRecursively(missing, dst)` returns `true`** and creates an
    empty `dst`; **`isEmptyDir(missing)` is `true`**: test
    `directoryExists` first.
11. **Recursive removals stop at the first failure** (file in use,
    read-only) and return `false`, leaving a partial tree. Use the `Force`
    variants for build outputs (read-only files from git / archives on
    Windows). Links inside trees are removed, never followed.
12. **`rename` does not replace on Windows**: `removeFile(dst)` first.
13. **`system` vs `execute`**: `system` always goes through the shell (pipes,
    `>`, `&&`, built-ins, `.cmd`); `execute` on Windows does not — use
    `cmd /c ...` for built-ins there. `executeNoWait` never uses a shell.
    Exit codes: Linux `128 + signal`; Windows `execute` `127` if the program
    cannot start; `cmd.exe` gives `1` for an unknown command.
14. **`getenv` returns `""` for unset variables**: test `hasEnv`. Names are
    case-insensitive on Windows only.
15. **`terminateProcess` stops only that process**, not its children
    (`cmd /c x` → `x` keeps running). Call `isProcessTerminated` on Linux
    children so they are reaped.
16. **Platform differences**: `getFileSize(dir)` is `-1` on Windows only;
    `realPath(missing)` is `undefined` on Linux only; `remove(emptyDir)`
    works on Linux only; wildcard matching is case-insensitive on Windows.
17. **Whole files only**: for line by line, append or seek use `File`
    (`quantum-script--file`); for directory walking with attributes use
    `ShellFind` (loaded with `Shell`).

## Patterns

```javascript
Shell.mkdirFilePath(out); Shell.filePutContents(out, text);           // write generated file

if (Shell.compareLastWriteTime(target, source) < 0) { rebuild(); };   // target missing or older

if (Shell.system("git rev-parse HEAD > temp/head.txt") != 0) { throw "git failed"; };
var head = Shell.fileGetContents("temp/head.txt").trim();             // capture output

if (Shell.directoryExists("output")) { Shell.removeDirRecursivelyForce("output"); };   // clean

Shell.fileReplaceText("Version.Template.rh", "temp/Version.rh", [["$VERSION", "1.2.3"]], 0);   // template

var sep = Shell.is("win") ? ";" : ":";                                // PATH for children
Shell.setenv("PATH", Shell.realPath("bin") + sep + Shell.getenv("PATH"));

var id = Shell.executeHiddenNoWait("server --port 8080");            // background
if (id == 0) { throw "cannot start"; };
Shell.terminateProcess(id, 3000);

function findFiles(dir, pattern, result) {                            // recursive listing
	var f = Shell.getFileList(dir + "/" + pattern);
	for (var k = 0; k < f.length; ++k) { result.push(f[k]); };
	var d = Shell.getDirList(dir + "/*");
	for (var m = 0; m < d.length; ++m) { findFiles(d[m], pattern, result); };
	return result;
};
```

## C++

```cpp
#include <XYO/QuantumScript.Extension/Shell.hpp>
using namespace XYO::QuantumScript;

// host init callback: Shell requires Buffer and ShellFind
Extension::Buffer::registerInternalExtension(executive);
Extension::Shell::registerInternalExtension(executive);
Extension::ShellFind::registerInternalExtension(executive);
// scripts still call Script.requireExtension("Shell")
```

- `fabricare.json`: depend on `"quantum-script--shell"` **and**
  `"quantum-script--shellfind"` (Shell only requires ShellFind at run time,
  its fabricare.json does not list it); static: the `.static` variants,
  `"crt": "static"`, and register all three as internal.
- `quantum-script--shell.static` defines
  `XYO_QUANTUMSCRIPT_EXTENSION_SHELL_LIBRARY` (empty export macro, no
  `quantumScriptExtension` entry point).
- From C++ call `XYO::System::Shell::...` directly; inside
  `namespace XYO::QuantumScript::Extension::Shell` the name `Shell` is the
  namespace, write `XYO::System::Shell` in full.

## Working in this repository

- Build: `fabricare make`, `fabricare test` (runs `test/test.01`, which
  registers Console, Buffer, Shell and ShellFind as internal and runs
  `test/test.01.js` from `output/test`; run `make` first), `fabricare
  install` (see the `fabricare` skill). `quantum-script`,
  `quantum-script--console`, `quantum-script--buffer` and
  `quantum-script--shellfind` must be installed first. Windows: run from a
  `vcvars64.bat` environment (`C:\Program Files\Microsoft Visual
  Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat` on this machine).
  If `fabricare test` reports `'test.01' is not recognized`, the
  environment sets `NoDefaultCurrentDirectoryInExePath` (Claude Code does):
  run `output/test/test.01.exe` from `output/test` with `output/bin` first
  on `PATH`.
- `test/test.01.js` checks `Shell.is` and `Shell.fileReplaceText`
  (regressions of the 5.10.0 build 7 bugs); add a check there when fixing
  a function.
- Native functions live in `Shell/Library.cpp` as
  `static TPointer<Variable> name(VariableFunction *, Variable *this_, VariableArray *arguments)`,
  convert arguments with `toString()` / `toNumber()`, call
  `XYO::System::Shell`, and are registered in `initExecutive` with
  `executive->setFunction2("Shell.name(args)", name)`.
- `is` tests `XYO_PLATFORM_OS_WINDOWS` / `XYO_PLATFORM_OS_LINUX` (the
  platform headers set them for MinGW / Emscripten too); never use
  `XYO_OS_TYPE_*`, nothing defines them. `fileReplaceText` reads
  `lineMaxLength` from `index(3)` and maps `0` to `32768`: a line size of
  `0` makes `Stream::LineRead::readLn` loop forever.
- New functions: update `README.md`, `docs/script-api.md`,
  `docs/reference.md`, `docs/cpp-api.md` and this skill.
- Code style: tabs (width 8), `.clang-format`, CRLF, statements and blocks
  end with `};`, camelCase. SPDX header: MIT for `source/` and `docs/`,
  Unlicense for `test/` and `.claude/` (see `.reuse/dep5`).
