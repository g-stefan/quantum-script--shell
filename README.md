# Quantum Script Extension Shell

```javascript
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
Shell.copyFile(source,target);
Shell.removeDirContentRecursively(dir);
Shell.realPath(path);
Shell.getFileSize(file);
Shell.removeFileForce(file);
Shell.rmdirForce(path);
Shell.removeDirContentRecursivelyForce(path);
Shell.removeDirRecursivelyForce(path);
```

## License

Copyright (c) 2016-2023 Grigore Stefan
Licensed under the [MIT](LICENSE) license.
