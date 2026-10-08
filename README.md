# Quantum Script Extension Shell

Quantum Script extension
- A `Shell` object with the work of a shell or build script, one API for
Windows and Linux.
- Whole files as text or `Buffer` (`fileGetContents`, `filePutContents`),
file tests, copy, rename, remove, touch.
- Directories: create, list with wildcards, copy and remove whole trees.
- Path names: folder, file name, extension, current directory, absolute paths.
- Processes: run through the shell (`system`) or directly (`execute`), start
in the background and stop (`executeNoWait`, `terminateProcess`).
- Environment variables (`getenv`, `setenv`, `hasEnv`).
- Errors are return values (`false`, `undefined`, `-1`), not exceptions.

```javascript
Script.requireExtension("Shell");

Shell;
Shell.fileGetContents(file);
Shell.filePutContents(file,text);
Shell.fileGetContentsBuffer(file);
Shell.filePutContentsBuffer(file,buffer);
Shell.system(cmd);
Shell.getenv(name);
Shell.setenv(name,value);
Shell.hasEnv(name);
Shell.fileExists(name);
Shell.directoryExists(name);
Shell.chdir(path);
Shell.rmdir(path);
Shell.mkdir(path);
Shell.getcwd();
Shell.copy(src,dst);
Shell.rename(src,dst);
Shell.remove(file);
Shell.compareLastWriteTime(fileA,fileB);
Shell.touch(file);
Shell.is(what);
Shell.getFileName(fileName);
Shell.getFileExtension(fileName);
Shell.getFileBasename(fileName);
Shell.getFilePath(fileName);
Shell.getFilePathX(fileName);
Shell.execute(cmd);
Shell.executeHidden(cmd);
Shell.executeNoWait(cmd);
Shell.executeHiddenNoWait(cmd);
Shell.isProcessTerminated(id);
Shell.terminateProcess(id,timeout);
Shell.removeFile(file);
Shell.removeEmptyDir(dir);
Shell.removeFileAndDirectoryIfEmpty(file);
Shell.touchIfExists(file);
Shell.isEnv(name,value);
Shell.getFileList(fileName);
Shell.getDirList(fileName);
Shell.mkdirRecursively(dir);
Shell.removeEmptyDirRecursively(dir);
Shell.removeDirRecursively(dir);
Shell.copyDirRecursively(source,target);
Shell.removeFileRecursively(dir,file);
Shell.copyFilesToDirectory(source,target);
Shell.fileReplaceText(fileIn,fileOut,textInOut,lineMaxLength);
Shell.mkdirRecursivelyIfNotExists(path);
Shell.mkdirFilePath(file);
Shell.copyFile(source,target);
Shell.removeDirContentRecursively(dir);
Shell.realPath(path);
Shell.getFileSize(file);
Shell.removeFileForce(file);
Shell.rmdirForce(path);
Shell.removeDirContentRecursivelyForce(path);
Shell.removeDirRecursivelyForce(path);
Shell.isEmptyDir(name);
```

Built on `quantum-script`, `quantum-script--buffer` and
`quantum-script--shellfind`, part of the XYO C++ SDK.

## Documentation

- [Overview](docs/README.md) - purpose, concepts, known issues
- [Getting started](docs/getting-started.md) - build, load from a script, fabricare scripts, register in a C++ host, static builds, threads
- [Files and directories](docs/files-and-directories.md) - path names, tests, reading and writing, copy, remove, listing, platform differences
- [Processes and environment](docs/processes-and-environment.md) - `system` versus `execute`, exit codes, background processes, environment variables
- [Script API](docs/script-api.md) - every function: exact behavior, edge cases
- [Recipes](docs/recipes.md) - generated files, clean, install, incremental builds, walk a tree, run tools, servers
- [C++ API](docs/cpp-api.md) - registration, the `XYO::System::Shell` function behind each script function
- [API reference](docs/reference.md)

A Claude Code skill for this extension is in
[.claude/skills/quantum-script--shell](.claude/skills/quantum-script--shell/SKILL.md).

## License

Copyright (c) 2016-2026 Grigore Stefan
Licensed under the [MIT](LICENSE) license.
