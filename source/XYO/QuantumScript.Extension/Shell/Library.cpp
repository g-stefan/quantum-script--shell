// Quantum Script Extension Shell
// Copyright (c) 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <XYO/QuantumScript.Extension/Shell/Library.hpp>
#include <XYO/QuantumScript.Extension/Shell/Copyright.hpp>
#include <XYO/QuantumScript.Extension/Shell/License.hpp>
#include <XYO/QuantumScript.Extension/Shell/Version.hpp>
#include <XYO/QuantumScript.Extension/Buffer/VariableBuffer.hpp>

namespace XYO::QuantumScript::Extension::Shell {
	using namespace XYO::QuantumScript::Extension::Buffer;

	static TPointer<Variable> fileGetContents(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- shell-file-get-contents\n");
#endif
		String output;
		if (XYO::System::Shell::fileGetContents((arguments->index(0))->toString(), output)) {
			return VariableString::newVariable(output);
		};
		return Context::getValueUndefined();
	};

	static TPointer<Variable> filePutContents(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- shell-file-put-contents\n");
#endif
		return VariableBoolean::newVariable(XYO::System::Shell::filePutContents((arguments->index(0))->toString(), (arguments->index(1))->toString()));
	};

	static TPointer<Variable> fileGetContentsBuffer(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- shell-file-get-contents-buffer\n");
#endif
		TPointer<Variable> retV(VariableBuffer::newVariable());
		if (XYO::System::Shell::fileGetContents((arguments->index(0))->toString(), ((VariableBuffer *)(retV.value()))->buffer)) {
			return retV;
		};
		return Context::getValueUndefined();
	};

	static TPointer<Variable> filePutContentsBuffer(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- shell-file-put-contents-buffer\n");
#endif
		TPointerX<Variable> &buffer(arguments->index(1));
		if (TIsType<VariableBuffer>(buffer)) {
			return VariableBoolean::newVariable(XYO::System::Shell::filePutContents(
			    (arguments->index(0))->toString(),
			    ((VariableBuffer *)buffer.value())->buffer));
		};
		return Context::getValueUndefined();
	};

	static TPointer<Variable> system(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- shell-system\n");
#endif
		return VariableNumber::newVariable((Number)XYO::System::Shell::system((arguments->index(0))->toString()));
	};

	static TPointer<Variable> shell_getenv(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- shell-getenv\n");
#endif
		return VariableString::newVariable(XYO::System::Shell::getEnv((arguments->index(0))->toString()));
	};

	static TPointer<Variable> shell_setenv(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- shell-setenv\n");
#endif
		return VariableBoolean::newVariable(XYO::System::Shell::setenv((arguments->index(0))->toString(), (arguments->index(1))->toString()));
	};

	static TPointer<Variable> fileExists(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- shell-file-exists\n");
#endif

		return VariableBoolean::newVariable(XYO::System::Shell::fileExists((arguments->index(0))->toString()));
	};

	static TPointer<Variable> directoryExists(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- shell-directory-exists\n");
#endif

		return VariableBoolean::newVariable(XYO::System::Shell::directoryExists((arguments->index(0))->toString()));
	};

	static TPointer<Variable> chdir(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- shell-chdir\n");
#endif
		return VariableBoolean::newVariable(XYO::System::Shell::chdir((arguments->index(0))->toString()));
	};

	static TPointer<Variable> rmdir(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- shell-rmdir\n");
#endif
		return VariableBoolean::newVariable(XYO::System::Shell::rmdir((arguments->index(0))->toString()));
	};

	static TPointer<Variable> mkdir(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- shell-mkdir\n");
#endif
		return VariableBoolean::newVariable(XYO::System::Shell::mkdir((arguments->index(0))->toString()));
	};

	static TPointer<Variable> getcwd(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- shell-getcwd\n");
#endif
		return VariableString::newVariable(XYO::System::Shell::getCwd());
	};

	static TPointer<Variable> copy(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- shell-copy\n");
#endif

		return VariableBoolean::newVariable(XYO::System::Shell::copy(
		    (arguments->index(0))->toString(),
		    (arguments->index(1))->toString()));
	};

	static TPointer<Variable> rename(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- shell-rename\n");
#endif

		return VariableBoolean::newVariable(XYO::System::Shell::rename(
		    (arguments->index(0))->toString(),
		    (arguments->index(1))->toString()));
	};

	static TPointer<Variable> remove(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- shell-remove\n");
#endif

		return VariableBoolean::newVariable(XYO::System::Shell::remove((arguments->index(0))->toString()));
	};

	static TPointer<Variable> compareLastWriteTime(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- shell-compare-last-write-time\n");
#endif

		return VariableNumber::newVariable(
		    XYO::System::Shell::compareLastWriteTime(
		        (arguments->index(0))->toString(),
		        (arguments->index(1))->toString()));
	};

	static TPointer<Variable> touch(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- shell-touch\n");
#endif

		return VariableBoolean::newVariable(
		    XYO::System::Shell::touch(
		        (arguments->index(0))->toString()));
	};

	static TPointer<Variable> is(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- shell-is\n");
#endif
		String name = (arguments->index(0))->toString();

#ifdef XYO_OS_TYPE_WIN
		if (name == "win") {
			return VariableBoolean::newVariable(true);
		};
#endif

#ifdef XYO_OS_TYPE_UNIX
		if (name == "unix") {
			return VariableBoolean::newVariable(true);
		};
#endif

		return VariableBoolean::newVariable(false);
	};

	static TPointer<Variable> getFileName(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- shell-get-file-name\n");
#endif
		return VariableString::newVariable(XYO::System::Shell::getFileName((arguments->index(0))->toString()));
	};

	static TPointer<Variable> getFileExtension(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- shell-get-file-extension\n");
#endif
		return VariableString::newVariable(XYO::System::Shell::getFileExtension((arguments->index(0))->toString()));
	};

	static TPointer<Variable> getFileBasename(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- shell-get-file-basename\n");
#endif
		return VariableString::newVariable(XYO::System::Shell::getFileBasename((arguments->index(0))->toString()));
	};

	static TPointer<Variable> getFilePath(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- shell-get-file-path\n");
#endif
		return VariableString::newVariable(XYO::System::Shell::getFilePath((arguments->index(0))->toString()));
	};

	static TPointer<Variable> getFilePathX(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- shell-get-file-pathx\n");
#endif
		return VariableString::newVariable(XYO::System::Shell::getFilePathX((arguments->index(0))->toString()));
	};

	static TPointer<Variable> execute(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- shell-execute\n");
#endif
		return VariableNumber::newVariable(XYO::System::Shell::execute((arguments->index(0))->toString()));
	};

	static TPointer<Variable> executeHidden(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- shell-execute-hidden\n");
#endif
		return VariableNumber::newVariable(XYO::System::Shell::executeHidden((arguments->index(0))->toString()));
	};

	static TPointer<Variable> executeNoWait(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- shell-execute-no-wait\n");
#endif
		return VariableNumber::newVariable(XYO::System::Shell::executeNoWait((arguments->index(0))->toString()));
	};

	static TPointer<Variable> executeHiddenNoWait(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- shell-execute-hidden-no-wait\n");
#endif
		return VariableNumber::newVariable(XYO::System::Shell::executeHiddenNoWait((arguments->index(0))->toString()));
	};

	static TPointer<Variable> isProcessTerminated(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- shell-is-process-terminated\n");
#endif
		return VariableBoolean::newVariable(XYO::System::Shell::isProcessTerminated((XYO::System::Shell::ProcessId)((arguments->index(0))->toNumber())));
	};

	static TPointer<Variable> terminateProcess(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- shell-terminate-process\n");
#endif
		return VariableBoolean::newVariable(XYO::System::Shell::terminateProcess((XYO::System::Shell::ProcessId)((arguments->index(0)->toNumber())), (arguments->index(1))->toNumber()));
	};

	static TPointer<Variable> removeFile(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- shell-remove-file\n");
#endif
		return VariableBoolean::newVariable(XYO::System::Shell::removeFile((arguments->index(0))->toString()));
	};

	static TPointer<Variable> removeEmptyDir(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- shell-remove-empty-dir\n");
#endif
		return VariableBoolean::newVariable(XYO::System::Shell::removeEmptyDir((arguments->index(0))->toString()));
	};

	static TPointer<Variable> removeFileAndDirectoryIfEmpty(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- shell-remove-file-and-directory-if-empty\n");
#endif
		return VariableBoolean::newVariable(XYO::System::Shell::removeFileAndDirectoryIfEmpty((arguments->index(0))->toString()));
	};

	static TPointer<Variable> touchIfExists(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- shell-touch-if-exists\n");
#endif
		return VariableBoolean::newVariable(XYO::System::Shell::touchIfExists((arguments->index(0))->toString()));
	};

	static TPointer<Variable> isEnv(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- shell-is-env\n");
#endif
		return VariableBoolean::newVariable(XYO::System::Shell::isEnv((arguments->index(0))->toString(), (arguments->index(1))->toString()));
	};

	static TPointer<Variable> getFileList(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- shell-get-file-list\n");
#endif
		TPointer<VariableArray> retV;
		retV.newMemory();
		TDynamicArray<String> retVX;
		size_t k;
		//
		XYO::System::Shell::getFileList((arguments->index(0))->toString(), retVX);
		for (k = 0; k < retVX.length(); ++k) {
			(retV->index(k)) = VariableString::newVariable(retVX[k]);
		};
		//
		return retV.value();
	};

	static TPointer<Variable> getDirList(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- shell-get-dir-list\n");
#endif
		TPointer<VariableArray> retV;
		retV.newMemory();
		TDynamicArray<String> retVX;
		size_t k;
		//
		XYO::System::Shell::getDirList((arguments->index(0))->toString(), retVX);
		for (k = 0; k < retVX.length(); ++k) {
			(retV->index(k)) = VariableString::newVariable(retVX[k]);
		};
		//
		return retV.value();
	};

	static TPointer<Variable> mkdirRecursively(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- shell-mkdir-recursively\n");
#endif
		return VariableBoolean::newVariable(XYO::System::Shell::mkdirRecursively((arguments->index(0))->toString()));
	};

	static TPointer<Variable> removeEmptyDirRecursively(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- shell-remove-empty-dir-recursively\n");
#endif
		return VariableBoolean::newVariable(XYO::System::Shell::removeEmptyDirRecursively((arguments->index(0))->toString()));
	};

	static TPointer<Variable> removeDirContentRecursively(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- shell-remove-dir-content-recursively\n");
#endif
		return VariableBoolean::newVariable(XYO::System::Shell::removeDirContentRecursively((arguments->index(0))->toString()));
	};

	static TPointer<Variable> removeDirRecursively(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- shell-remove-dir-recursively\n");
#endif
		return VariableBoolean::newVariable(XYO::System::Shell::removeDirRecursively((arguments->index(0))->toString()));
	};

	static TPointer<Variable> removeFileForce(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- shell-remove-file-force\n");
#endif
		return VariableBoolean::newVariable(XYO::System::Shell::removeFileForce((arguments->index(0))->toString()));
	};

	static TPointer<Variable> rmdirForce(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- shell-rmdir-force\n");
#endif
		return VariableBoolean::newVariable(XYO::System::Shell::rmdirForce((arguments->index(0))->toString()));
	};

	static TPointer<Variable> removeDirContentRecursivelyForce(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- shell-remove-dir-content-recursively-force\n");
#endif
		return VariableBoolean::newVariable(XYO::System::Shell::removeDirContentRecursivelyForce((arguments->index(0))->toString()));
	};

	static TPointer<Variable> removeDirRecursivelyForce(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- shell-remove-dir-recursively-force\n");
#endif
		return VariableBoolean::newVariable(XYO::System::Shell::removeDirRecursivelyForce((arguments->index(0))->toString()));
	};

	static TPointer<Variable> copyDirRecursively(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- shell-copy-dir-recursively\n");
#endif
		return VariableBoolean::newVariable(XYO::System::Shell::copyDirRecursively((arguments->index(0))->toString(), (arguments->index(1))->toString()));
	};

	static TPointer<Variable> removeFileRecursively(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- shell-remove-file-recursively\n");
#endif
		return VariableBoolean::newVariable(XYO::System::Shell::removeFileRecursively((arguments->index(0))->toString(), (arguments->index(1))->toString()));
	};

	static TPointer<Variable> copyFilesToDirectory(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- shell-copy-files-to-directory\n");
#endif
		return VariableBoolean::newVariable(XYO::System::Shell::copyFilesToDirectory((arguments->index(0))->toString(), (arguments->index(1))->toString()));
	};

	static TPointer<Variable> mkdirRecursivelyIfNotExists(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- shell-mkdir-recursively-if-not-exists\n");
#endif
		return VariableBoolean::newVariable(XYO::System::Shell::mkdirRecursivelyIfNotExists((arguments->index(0))->toString()));
	};

	static TPointer<Variable> mkdirFilePath(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- shell-mkdir-file-path\n");
#endif
		return VariableBoolean::newVariable(XYO::System::Shell::mkdirFilePath((arguments->index(0))->toString()));
	};

	static TPointer<Variable> copyFile(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- shell-copy-file\n");
#endif
		return VariableBoolean::newVariable(XYO::System::Shell::copyFile((arguments->index(0))->toString(), (arguments->index(1))->toString()));
	};

	static TPointer<Variable> fileReplaceText(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- shell-file-replace-text\n");
#endif
		TDynamicArray<TDynamicArray<String>> textInOut;
		if (TIsType<VariableArray>(arguments->index(2))) {
			VariableArray *pairList = static_cast<VariableArray *>((arguments->index(2)).value());
			VariableArray *scan;
			size_t k;
			for (k = 0; k < pairList->value->length(); ++k) {
				if (TIsType<VariableArray>(pairList->index(k))) {
					scan = static_cast<VariableArray *>((pairList->index(k)).value());
					textInOut[k][0] = (scan->index(0))->toString();
					textInOut[k][1] = (scan->index(1))->toString();
				};
			};
		};

		return VariableBoolean::newVariable(XYO::System::Shell::fileReplaceText(
		    (arguments->index(0))->toString(),
		    (arguments->index(1))->toString(),
		    textInOut,
		    (arguments->index(2))->toIndex()));
	};

	static TPointer<Variable> realPath(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- shell-real-path\n");
#endif

		String retV;
		if (XYO::System::Shell::realPath(((arguments->index(0))->toString()).value(), retV)) {
			return VariableString::newVariable(retV);
		};

		return Context::getValueUndefined();
	};

	static TPointer<Variable> getFileSize(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- shell-get-file-size\n");
#endif
		int64_t size;
		if (XYO::System::Shell::getFileSize((arguments->index(0))->toString(), size)) {
			return VariableNumber::newVariable(size);
		};
		return VariableNumber::newVariable(-1);
	};

	static TPointer<Variable> hasEnv(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- shell-has-env\n");
#endif
		return VariableBoolean::newVariable(XYO::System::Shell::hasEnv((arguments->index(0))->toString()));
	};

	void registerInternalExtension(Executive *executive) {
		executive->registerInternalExtension("Shell", initExecutive);
	};

	void initExecutive(Executive *executive, void *extensionId) {

		String info = "Shell\r\n";
		info << License::shortLicense().c_str();

		executive->setExtensionName(extensionId, "Shell");
		executive->setExtensionInfo(extensionId, info);
		executive->setExtensionVersion(extensionId, Extension::Shell::Version::versionWithBuild());
		executive->setExtensionPublic(extensionId, true);

		executive->compileStringX("Script.requireExtension(\"Buffer\");");
		executive->compileStringX("Script.requireExtension(\"ShellFind\");");

		executive->compileStringX("var Shell={};");
		executive->setFunction2("Shell.fileGetContents(file)", fileGetContents);
		executive->setFunction2("Shell.filePutContents(file,text)", filePutContents);
		executive->setFunction2("Shell.fileGetContentsBuffer(file)", fileGetContentsBuffer);
		executive->setFunction2("Shell.filePutContentsBuffer(file,buffer)", filePutContentsBuffer);
		executive->setFunction2("Shell.system(cmd)", system);
		executive->setFunction2("Shell.getenv(name)", shell_getenv);
		executive->setFunction2("Shell.setenv(name,value)", shell_setenv);
		executive->setFunction2("Shell.fileExists(name)", fileExists);
		executive->setFunction2("Shell.directoryExists(name)", directoryExists);
		executive->setFunction2("Shell.chdir(path)", chdir);
		executive->setFunction2("Shell.rmdir(path)", rmdir);
		executive->setFunction2("Shell.mkdir(path)", mkdir);
		executive->setFunction2("Shell.getcwd()", getcwd);
		executive->setFunction2("Shell.copy(src,dst)", copy);
		executive->setFunction2("Shell.rename(src,dst)", rename);
		executive->setFunction2("Shell.remove(file)", remove);
		executive->setFunction2("Shell.compareLastWriteTime(fileA,fileB)", compareLastWriteTime);
		executive->setFunction2("Shell.touch(file)", touch);
		executive->setFunction2("Shell.is(what)", is);
		executive->setFunction2("Shell.getFileName(fileName)", getFileName);
		executive->setFunction2("Shell.getFileExtension(fileName)", getFileExtension);
		executive->setFunction2("Shell.getFileBasename(fileName)", getFileBasename);
		executive->setFunction2("Shell.getFilePath(fileName)", getFilePath);
		executive->setFunction2("Shell.getFilePathX(fileName)", getFilePathX);
		executive->setFunction2("Shell.execute(cmd)", execute);
		executive->setFunction2("Shell.executeHidden(cmd)", executeHidden);
		executive->setFunction2("Shell.executeNoWait(cmd)", executeNoWait);
		executive->setFunction2("Shell.executeHiddenNoWait(cmd)", executeHiddenNoWait);
		executive->setFunction2("Shell.isProcessTerminated(id)", isProcessTerminated);
		executive->setFunction2("Shell.terminateProcess(id,timeout)", terminateProcess);
		executive->setFunction2("Shell.removeFile(file)", removeFile);
		executive->setFunction2("Shell.removeEmptyDir(dir)", removeEmptyDir);
		executive->setFunction2("Shell.removeFileAndDirectoryIfEmpty(file)", removeFileAndDirectoryIfEmpty);
		executive->setFunction2("Shell.touchIfExists(file)", touchIfExists);
		executive->setFunction2("Shell.isEnv(name,value)", isEnv);
		executive->setFunction2("Shell.getFileList(path)", getFileList);
		executive->setFunction2("Shell.getDirList(path)", getDirList);
		executive->setFunction2("Shell.mkdirRecursively(path)", mkdirRecursively);
		executive->setFunction2("Shell.removeEmptyDirRecursively(path)", removeEmptyDirRecursively);
		executive->setFunction2("Shell.removeDirContentRecursively(path)", removeDirContentRecursively);
		executive->setFunction2("Shell.removeDirRecursively(path)", removeDirRecursively);
		executive->setFunction2("Shell.copyDirRecursively(source,target)", copyDirRecursively);
		executive->setFunction2("Shell.removeFileRecursively(dir,file)", removeFileRecursively);
		executive->setFunction2("Shell.copyFilesToDirectory(source,target)", copyFilesToDirectory);
		executive->setFunction2("Shell.mkdirRecursivelyIfNotExists(path)", mkdirRecursivelyIfNotExists);
		executive->setFunction2("Shell.mkdirFilePath(file)", mkdirFilePath);
		executive->setFunction2("Shell.copyFile(source,target)", copyFile);
		executive->setFunction2("Shell.fileReplaceText(fileIn,fileOut,textInOut,lineMaxLength)", fileReplaceText);
		executive->setFunction2("Shell.realPath(path)", realPath);
		executive->setFunction2("Shell.getFileSize(file)", getFileSize);
		executive->setFunction2("Shell.removeFileForce(file)", removeFileForce);
		executive->setFunction2("Shell.rmdirForce(path)", rmdirForce);
		executive->setFunction2("Shell.removeDirContentRecursivelyForce(path)", removeDirContentRecursivelyForce);
		executive->setFunction2("Shell.removeDirRecursivelyForce(path)", removeDirRecursivelyForce);
		executive->setFunction2("Shell.hasEnv(name)", hasEnv);
	};

};

#ifdef XYO_COMPILE_DYNAMIC_LIBRARY
extern "C" XYO_QUANTUMSCRIPT_EXTENSION_SHELL_EXPORT void quantumScriptExtension(XYO::QuantumScript::Executive *executive, void *extensionId) {
	XYO::QuantumScript::Extension::Shell::initExecutive(executive, extensionId);
};
#endif
