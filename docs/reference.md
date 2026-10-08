# API reference

## Script

Available after `Script.requireExtension("Shell")` (which also loads
`Buffer` and `ShellFind`). All functions are on the `Shell` object.

### Whole files

| Function | Returns | Behavior |
|----------|---------|----------|
| `fileGetContents(file)` | String / `undefined` | whole file, bytes unchanged |
| `filePutContents(file, text)` | Boolean | create / truncate; folder must exist |
| `fileGetContentsBuffer(file)` | Buffer / `undefined` | whole file into a new `Buffer` |
| `filePutContentsBuffer(file, buffer)` | Boolean / `undefined` | writes `buffer.length` bytes; `undefined` if not a `Buffer` |
| `fileReplaceText(fileIn, fileOut, textInOut, lineMaxLength)` | Boolean | copy line by line replacing `[search, replacement]` pairs in order; `lineMaxLength` `0` → `32768`; `fileOut` must differ from `fileIn` |

### Tests and information

| Function | Returns | Behavior |
|----------|---------|----------|
| `fileExists(name)` | Boolean | regular file or link to one |
| `directoryExists(name)` | Boolean | directory or link to one; `"C:"` → `"C:\"` |
| `isEmptyDir(name)` | Boolean | no entries; **`true` if missing** |
| `getFileSize(file)` | Number | bytes, `-1` if missing (and for a directory on Windows) |
| `compareLastWriteTime(fileA, fileB)` | Number | `-1` / `0` / `1`; missing is older |
| `is(what)` | Boolean | `"win"` on Windows, `"unix"` on Linux (always `false` up to 5.10.0 build 7) |

### Path names

| Function | `"a/b/c.tar.gz"` gives | Notes |
|----------|------------------------|-------|
| `getFileName(fileName)` | `"c.tar.gz"` | |
| `getFileExtension(fileName)` | `"gz"` | `""` if none |
| `getFileBasename(fileName)` | `"a/b/c.tar"` | folder kept |
| `getFilePath(fileName)` | `"a/b"` | `""` if no folder |
| `getFilePathX(fileName)` | `"a/b/"` | `""` if no folder |
| `realPath(path)` | absolute name | String / `undefined` (Linux: must exist) |
| `getcwd()` | current directory | |
| `chdir(path)` | Boolean | process wide |

### Files

| Function | Returns | Behavior |
|----------|---------|----------|
| `copy(src, dst)` | Boolean | one file, overwrites; folder of `dst` must exist |
| `copyFile(source, target)` | Boolean | creates the folder of `target`, then copies |
| `rename(src, dst)` | Boolean | Windows: fails if `dst` exists; Linux: replaces |
| `remove(file)` | Boolean | file or link (Linux: also an empty directory) |
| `removeFile(file)` | Boolean | only if `fileExists` |
| `removeFileForce(file)` | Boolean | also read-only files |
| `removeFileAndDirectoryIfEmpty(file)` | Boolean | file, then its folder if empty |
| `touch(file)` | Boolean | time to now; creates if missing |
| `touchIfExists(file)` | Boolean | only if `fileExists` |

### Directories

| Function | Returns | Behavior |
|----------|---------|----------|
| `mkdir(path)` | Boolean | one level; `false` if it exists |
| `mkdirRecursively(path)` | Boolean | all levels; `true` if it exists |
| `mkdirRecursivelyIfNotExists(path)` | Boolean | as above; `false` if `path` is a file |
| `mkdirFilePath(file)` | Boolean | the folder of `file`; `true` if none |
| `rmdir(path)` / `rmdirForce(path)` | Boolean | empty directory (Force: also read-only) |
| `removeEmptyDir(dir)` | Boolean | `rmdir` if empty |
| `removeEmptyDirRecursively(path)` | Boolean | every empty sub folder, then `path`; `true` if `path` removed |
| `removeDirContentRecursively(path)` | Boolean | everything inside, keeps `path` |
| `removeDirRecursively(path)` | Boolean | everything, `path` too |
| `removeDirContentRecursivelyForce(path)` / `removeDirRecursivelyForce(path)` | Boolean | also read-only entries |
| `removeFileRecursively(dir, file)` | Boolean | files matching `file` in the whole tree |
| `copyDirRecursively(source, target)` | Boolean | content of `source` into `target`; missing `source` → `true` |
| `copyFilesToDirectory(source, target)` | Boolean | files of a folder or `folder/pattern`, not recursive |
| `getFileList(path)` | Array | files matching `path` (`*`, `?` in the last part); not recursive |
| `getDirList(path)` | Array | directories matching `path`; use `dir + "/*"` |

### Processes

| Function | Returns | Behavior |
|----------|---------|----------|
| `system(cmd)` | Number | system shell, waits, exit code |
| `execute(cmd)` / `executeHidden(cmd)` | Number | Windows: no shell, `127` if it cannot start; Linux: as `system` |
| `executeNoWait(cmd)` / `executeHiddenNoWait(cmd)` | Number | no shell, process id, `0` if it cannot start |
| `isProcessTerminated(id)` | Boolean | ended, or not running |
| `terminateProcess(id, timeout)` | Boolean | close, wait `timeout` ms, kill |

### Environment

| Function | Returns | Behavior |
|----------|---------|----------|
| `getenv(name)` | String | `""` if not set |
| `setenv(name, value)` | Boolean | process wide; Windows: `""` removes |
| `hasEnv(name)` | Boolean | set |
| `isEnv(name, value)` | Boolean | set and equal |

### Errors

Nothing throws for file system or process failures: test the return value.
A missing argument is converted to the string `"undefined"` (so
`Shell.mkdir()` creates a folder named `undefined`). Loading fails with
`Unable to open "Shell"` (or `"Buffer"` / `"ShellFind"`) when a library is
not found and no internal extension is registered.

## C++

```cpp
#include <XYO/QuantumScript.Extension/Shell.hpp>

namespace XYO::QuantumScript::Extension::Shell {
	void initExecutive(Executive *executive, void *extensionId);   // extension init, run by the engine
	void registerInternalExtension(Executive *executive);          // register "Shell" as internal
};

extern "C" void quantumScriptExtension(Executive *, void *);       // DLL entry point (not in static builds)
```

| Macro | Meaning |
|-------|---------|
| `XYO_QUANTUMSCRIPT_EXTENSION_SHELL_EXPORT` | import / export of the library symbols |
| `XYO_QUANTUMSCRIPT_EXTENSION_SHELL_INTERNAL` | defined while building the DLL (export) |
| `XYO_QUANTUMSCRIPT_EXTENSION_SHELL_LIBRARY` | static library: empty export macro, no entry point |

| fabricare project | Kind |
|-------------------|------|
| `quantum-script--shell` | DLL / shared library |
| `quantum-script--shell.static` | static library, static CRT |
