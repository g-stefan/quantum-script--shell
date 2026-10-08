# C++ API

For hosts that embed Quantum Script and for maintainers of this extension.
Read the `quantum-script` repository's `docs/embedding.md` and
`docs/writing-extensions.md` first. `Shell` has no value type of its own:
it is a set of native functions on a plain object, so the C++ surface is
small.

## Headers and namespace

```cpp
#include <XYO/QuantumScript.Extension/Shell.hpp>   // Library.hpp: initExecutive, registerInternalExtension

using namespace XYO::QuantumScript;
```

Namespace: `XYO::QuantumScript::Extension::Shell`. Export macro:
`XYO_QUANTUMSCRIPT_EXTENSION_SHELL_EXPORT` (empty when
`XYO_QUANTUMSCRIPT_EXTENSION_SHELL_LIBRARY` is defined, i.e. static builds).

Inside that namespace `Shell` is the namespace itself: name the system
functions `XYO::System::Shell::...` in full.

## Registering the extension

```cpp
void Extension::Shell::registerInternalExtension(Executive *executive);
void Extension::Shell::initExecutive(Executive *executive, void *extensionId);
```

- `registerInternalExtension` registers `"Shell"` as an internal extension;
  call it from the host's init callback, together with
  `Extension::Buffer::registerInternalExtension` and
  `Extension::ShellFind::registerInternalExtension` (see
  [Getting started](getting-started.md#4-register-it-in-a-c-host)).
- `initExecutive` is the extension's init function, run by the engine when a
  script first requires `Shell` in a thread. It sets the extension name,
  info (license text), version and marks it public, runs
  `Script.requireExtension("Buffer")` and
  `Script.requireExtension("ShellFind")`, creates `var Shell={};` and
  registers every native function. Do not call it directly.
- The DLL build also exports
  `extern "C" void quantumScriptExtension(Executive *, void *)`, which
  forwards to `initExecutive`; it is what `Script.requireExtension` looks up
  in `quantum-script--shell.dll`.

From C++ code use `XYO::System::Shell` directly (see the `xyo-system`
repository, `docs/files.md` and `docs/processes.md`); the script functions
add nothing but argument conversion.

## Script function → C++ function

| Script | `XYO::System::Shell::` | Result conversion |
|--------|------------------------|-------------------|
| `fileGetContents(file)` | `fileGetContents(name, String &)` | String, `undefined` on `false` |
| `filePutContents(file, text)` | `filePutContents(name, String)` | Boolean |
| `fileGetContentsBuffer(file)` | `fileGetContents(name, Buffer &)` into a new `VariableBuffer` | Buffer, `undefined` on `false` |
| `filePutContentsBuffer(file, buffer)` | `filePutContents(name, Buffer)` | Boolean, `undefined` if not a `VariableBuffer` |
| `fileReplaceText(fileIn, fileOut, textInOut, lineMaxLength)` | `fileReplaceText(in, out, TDynamicArray<TDynamicArray<String>>, maxLineSize)` | Boolean; `lineMaxLength` `0` → `32768` |
| `fileExists(name)` | `fileExists` | Boolean |
| `directoryExists(name)` | `directoryExists` | Boolean |
| `isEmptyDir(name)` | `isEmptyDir` | Boolean |
| `getFileSize(file)` | `getFileSize(name, int64_t &)` | Number, `-1` on `false` |
| `compareLastWriteTime(fileA, fileB)` | `compareLastWriteTime` | Number |
| `is(what)` | — (`#ifdef XYO_PLATFORM_OS_WINDOWS` / `XYO_PLATFORM_OS_LINUX`) | Boolean |
| `getFileName`, `getFileExtension`, `getFileBasename`, `getFilePath`, `getFilePathX` | same names | String |
| `realPath(path)` | `realPath(path, String &)` | String, `undefined` on `false` |
| `getcwd()` | `getCwd` | String |
| `chdir(path)` | `chdir` | Boolean |
| `copy`, `copyFile`, `rename`, `remove`, `removeFile`, `removeFileForce`, `removeFileAndDirectoryIfEmpty`, `touch`, `touchIfExists` | same names | Boolean |
| `mkdir`, `mkdirRecursively`, `mkdirRecursivelyIfNotExists`, `mkdirFilePath`, `rmdir`, `rmdirForce`, `removeEmptyDir`, `removeEmptyDirRecursively`, `removeDirContentRecursively`, `removeDirRecursively`, `removeDirContentRecursivelyForce`, `removeDirRecursivelyForce`, `removeFileRecursively`, `copyDirRecursively`, `copyFilesToDirectory` | same names | Boolean |
| `getFileList(path)`, `getDirList(path)` | `getFileList` / `getDirList(name, TDynamicArray<String> &)` | Array of String |
| `system(cmd)` | `system` | Number |
| `execute`, `executeHidden` | same names | Number (exit code) |
| `executeNoWait`, `executeHiddenNoWait` | same names | Number (`ProcessId`) |
| `isProcessTerminated(id)` | `isProcessTerminated((ProcessId)id)` | Boolean |
| `terminateProcess(id, timeout)` | `terminateProcess((ProcessId)id, timeout)` | Boolean |
| `getenv(name)` | `getEnv` | String |
| `setenv(name, value)` | `setenv` | Boolean |
| `hasEnv(name)`, `isEnv(name, value)` | same names | Boolean |

`XYO::System::Shell` functions that have **no** script binding:
`moveDirRecursively`, `copyFileIfExists`, `filePutContentsAppend`,
`fileGetContentsSkipLines`, `isChanged`, `isChangedRecursive`,
`isAbsolutePath`, `getExecutable`, `getExecutablePath`, `normalize`,
`isReadOnly`, `setReadOnly`, `executeWriteOutputToFile`, the `...UTF8`
variants, `pathSeparator`, `envPathSeparator`.

## Fixed issues in `Library.cpp`

Both were broken up to 5.10.0 build 7; `test/test.01.js` checks them.

- **`is`** tested `XYO_OS_TYPE_WIN` / `XYO_OS_TYPE_UNIX`, which nothing
  defines, so it always returned `false`. It now tests the platform macros
  `XYO_PLATFORM_OS_WINDOWS` (also set for MinGW) and
  `XYO_PLATFORM_OS_LINUX` (also set for Emscripten) from `xyo-platform`.
- **`fileReplaceText`** passed `(arguments->index(2))->toIndex()`, the
  `textInOut` array (`0`), as `maxLineSize`. `Stream::LineRead::readLn(line, 0)`
  returns `true` with an empty line, so the copy loop never ended. It now
  passes `(arguments->index(3))->toIndex()`, and replaces `0` (missing or
  invalid `lineMaxLength`) with `32768` for the same reason.

## Notes for maintainers

- Native functions live in `Shell/Library.cpp` as
  `static TPointer<Variable> name(VariableFunction *, Variable *this_, VariableArray *arguments)`,
  convert their arguments with `toString()` / `toNumber()` and call
  `XYO::System::Shell`. They are registered in `initExecutive` with
  `executive->setFunction2("Shell.name(args)", name)`; the signature only
  names the parameters, every passed argument reaches the function.
- `getenv` / `setenv` are implemented as `shell_getenv` / `shell_setenv`
  to avoid clashing with the C library names.
- Each function prints its name under `XYO_QUANTUMSCRIPT_DEBUG_RUNTIME`.
- `fabricare.json`: the DLL depends on `quantum-script`,
  `quantum-script--console` and `quantum-script--buffer`; `ShellFind` is
  only required at run time (`compileStringX`), so
  `quantum-script--shellfind` appears only in the test project.
- When adding a function, update `README.md`, `docs/script-api.md`,
  `docs/reference.md`, the table above and the skill in
  `.claude/skills/quantum-script--shell`.
- Code style: tabs (width 8), `.clang-format`, CRLF, statements and blocks
  end with `};`, camelCase. SPDX header: MIT for `source/` and `docs/`,
  Unlicense for `test/` and `.claude/` (see `.reuse/dep5`).
