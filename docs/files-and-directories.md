# Files and directories

## Names and paths

All functions take names as strings (converted with `toString`) and pass
them to the operating system unchanged:

- **Relative names** are relative to the process current directory
  (`Shell.getcwd()`, changed by `Shell.chdir(path)`), not to the script.
- **Separators**: `/` works on both platforms. On Windows `\` works too,
  and the path name functions (`getFileName`, `getFilePath`, ...) accept
  both; on Linux only `/` is a separator.
- **Case**: Windows file systems ignore case (`a.TXT` is `a.txt`), Linux
  ones do not.
- **Encoding**: names are UTF-8 byte strings. On Windows the ANSI functions
  are used, so non-ASCII names need a UTF-8 code page manifest in the host
  executable (`quantum-script.exe` has it).
- **Drive roots**: `Shell.directoryExists("C:")` checks `C:\`.

### Path name functions

`getFileName`, `getFileExtension`, `getFileBasename`, `getFilePath` and
`getFilePathX` work on the string only: the file does not have to exist,
and the result keeps the separators as written.

| `name` | `getFilePath` | `getFilePathX` | `getFileName` | `getFileExtension` | `getFileBasename` |
|--------|---------------|----------------|---------------|--------------------|-------------------|
| `"a/b/c.tar.gz"` | `"a/b"` | `"a/b/"` | `"c.tar.gz"` | `"gz"` | `"a/b/c.tar"` |
| `"c.txt"` | `""` | `""` | `"c.txt"` | `"txt"` | `"c"` |
| `"dir.v2/file"` | `"dir.v2"` | `"dir.v2/"` | `"file"` | `""` | `"dir.v2/file"` |
| `".gitignore"` | `""` | `""` | `".gitignore"` | `"gitignore"` | `""` |
| `"a/b/"` | `"a/b"` | `"a/b/"` | `""` | `""` | `"a/b/"` |

- `getFileBasename` is the name **without its extension, folder kept**,
  not the file name without extension. For that combine the two:
  `Shell.getFileBasename(Shell.getFileName(name))` → `"c.tar"`.
- The extension is what follows the **last** dot of the file name; a dot in
  a folder name is ignored.
- `getFilePath` has no trailing separator, `getFilePathX` has one, so
  `getFilePathX(name) + newName` builds a sibling name, also when there is
  no folder.

### Current directory and absolute paths

- `Shell.getcwd()` returns the current directory (`"."` if it cannot be
  determined).
- `Shell.chdir(path)` changes it for the **whole process** (all threads),
  `false` if the folder does not exist.
- `Shell.realPath(name)` returns an absolute name:
  - Windows: `GetFullPathName`, a string operation: the name does not have
    to exist, `.` and `..` are resolved, links are not;
  - Linux: `realpath`: the name must exist (`undefined` otherwise), links
    are resolved.

## What is a file

| Test | `true` for | `false` for |
|------|------------|-------------|
| `Shell.fileExists(name)` | a regular file, or a link to one | directories, missing names, broken links, devices (`NUL`, `/dev/null`), sockets |
| `Shell.directoryExists(name)` | a directory, or a link / junction to one | files, missing names, broken links |
| `Shell.isEmptyDir(name)` | a directory with no entries besides `.` and `..` | a directory with entries; note: **`true` for a missing directory** |

Combine them when it matters: `Shell.directoryExists(d) && Shell.isEmptyDir(d)`.

`Shell.getFileSize(file)` returns the size in bytes, `-1` when the file does
not exist. On Windows it is `-1` for a directory too; on Linux a directory
has a size (often `4096`): test with `fileExists` first. For a link it is
the size of what the link points to.

`Shell.compareLastWriteTime(a, b)` compares modification times: `-1` if
`a` is older, `0` if equal, `1` if newer. A **missing file is older** than
an existing one (two missing files are equal), so
`compareLastWriteTime(target, source) < 0` means "target missing or out of
date". Links are followed. Linux compares nanoseconds.

## Reading and writing whole files

| Function | Result |
|----------|--------|
| `Shell.fileGetContents(file)` | the content as a String, `undefined` if the file cannot be read |
| `Shell.filePutContents(file, text)` | `true` / `false`; creates or **truncates** |
| `Shell.fileGetContentsBuffer(file)` | a `Buffer` (`length` = `size` = file size), `undefined` if the file cannot be read |
| `Shell.filePutContentsBuffer(file, buffer)` | `true` / `false`; writes `buffer.length` bytes; `undefined` (nothing written) if `buffer` is not a `Buffer` |

- Files are read and written in **binary mode**: bytes unchanged, no line
  end translation, no encoding conversion. Quantum Script strings are
  byte strings, so a UTF-8 file reads as UTF-8 text.
- The whole file is in memory: for big files, or line by line work, use
  `File` from `quantum-script--file`.
- `filePutContents` does not create folders: call
  `Shell.mkdirFilePath(file)` first.
- `filePutContents(file, value)` writes `value.toString()`: a number writes
  its digits, a `Buffer` its bytes.
- An empty file reads as `""` (a String), not `undefined`.

## Copying, renaming, removing files

| Function | Behavior |
|----------|----------|
| `Shell.copy(src, dst)` | copies one file, **overwrites** `dst`; the folder of `dst` must exist |
| `Shell.copyFile(src, dst)` | creates the folder of `dst` (`mkdirFilePath`), then `copy` |
| `Shell.rename(src, dst)` | renames / moves a file or a folder; see below |
| `Shell.remove(name)` | the C `remove`: a file or a link; on Linux also an **empty** directory |
| `Shell.removeFile(file)` | removes only when `fileExists(file)`: `false` for missing names and directories |
| `Shell.removeFileForce(file)` | `removeFile`, and if that fails on a read-only file, clears read-only and retries |
| `Shell.removeFileAndDirectoryIfEmpty(file)` | removes the file, then its folder if it became empty (one level only) |
| `Shell.touch(file)` | sets the access and modification time to now, **creates** an empty file if missing, never truncates; works on directories |
| `Shell.touchIfExists(file)` | `touch` only when `fileExists(file)` |

- `rename`: on Windows it **fails if `dst` exists**; on Linux it replaces
  `dst`. To replace on both: `Shell.removeFile(dst); Shell.rename(src, dst);`.
  Moving between file systems / drives may fail: copy, then remove.
- `copy` of a file onto itself fails (it would empty the file).
- On Linux `copy` keeps the permission bits of the source (an executable
  stays executable).
- `removeFileAndDirectoryIfEmpty` returns `true` only if both were removed:
  a file with no folder part is still removed but the result is `false`.

## Creating directories

| Function | Behavior |
|----------|----------|
| `Shell.mkdir(path)` | one level; `false` if it exists or the parent is missing |
| `Shell.mkdirRecursively(path)` | every missing level; `true` if it exists already |
| `Shell.mkdirRecursivelyIfNotExists(path)` | as `mkdirRecursively`, and `false` if `path` is a **file** |
| `Shell.mkdirFilePath(file)` | creates the folder that will hold `file`; `true` when `file` has no folder part |

`mkdirRecursivelyIfNotExists` is the one to use: idempotent, and a
directory created at the same time by a parallel build counts as success.
On Linux new directories get `0777` minus the `umask`.

## Removing directories

| Function | Removes | Result |
|----------|---------|--------|
| `Shell.rmdir(path)` | an **empty** directory | `false` if not empty / missing |
| `Shell.rmdirForce(path)` | as `rmdir`, clears read-only and retries | |
| `Shell.removeEmptyDir(path)` | `rmdir` when `isEmptyDir(path)` | |
| `Shell.removeEmptyDirRecursively(path)` | every empty sub folder (a folder holding only empty folders is empty), then `path` if it became empty | `true` only if `path` itself was removed |
| `Shell.removeDirContentRecursively(path)` | everything inside, keeps `path` | `true` also for a missing `path` |
| `Shell.removeDirRecursively(path)` | everything inside, then `path` | `false` for a missing `path` |
| `Shell.removeDirContentRecursivelyForce(path)` / `Shell.removeDirRecursivelyForce(path)` | as above, read-only files and folders are made writable and retried | |
| `Shell.removeFileRecursively(dir, pattern)` | files matching `pattern` in `dir` and every sub folder | `true` also when nothing matched |

- The recursive removals **stop at the first entry they cannot remove** and
  return `false`, leaving the rest in place. A file in use (Windows) or
  without permission stops them.
- **Links are not followed**: a link (Linux symbolic link, Windows junction
  or directory symbolic link) inside the tree is removed as a link, what it
  points to is not touched.
- To clean an output folder that may not exist yet:
  `Shell.removeDirRecursivelyForce("output");` and ignore the result, or
  test `directoryExists` first. Prefer the `Force` variants for build
  outputs: `git` checkouts and extracted archives contain read-only files on
  Windows.

## Listing

```javascript
Shell.getFileList("source/*.cpp");   // ["source/a.cpp", "source/b.cpp"]
Shell.getDirList("source/*");        // ["source/XYO"]
```

| Function | Returns |
|----------|---------|
| `Shell.getFileList(pattern)` | Array of the **files** (and links to files) matching `pattern` |
| `Shell.getDirList(pattern)` | Array of the **directories** (and links to directories) matching `pattern`, without `.` and `..` |

- Wildcards `*` and `?` are allowed in the **last** part of the name only
  (`"src/*/x.cpp"` does not work).
- Each entry is the folder part of the pattern, **as written**, plus the
  name: `"./*.txt"` gives `"./a.txt"`, `"*.txt"` gives `"a.txt"`.
- **Not recursive**. For a tree, recurse with `getDirList(dir + "/*")`
  (see [Recipes](recipes.md#walk-a-tree)) or use `ShellFind`.
- **Without a wildcard** the pattern is tested as one name:
  `getFileList("a.txt")` is `["a.txt"]` if the file exists, else `[]`;
  `getDirList("dir")` is `["dir"]`, **not** the folders inside `dir` (use
  `"dir/*"`).
- The order is the order the system returns: by name on NTFS, unspecified
  on Linux. Sort when it matters: `list = list.sort();` (Quantum Script
  `sort` returns a new array).
- Matching follows the system: case-insensitive on Windows
  (`FindFirstFile`, which can also match 8.3 short names: `*.htm` may match
  `.html` files), case-sensitive on Linux.

## Copying trees

| Function | Copies |
|----------|--------|
| `Shell.copyDirRecursively(src, dst)` | the **content** of `src` into `dst` (not `dst/<name of src>`), all levels; creates `dst`, merges into an existing one, overwrites files |
| `Shell.copyFilesToDirectory(source, target)` | the files directly in `source` (a folder, or a folder + pattern such as `"bin/*.dll"`), not sub folders; creates `target` |

- `copyDirRecursively` with a **missing** `src` returns `true` and creates an
  empty `dst`: test `directoryExists(src)` first if that matters.
- `copyDirRecursively` follows links to directories (copies what they point
  to, like `cp -rL`) but refuses a link that loops back into the tree
  (`false`).
- `copyFilesToDirectory` with a pattern **needs a folder part**: use
  `"./*.txt"`, not `"*.txt"` (a bare pattern is looked up in the current
  folder but copied from the root of the drive, and fails).
- Both stop at the first file that cannot be copied and return `false`.

## Platform differences

| Topic | Windows | Linux |
|-------|---------|-------|
| `rename` onto an existing name | fails | replaces |
| `remove` on an empty directory | fails | removes it |
| `getFileSize` of a directory | `-1` | the directory size |
| `realPath` of a missing name | an absolute name | `undefined` |
| `realPath` and links | not resolved | resolved |
| Wildcard matching | case-insensitive | case-sensitive |
| Separators | `/` and `\` | `/` |
| Read-only | the read-only attribute | no write permission for the user |
