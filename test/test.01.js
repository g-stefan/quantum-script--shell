// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

Script.requireExtension("Console");
Script.requireExtension("Shell");

function check(condition, name) {
	if (!condition) {
		throw "Test failed: " + name;
	};
	Console.writeLn("- " + name + ": ok");
};

// run from output/test
var path = "../../temp/test.01";
Shell.removeDirRecursivelyForce(path);
check(Shell.mkdirRecursivelyIfNotExists(path), "mkdirRecursivelyIfNotExists");

// Shell.is
var isWindows = Shell.hasEnv("WINDIR");
check(Shell.is("win") == isWindows, "is(\"win\")");
check(Shell.is("unix") == !isWindows, "is(\"unix\")");
check(!Shell.is("other"), "is(\"other\")");

// Shell.fileReplaceText
var fileIn = path + "/in.txt";
check(Shell.filePutContents(fileIn, "Hello NAME\r\nVersion VERSION, NAME\r\nlast line"), "filePutContents");

check(Shell.fileReplaceText(fileIn, path + "/out/a.txt", [["NAME", "World"], ["VERSION", "1.2.3"]], 1024), "fileReplaceText");
check(Shell.fileGetContents(path + "/out/a.txt") == "Hello World\r\nVersion 1.2.3, World\r\nlast line", "fileReplaceText content");

check(Shell.fileReplaceText(fileIn, path + "/out/b.txt", [["NAME", "VERSION"], ["VERSION", "1"]]), "fileReplaceText without lineMaxLength");
check(Shell.fileGetContents(path + "/out/b.txt") == "Hello 1\r\nVersion 1, 1\r\nlast line", "fileReplaceText pairs in order");

check(Shell.fileReplaceText(fileIn, path + "/out/c.txt", [], 4), "fileReplaceText small lineMaxLength");
check(Shell.fileGetContents(path + "/out/c.txt") == Shell.fileGetContents(fileIn), "fileReplaceText small lineMaxLength content");

check(!Shell.fileReplaceText(path + "/missing.txt", path + "/out/d.txt", [], 1024), "fileReplaceText missing input");

Shell.removeDirRecursivelyForce(path);
