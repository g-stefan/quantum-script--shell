# Script API

Everything the extension defines after `Script.requireExtension("Shell")`:
a global object `Shell` with plain functions (no `this`, no constructor).

General rules:

- Name arguments are converted with `toString`, numbers with `toNumber`.
  Missing arguments become `undefined`, i.e. the string `"undefined"`:
  always pass every argument.
- Nothing throws for I/O problems: functions return `false`, `undefined`,
  `-1`, `0` or an empty array.
- Relative names use the process current directory. See
  [Files and directories](files-and-directories.md) and
  [Processes and environment](processes-and-environment.md) for the
  details shared by several functions.

## Whole files

### `Shell.fileGetContents(file)`

Reads the whole file (binary, bytes unchanged) and returns it as a String;
`undefined` if it cannot be opened or read. An empty file gives `""`.

### `Shell.filePutContents(file, text)`

Creates or truncates `file` and writes `text.toString()`. `true` if every
byte was written. The folder must exist (`Shell.mkdirFilePath(file)`).

### `Shell.fileGetContentsBuffer(file)`

Reads the whole file into a new `Buffer` (`length` = `size` = file size);
`undefined` on failure.

### `Shell.filePutContentsBuffer(file, buffer)`

Creates or truncates `file` and writes the first `buffer.length` bytes of a
`Buffer`. `true` / `false`; `undefined` (and nothing is created) if `buffer`
is not a `Buffer`.

### `Shell.fileReplaceText(fileIn, fileOut, textInOut, lineMaxLength)`

Copies `fileIn` to `fileOut` line by line, replacing in each line every
occurrence of `textInOut[k][0]` with `textInOut[k][1]`. `true` if the whole
file was copied, `false` if `fileIn` cannot be read or `fileOut` cannot be
created or written.

```javascript
Shell.fileReplaceText("source/Version.Template.rh", "temp/Version.rh", [
	["$VERSION_ABCD", "1,2,3,0"],
	["$VERSION_STR", "1.2.3"]
], 0);
```

- The folder of `fileOut` is created. Line ends are kept as they are.
- `textInOut` is an array of `[search, replacement]` pairs, applied **in
  order** to each line, so a later pair also sees the text an earlier one
  inserted (`[["A", "B"], ["B", "C"]]` turns `A` into `C`). Entries that are
  not arrays, and pairs with an empty `search`, change nothing; a
  `textInOut` that is not an array copies the file unchanged.
- `lineMaxLength` limits how many bytes are read as one line; `0` or a
  missing value means `32768`. A longer line is processed in pieces, and a
  `search` text split between two pieces is not replaced: keep it larger
  than the longest line.
- **`fileOut` must not be `fileIn`**: the output is created (truncated)
  before the input is read, so the file ends up empty. Write to a temporary
  name, then `Shell.removeFile(fileIn); Shell.rename(temp, fileIn);`.

> Builds up to 5.10.0 build 7 read the line size from the third argument
> (the array, converted to `0`) instead of `lineMaxLength`, and the call
> **never returned**. Where such an SDK may still be installed, use:

```javascript
function fileReplaceText(fileIn, fileOut, textInOut) {
	var text = Shell.fileGetContents(fileIn);
	if (Script.isUndefined(text)) {
		return false;
	};
	for (var k = 0; k < textInOut.length; ++k) {
		text = text.replace(textInOut[k][0], textInOut[k][1]);   // every occurrence
	};
	return Shell.mkdirFilePath(fileOut) && Shell.filePutContents(fileOut, text);
};
```

## Tests and information

### `Shell.fileExists(name)`

`true` for a regular file or a link to one; `false` for directories,
missing names, broken links and devices.

### `Shell.directoryExists(name)`

`true` for a directory or a link / junction to one. `"C:"` tests `"C:\"`.

### `Shell.isEmptyDir(name)`

`true` if the directory has no entries besides `.` and `..`. **Also `true`
when `name` does not exist**; combine with `directoryExists`.

### `Shell.getFileSize(file)`

Size in bytes, `-1` if it does not exist. Windows: `-1` for a directory;
Linux: a directory has a size. Links: the size of the target.

### `Shell.compareLastWriteTime(fileA, fileB)`

`-1` if `fileA` was modified before `fileB`, `0` if at the same time, `1` if
after. A missing file is older than an existing one; two missing files
give `0`. Links are followed.

### `Shell.is(what)`

`true` for `"win"` on Windows (MinGW builds too) and for `"unix"` on Linux
(Emscripten builds too); `false` for anything else. Decided when the
extension is compiled (`XYO_PLATFORM_OS_WINDOWS` / `XYO_PLATFORM_OS_LINUX`).

> Builds up to 5.10.0 build 7 tested macros that nothing defines and
> **always returned `false`**. Where such an SDK may still be installed,
> test `Shell.hasEnv("WINDIR")`; fabricare scripts also have
> `OS.isWindows()` / `OS.isLinux()`.

## Path names

String operations: the names do not have to exist. On Windows both `/` and
`\` are separators. See the table in
[Files and directories](files-and-directories.md#path-name-functions).

### `Shell.getFileName(fileName)`

The part after the last separator (`"a/b/c.txt"` → `"c.txt"`); the whole
name if there is none.

### `Shell.getFileExtension(fileName)`

The part of the file name after its last dot, without the dot
(`"c.tar.gz"` → `"gz"`, `".gitignore"` → `"gitignore"`); `""` if the file
name has no dot.

### `Shell.getFileBasename(fileName)`

`fileName` without `"." + extension`, **folder kept**
(`"a/b/c.tar.gz"` → `"a/b/c.tar"`); unchanged if there is no extension.

### `Shell.getFilePath(fileName)`

The folder part without the trailing separator (`"a/b/c.txt"` → `"a/b"`);
`""` if there is no separator.

### `Shell.getFilePathX(fileName)`

The folder part **with** the trailing separator (`"a/b/c.txt"` → `"a/b/"`);
`""` if there is none.

### `Shell.realPath(path)`

An absolute name. Windows: computed from the current directory, `.` / `..`
resolved, the name does not have to exist, links not resolved. Linux: the
name must exist (`undefined` otherwise), links resolved.

### `Shell.getcwd()`

The current directory of the process (`"."` if it cannot be determined).

### `Shell.chdir(path)`

Changes the current directory of the **process** (every thread). `true` /
`false`.

## Files

### `Shell.copy(src, dst)`

Copies one file, overwriting `dst`. The folder of `dst` must exist.
`false` if `src` is missing, `dst`'s folder is missing, or both name the same
file. Linux keeps the permission bits.

### `Shell.copyFile(source, target)`

Creates the folder of `target` if needed, then `Shell.copy`.

### `Shell.rename(src, dst)`

Renames or moves a file or directory. Windows: `false` if `dst` exists.
Linux: replaces an existing `dst` file (or empty directory).

### `Shell.remove(file)`

The C `remove`: deletes a file or a link (on Windows also a junction /
directory link, not what it points to). On Linux it also removes an empty
directory.

### `Shell.removeFile(file)`

Deletes `file` only if `Shell.fileExists(file)`. `false` for missing names
and directories.

### `Shell.removeFileForce(file)`

As `removeFile`; if the delete fails and the file is read-only, removes the
read-only flag and tries again.

### `Shell.removeFileAndDirectoryIfEmpty(file)`

Deletes `file`, then its folder if that is now empty (one level). `true`
only if both were removed; with no folder part the file is still deleted but
the result is `false`.

### `Shell.touch(file)`

Sets the access and modification time to now; creates an empty file if it
does not exist; never truncates. Works on directories. `true` / `false`.

### `Shell.touchIfExists(file)`

`Shell.touch(file)` if `Shell.fileExists(file)`, otherwise `false` (nothing
is created).

## Directories

### `Shell.mkdir(path)`

Creates one directory. `false` if it exists or its parent is missing.

### `Shell.mkdirRecursively(path)`

Creates every missing level. `true` if the directory exists at the end
(also when it existed before, or was created at the same time by another
process); `false` for `""` or when a level is a file.

### `Shell.mkdirRecursivelyIfNotExists(path)`

`false` if `path` is a file, `true` if it is a directory, otherwise
`Shell.mkdirRecursively(path)`.

### `Shell.mkdirFilePath(file)`

`Shell.mkdirRecursivelyIfNotExists(Shell.getFilePath(file))`; `true` when
`file` has no folder part.

### `Shell.rmdir(path)`

Removes an empty directory. `true` / `false`.

### `Shell.rmdirForce(path)`

As `rmdir`; on failure removes the read-only flag of the directory and tries
again.

### `Shell.removeEmptyDir(dir)`

`Shell.rmdir(dir)` if `Shell.isEmptyDir(dir)`, otherwise `false`.

### `Shell.removeEmptyDirRecursively(path)`

Removes every empty sub directory at any depth (a directory holding only
empty directories is empty), then `path` itself if it became empty. `true`
only if `path` was removed; `false` for a missing `path`. Links are not
entered.

### `Shell.removeDirContentRecursively(path)`

Deletes everything inside `path` and keeps `path`. Links are deleted, not
followed. Stops at the first entry that cannot be deleted (`false`).
`true` for a missing `path`.

### `Shell.removeDirRecursively(path)`

`removeDirContentRecursively(path)`, then `rmdir(path)`. `false` for a
missing `path`.

### `Shell.removeDirContentRecursivelyForce(path)`

As `removeDirContentRecursively`; entries that cannot be deleted because
they are read-only are made writable and deleted.

### `Shell.removeDirRecursivelyForce(path)`

`removeDirContentRecursivelyForce(path)`, then `rmdir(path)`.

### `Shell.removeFileRecursively(dir, file)`

Deletes the files matching the name or pattern `file` (`*`, `?`) in `dir`
and in every sub directory (links to directories are not entered). `false`
at the first file that cannot be deleted, `true` otherwise (also when
nothing matched).

### `Shell.copyDirRecursively(source, target)`

Copies the content of `source` into `target`, every level: creates
`target`, merges into existing directories, overwrites files. Links to
directories are copied as directories (what they point to); a link that
loops back into the tree makes it fail. `false` at the first failure.
A **missing `source` gives `true`** and an empty `target`.

### `Shell.copyFilesToDirectory(source, target)`

Copies the files directly in `source` (not sub directories) into `target`,
created if needed. `source` is a directory, or a directory + pattern
(`"bin/*.dll"`). A pattern must have a folder part: `"./*.dll"`, not
`"*.dll"` (which fails). `false` at the first file that cannot be copied.

### `Shell.getFileList(path)`

Array of Strings: the files (and links to files) matching `path`.
Wildcards `*` and `?` in the last part only; each entry is the folder part
of `path` as written + the file name; not recursive; order as the system
returns it. Without a wildcard: `[path]` if `Shell.fileExists(path)`,
otherwise `[]`.

### `Shell.getDirList(path)`

As `getFileList` for directories (and links to directories), without `.`
and `..`. Without a wildcard: `[path]` if `Shell.directoryExists(path)`,
otherwise `[]`, so list the sub directories of `dir` with `dir + "/*"`.

## Processes

### `Shell.system(cmd)`

Runs `cmd` with the system shell (`cmd.exe /c`, `/bin/sh -c`) and waits.
Returns the exit code (Linux: `128 + signal` when killed, `-1` if the shell
cannot run).

### `Shell.execute(cmd)`

Runs and waits. Windows: starts the program directly (no shell, window
shown), `127` if it cannot be started. Linux: as `Shell.system`. Returns the
exit code.

### `Shell.executeHidden(cmd)`

As `execute`, window hidden on Windows.

### `Shell.executeNoWait(cmd)`

Starts the program without a shell and returns at once with its process id
(a Number), `0` if it cannot be started. Linux: arguments split on spaces,
double quotes group, `PATH` searched.

### `Shell.executeHiddenNoWait(cmd)`

As `executeNoWait`, window hidden on Windows.

### `Shell.isProcessTerminated(id)`

`true` if the process has ended (or `id` is `0` / not running), `false` while
it runs. Linux: also reaps the ended child.

### `Shell.terminateProcess(id, timeout)`

Asks the process to close (Windows `WM_CLOSE`, Linux `SIGTERM`), waits up to
`timeout` milliseconds, then kills it (`TerminateProcess`, `SIGKILL`). `true`
when it has ended or was not running, `false` if not allowed. Child
processes of that process are not stopped.

## Environment

### `Shell.getenv(name)`

The value of the environment variable, `""` if it is not set.

### `Shell.setenv(name, value)`

Sets the variable for the process and the processes it starts later.
`true` / `false`. Windows: `value` `""` removes the variable.

### `Shell.hasEnv(name)`

`true` if the variable is set.

### `Shell.isEnv(name, value)`

`true` if the variable is set and equals `value` exactly (case-sensitive
comparison of the value).
