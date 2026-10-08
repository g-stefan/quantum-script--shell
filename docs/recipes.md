# Recipes

All examples start with:

```javascript
Script.requireExtension("Console");
Script.requireExtension("Shell");      // also loads Buffer and ShellFind
```

Quantum Script does not hoist function declarations: define a function
before the code that calls it.

## Write a generated file

```javascript
function writeFile(fileName, text) {
	if (!Shell.mkdirFilePath(fileName)) {
		throw "cannot create the folder of " + fileName;
	};
	if (!Shell.filePutContents(fileName, text)) {
		throw "cannot write " + fileName;
	};
};

writeFile("temp/include/Version.hpp",
	"// generated, do not edit\r\n" +
	"#define APP_VERSION \"1.2.3\"\r\n");
```

## Write only when the content changed

Keeps the modification time, so an incremental build does not rebuild
everything that includes the file.

```javascript
function updateFile(fileName, text) {
	if (Shell.fileGetContents(fileName) == text) {
		return false;
	};
	Shell.mkdirFilePath(fileName);
	return Shell.filePutContents(fileName, text);
};
```

## Replace text in a template

```javascript
if (!Shell.fileReplaceText("source/Version.Template.rh", "temp/Version.rh", [
	["$VERSION_ABCD", "1,2,3,0"],
	["$VERSION_STR", "1.2.3"]
], 0)) {                                // 0: default line size
	throw "cannot write temp/Version.rh";
};
```

The output must be another file than the template (the same name would
empty it). Builds up to 5.10.0 build 7 hang in `Shell.fileReplaceText`;
where such an SDK may still be installed, this does the same:

```javascript
function fileReplaceText(fileIn, fileOut, textInOut) {
	var text = Shell.fileGetContents(fileIn);
	if (Script.isUndefined(text)) {
		return false;
	};
	for (var k = 0; k < textInOut.length; ++k) {
		text = text.replace(textInOut[k][0], textInOut[k][1]);
	};
	return Shell.mkdirFilePath(fileOut) && Shell.filePutContents(fileOut, text);
};
```

## Clean build outputs

```javascript
var folders = ["output", "temp"];
for (var k = 0; k < folders.length; ++k) {
	if (Shell.directoryExists(folders[k])) {
		if (!Shell.removeDirRecursivelyForce(folders[k])) {
			throw "cannot remove " + folders[k] + " (a file in use?)";
		};
	};
};
```

The `Force` variant also removes read-only files (common in `git`
checkouts and extracted archives on Windows).

## Install into an SDK folder

```javascript
var sdk = Shell.getenv("HOME") + "/.my-sdk";   // HOME may be unset on Windows: test with Shell.hasEnv
if (!Shell.mkdirRecursivelyIfNotExists(sdk + "/bin")) {
	throw "cannot create " + sdk + "/bin";
};
Shell.copyFilesToDirectory("output/bin/*.dll", sdk + "/bin");
Shell.copyFilesToDirectory("output/bin/*.exe", sdk + "/bin");
Shell.copyDirRecursively("output/include", sdk + "/include");   // content of include/ into sdk/include
```

`copyFilesToDirectory` with a pattern needs a folder part (`"./*.dll"`, not
`"*.dll"`).

## Rebuild only when a source is newer

```javascript
function isChanged(target, sources) {
	if (!Shell.fileExists(target)) {
		return true;
	};
	for (var k = 0; k < sources.length; ++k) {
		if (Shell.compareLastWriteTime(target, sources[k]) < 0) {
			return true;
		};
	};
	return false;
};

var sources = Shell.getFileList("source/*.cpp");
if (isChanged("output/app.exe", sources)) {
	if (Shell.system("cl /nologo /Feoutput/app.exe " + sources.join(" ")) != 0) {
		throw "build failed";
	};
};
```

## Walk a tree

`getFileList` / `getDirList` are not recursive; recurse on the directories:

```javascript
function findFiles(dir, pattern, result) {
	var files = Shell.getFileList(dir + "/" + pattern);
	for (var k = 0; k < files.length; ++k) {
		result.push(files[k]);
	};
	var dirs = Shell.getDirList(dir + "/*");
	for (var m = 0; m < dirs.length; ++m) {
		findFiles(dirs[m], pattern, result);
	};
	return result;
};

var headers = findFiles("source", "*.hpp", []);
Console.writeLn(headers.sort().join("\n"));
```

`getDirList` returns links to directories too; a link that points back up
the tree would recurse forever. Use `ShellFind` (`isDirectory`, attributes)
when the tree may contain such links.

## Split and rebuild names

```javascript
var name = "source/XYO/App/Main.cpp";
var folder = Shell.getFilePath(name);                                // "source/XYO/App"
var file = Shell.getFileName(name);                                  // "Main.cpp"
var stem = Shell.getFileBasename(file);                              // "Main"  (of the file name!)
var objectFile = "temp/" + folder + "/" + stem + ".obj";             // "temp/source/XYO/App/Main.obj"
var sibling = Shell.getFilePathX(name) + "Main.hpp";                 // "source/XYO/App/Main.hpp"
```

## Run a tool and check the exit code

```javascript
function run(cmd) {
	Console.writeLn("> " + cmd);
	var code = Shell.system(cmd);
	if (code != 0) {
		throw "failed (" + code + "): " + cmd;
	};
};

run("git submodule update --init");
run("7z a -mx9 release/app.7z release/app");
```

## Capture the output of a command

```javascript
Shell.mkdirRecursivelyIfNotExists("temp");
if (Shell.system("git describe --tags > temp/describe.txt") == 0) {
	var version = Shell.fileGetContents("temp/describe.txt").trim();
	Shell.removeFile("temp/describe.txt");
};
```

For an interactive conversation with a child process (pipes) use
`ProcessInteractive` from `quantum-script--processinteractive`.

## Prepend a folder to PATH for child processes

```javascript
var separator = Shell.is("win") ? ";" : ":";
Shell.setenv("PATH", Shell.realPath("tools/bin") + separator + Shell.getenv("PATH"));
Shell.system("my-tool --version");    // found in tools/bin
```

`setenv` changes the environment of the whole process.

## Start a server, use it, stop it

```javascript
Script.requireExtension("Thread");    // CurrentThread.sleep

var id = Shell.executeHiddenNoWait("my-server --port 8080");
if (id == 0) {
	throw "cannot start my-server";
};
CurrentThread.sleep(1000);            // give it time to listen
var code = Shell.system("my-client --port 8080 --test");
if (!Shell.terminateProcess(id, 3000)) {
	Console.writeLn("cannot stop my-server");
};
```

## Wait for a background job with a time limit

```javascript
Script.requireExtension("Thread");

function waitProcess(id, milliseconds) {
	for (var waited = 0; waited < milliseconds; waited += 100) {
		if (Shell.isProcessTerminated(id)) {
			return true;
		};
		CurrentThread.sleep(100);
	};
	Shell.terminateProcess(id, 1000);    // too long: stop it
	return false;
};
```

## Remove a file and its folder when empty

```javascript
Shell.removeFileAndDirectoryIfEmpty("temp/lock/build.lock");   // also removes temp/lock if empty
Shell.removeEmptyDirRecursively("temp");                       // every empty folder left in temp, and temp
```
