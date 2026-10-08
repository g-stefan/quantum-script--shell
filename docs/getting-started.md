# Getting started

## 1. Build and install

The extension is built with [fabricare](https://github.com/g-stefan/fabricare),
the build tool used by all XYO C++ projects. `quantum-script` (and everything
below it: `xyo-system`, `xyo-encoding`, ...), `quantum-script--console` and
`quantum-script--buffer` must be installed to the SDK first, and
`quantum-script--shellfind` for the test. From the repository root:

```bash
fabricare make       # build into output/
fabricare test       # build and run test/test.01 (run make first)
fabricare install    # copy output/{bin,include,lib} to ~/.fabricare/<platform>
fabricare clean      # remove output/ and temp/
```

Two libraries are produced:

| Project                        | Kind                                | Use it when                                         |
|--------------------------------|-------------------------------------|-----------------------------------------------------|
| `quantum-script--shell`        | DLL / shared library (`dll-or-lib`) | scripts run by `quantum-script`, or a host using the engine DLL |
| `quantum-script--shell.static` | static library, static CRT          | self-contained hosts built with `quantum-script.static` |

After `fabricare install`, `quantum-script--shell.dll` (Windows) /
`libquantum-script--shell.so` (Linux) sits in the SDK `bin` folder next to
`quantum-script.exe`, `quantum-script--buffer.dll` and
`quantum-script--shellfind.dll`, which is where
`Script.requireExtension("Shell")` finds them.

## 2. Use it from a script

```javascript
Script.requireExtension("Console");
Script.requireExtension("Shell");

if (!Shell.mkdirRecursivelyIfNotExists("output/report")) {
	throw "cannot create output/report";
};

var sources = Shell.getFileList("source/*.cpp");
var text = "";
for (var k = 0; k < sources.length; ++k) {
	text += Shell.getFileName(sources[k]) + " " + Shell.getFileSize(sources[k]) + "\r\n";
};
Shell.filePutContents("output/report/sources.txt", text);

if (Shell.system("git --version") != 0) {
	Console.writeLn("git not found");
};
```

Run it with:

```bash
quantum-script report.js
```

`Script.requireExtension("Shell")` looks for an external
`quantum-script--shell` library first (the file as named, then every include
path folder: next to the interpreter, next to the script), then for an
internal extension registered by the host. Loading twice does nothing. A
missing extension throws `Unable to open "Shell"`.

Loading `Shell` also runs `Script.requireExtension("Buffer")` and
`Script.requireExtension("ShellFind")`, so the `quantum-script--buffer` and
`quantum-script--shellfind` libraries must be available too; the `Buffer`
and `ShellFind` globals exist afterwards.

Relative names are relative to the **process current directory**
(`Shell.getcwd()`), not to the folder of the script. `quantum-script` does
not change the current directory; run it from the folder the script
expects, or `Shell.chdir` first.

## 3. fabricare build scripts

`fabricare` registers `Shell` (with `Buffer`, `ShellFind`, `File`, ...) as
an internal extension and loads it before running the build scripts, so
`fabricare/*.js` scripts use `Shell` directly:

```javascript
// fabricare/release.js
Shell.removeDirRecursivelyForce("release/my-tool");
Shell.copyFile("output/bin/my-tool.exe", "release/my-tool/bin/my-tool.exe");   // creates the folders
if (Shell.system("7z a -mx9 release/my-tool.7z release/my-tool") != 0) {
	throw "7z failed";
};
```

fabricare also defines `OS.isWindows()`, `OS.isLinux()`, `OS.isMinGW()`,
`OS.isEmscripten()`; they tell MinGW and Emscripten apart, which
`Shell.is("win")` / `Shell.is("unix")` do not (see
[Script API](script-api.md#shelliswhat)).

## 4. Register it in a C++ host

A host that embeds Quantum Script makes `Shell` available as an internal
extension by registering it, together with `Buffer` and `ShellFind`, in the
init callback (this is what `test/test.01.cpp` does):

```cpp
#include <XYO/QuantumScript.hpp>
#include <XYO/QuantumScript.Extension/Console.hpp>
#include <XYO/QuantumScript.Extension/Buffer.hpp>
#include <XYO/QuantumScript.Extension/Shell.hpp>
#include <XYO/QuantumScript.Extension/ShellFind.hpp>

using namespace XYO::QuantumScript;

void initExecutive(Executive *executive) {
	Extension::Console::registerInternalExtension(executive);
	Extension::Buffer::registerInternalExtension(executive);      // Shell requires Buffer
	Extension::Shell::registerInternalExtension(executive);
	Extension::ShellFind::registerInternalExtension(executive);   // Shell requires ShellFind
};

int main(int cmdN, char *cmdS[]) {
	if (ExecutiveX::initExecutive(cmdN, cmdS, initExecutive)) {
		if (!ExecutiveX::executeFile("main.js")) {
			printf("%s\n", (ExecutiveX::getError()).value());
			printf("%s", (ExecutiveX::getStackTrace()).value());
		};
		ExecutiveX::endProcessing();
	};
	return 0;
};
```

Registering only makes the extension *available*: scripts still call
`Script.requireExtension("Shell")`. With the DLL build of the engine an
external `quantum-script--shell.dll` found on the include path wins over
the internal one for `requireExtension`; use
`Script.requireInternalExtension("Shell")` to force the internal one.

In the host's `fabricare.json`:

```json
{
	"name": "my-host",
	"make": "exe",
	"sourcePath": "XYO/MyHost",
	"dependency": [
		"quantum-script--shell",
		"quantum-script--shellfind"
	]
}
```

`quantum-script--shell` brings `quantum-script`, `quantum-script--console`
and `quantum-script--buffer` with it. It does **not** list
`quantum-script--shellfind` (it only loads it at run time), so add it
yourself when the host registers `ShellFind` or ships its DLL.

## 5. Static builds

For a self-contained executable depend on the static variants and the static
CRT:

```json
{
	"name": "my-host.static",
	"make": "exe",
	"sourcePath": "XYO/MyHost",
	"crt": "static",
	"dependency": [
		"quantum-script--shell.static",
		"quantum-script--shellfind.static"
	]
}
```

`quantum-script--shell.static` defines
`XYO_QUANTUMSCRIPT_EXTENSION_SHELL_LIBRARY` for its users (empty export
macro, no `quantumScriptExtension` entry point). There is no DLL to find,
so the host **must** register `Buffer`, `Shell` and `ShellFind` as internal
extensions (section 4).

## 6. Threads and process wide state

Each thread that runs scripts has its own engine and must require `Shell`
itself. The functions themselves can be used from any thread, but some
change **process wide** state shared by all threads and all engines:

- `Shell.chdir(path)` changes the current directory, and with it how every
  relative name is resolved, in every thread.
- `Shell.setenv(name, value)` changes the environment of the process (and of
  the processes it starts later).

Set them once at start-up, or pass absolute paths
(`Shell.realPath(name)`) to worker threads.
