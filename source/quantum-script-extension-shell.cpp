//
// Quantum Script Extension Shell
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "quantum-script-extension-shell-license.hpp"
#include "quantum-script-extension-shell.hpp"
#ifndef QUANTUM_SCRIPT_EXTENSION_SHELL_NO_VERSION
#include "quantum-script-extension-shell-version.hpp"
#endif

#include "quantum-script-variableboolean.hpp"
#include "quantum-script-variablenumber.hpp"
#include "quantum-script-variablestring.hpp"
#include "quantum-script-extension-buffer-variablebuffer.hpp"

//#define QUANTUM_SCRIPT_VM_DEBUG_RUNTIME

namespace Quantum {
	namespace Script {
		namespace Extension {
			namespace Shell {

				using namespace XYO;
				using namespace Quantum::Script;
				typedef Extension::Buffer::VariableBuffer VariableBuffer;

				static TPointer<Variable> fileGetContents(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- shell-file-get-contents\n");
#endif
					String output;
					if(XYO::Shell::fileGetContents((arguments->index(0))->toString(), output)) {
						return VariableString::newVariable(output);
					};
					return Context::getValueUndefined();
				};

				static TPointer<Variable> filePutContents(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- shell-file-put-contents\n");
#endif
					return VariableBoolean::newVariable(XYO::Shell::filePutContents((arguments->index(0))->toString(), (arguments->index(1))->toString()));
				};

				static TPointer<Variable> fileGetContentsBuffer(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- shell-file-get-contents-buffer\n");
#endif
					TPointer<Variable> retV(VariableBuffer::newVariable());
					if(XYO::Shell::fileGetContents((arguments->index(0))->toString(), ((VariableBuffer *)(retV.value()))->buffer)) {
						return retV;
					};
					return Context::getValueUndefined();
				};

				static TPointer<Variable> filePutContentsBuffer(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- shell-file-put-contents-buffer\n");
#endif
					TPointerX<Variable> &buffer(arguments->index(1));
					if(TIsType<VariableBuffer>(buffer)) {
						return VariableBoolean::newVariable(XYO::Shell::filePutContents(
									(arguments->index(0))->toString(),
									((VariableBuffer *)buffer.value())->buffer));
					};
					return Context::getValueUndefined();
				};

				static TPointer<Variable> system(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- shell-system\n");
#endif
					return VariableNumber::newVariable((Number)XYO::Shell::system( (arguments->index(0))->toString() ));
				};

				static TPointer<Variable> shell_getenv(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- shell-getenv\n");
#endif
					return VariableString::newVariable(XYO::Shell::getEnv((arguments->index(0))->toString()));
				};

				static TPointer<Variable> shell_setenv(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- shell-setenv\n");
#endif
					return VariableBoolean::newVariable(XYO::Shell::setenv( (arguments->index(0))->toString(), (arguments->index(1))->toString()));
				};


				static TPointer<Variable> fileExists(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- shell-file-exists\n");
#endif

					return VariableBoolean::newVariable(XYO::Shell::fileExists( (arguments->index(0))->toString() ));
				};


				static TPointer<Variable> directoryExists(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- shell-directory-exists\n");
#endif

					return VariableBoolean::newVariable(XYO::Shell::directoryExists( (arguments->index(0))->toString() ));
				};

				static TPointer<Variable> chdir(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- shell-chdir\n");
#endif
					return VariableBoolean::newVariable(XYO::Shell::chdir( (arguments->index(0))->toString() ));
				};

				static TPointer<Variable> rmdir(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- shell-rmdir\n");
#endif
					return VariableBoolean::newVariable(XYO::Shell::rmdir( (arguments->index(0))->toString() ));
				};

				static TPointer<Variable> mkdir(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- shell-mkdir\n");
#endif
					return VariableBoolean::newVariable(XYO::Shell::mkdir( (arguments->index(0))->toString() ));
				};


				static TPointer<Variable> getcwd(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- shell-getcwd\n");
#endif
					return VariableString::newVariable(XYO::Shell::getCwd());
				};


				static TPointer<Variable> copy(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- shell-copy\n");
#endif

					return VariableBoolean::newVariable(XYO::Shell::copy(
								(arguments->index(0))->toString(),
								(arguments->index(1))->toString()
							));
				};


				static TPointer<Variable> rename(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- shell-rename\n");
#endif

					return VariableBoolean::newVariable(XYO::Shell::rename(
								(arguments->index(0))->toString(),
								(arguments->index(1))->toString()
							));
				};

				static TPointer<Variable> remove(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- shell-remove\n");
#endif

					return VariableBoolean::newVariable(XYO::Shell::remove( (arguments->index(0))->toString() ));
				};

				static TPointer<Variable> compareLastWriteTime(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- shell-compare-last-write-time\n");
#endif

					return VariableNumber::newVariable(
							XYO::Shell::compareLastWriteTime(
								(arguments->index(0))->toString(),
								(arguments->index(1))->toString()
							)
						);
				};

				static TPointer<Variable> touch(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- shell-touch\n");
#endif

					return VariableBoolean::newVariable(
							XYO::Shell::touch(
								(arguments->index(0))->toString()
							)
						);
				};

				static TPointer<Variable> is(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
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
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- shell-get-file-name\n");
#endif
					return VariableString::newVariable(XYO::Shell::getFileName((arguments->index(0))->toString()));
				};

				static TPointer<Variable> getFileExtension(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- shell-get-file-extension\n");
#endif
					return VariableString::newVariable(XYO::Shell::getFileExtension((arguments->index(0))->toString()));
				};

				static TPointer<Variable> getFileBasename(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- shell-get-file-basename\n");
#endif
					return VariableString::newVariable(XYO::Shell::getFileBasename((arguments->index(0))->toString()));
				};

				static TPointer<Variable> getFilePath(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- shell-get-file-path\n");
#endif
					return VariableString::newVariable(XYO::Shell::getFilePath((arguments->index(0))->toString()));
				};

				static TPointer<Variable> getFilePathX(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- shell-get-file-pathx\n");
#endif
					return VariableString::newVariable(XYO::Shell::getFilePathX((arguments->index(0))->toString()));
				};

				static TPointer<Variable> execute(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- shell-execute\n");
#endif
					return VariableNumber::newVariable(XYO::Shell::execute((arguments->index(0))->toString()));
				};

				static TPointer<Variable> executeHidden(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- shell-execute-hidden\n");
#endif
					return VariableNumber::newVariable(XYO::Shell::executeHidden((arguments->index(0))->toString()));
				};

				static TPointer<Variable> executeNoWait(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- shell-execute-no-wait\n");
#endif
					return VariableNumber::newVariable(XYO::Shell::executeNoWait((arguments->index(0))->toString()));
				};

				static TPointer<Variable> executeHiddenNoWait(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- shell-execute-hidden-no-wait\n");
#endif
					return VariableNumber::newVariable(XYO::Shell::executeHiddenNoWait((arguments->index(0))->toString()));
				};

				static TPointer<Variable> isProcessTerminated(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- shell-is-process-terminated\n");
#endif
					return VariableBoolean::newVariable(XYO::Shell::isProcessTerminated((XYO::Shell::ProcessId)((arguments->index(0))->toNumber())));
				};

				static TPointer<Variable> terminateProcess(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- shell-terminate-process\n");
#endif
					return VariableBoolean::newVariable(XYO::Shell::terminateProcess((XYO::Shell::ProcessId)((arguments->index(0)->toNumber())), (arguments->index(1))->toNumber()));
				};

				static TPointer<Variable> removeFile(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- shell-remove-file\n");
#endif
					return VariableBoolean::newVariable(XYO::Shell::removeFile((arguments->index(0))->toString()));
				};

				static TPointer<Variable> removeEmptyDir(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- shell-remove-empty-dir\n");
#endif
					return VariableBoolean::newVariable(XYO::Shell::removeEmptyDir((arguments->index(0))->toString()));
				};

				static TPointer<Variable> removeFileAndDirectoryIfEmpty(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- shell-remove-file-and-directory-if-empty\n");
#endif
					return VariableBoolean::newVariable(XYO::Shell::removeFileAndDirectoryIfEmpty((arguments->index(0))->toString()));
				};

				static TPointer<Variable> touchIfExists(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- shell-touch-if-exists\n");
#endif
					return VariableBoolean::newVariable(XYO::Shell::touchIfExists((arguments->index(0))->toString()));
				};

				static TPointer<Variable> isEnv(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- shell-is-env\n");
#endif
					return VariableBoolean::newVariable(XYO::Shell::isEnv((arguments->index(0))->toString(), (arguments->index(1))->toString()));
				};


				static TPointer<Variable> getFileList(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- shell-get-file-list\n");
#endif
					TPointer<VariableArray> retV;
					retV.newMemory();
					TDynamicArray<String> retVX;
					size_t k;
					//
					XYO::Shell::getFileList((arguments->index(0))->toString(), retVX);
					for(k = 0; k < retVX.length(); ++k) {
						(retV->index(k))=VariableString::newVariable(retVX[k]);
					};
					//
					return retV.value();
				};

				static TPointer<Variable> getDirList(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- shell-get-dir-list\n");
#endif
					TPointer<VariableArray> retV;
					retV.newMemory();
					TDynamicArray<String> retVX;
					size_t k;
					//
					XYO::Shell::getDirList((arguments->index(0))->toString(), retVX);
					for(k = 0; k < retVX.length(); ++k) {
						(retV->index(k))=VariableString::newVariable(retVX[k]);
					};
					//
					return retV.value();
				};

				static TPointer<Variable> mkdirRecursively(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- shell-mkdir-recursively\n");
#endif
					return VariableBoolean::newVariable(XYO::Shell::mkdirRecursively((arguments->index(0))->toString()));
				};

				static TPointer<Variable> removeEmptyDirRecursively(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- shell-remove-empty-dir-recursively\n");
#endif
					return VariableBoolean::newVariable(XYO::Shell::removeEmptyDirRecursively((arguments->index(0))->toString()));
				};

				static TPointer<Variable> removeDirContentRecursively(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- shell-remove-dir-content-recursively\n");
#endif
					return VariableBoolean::newVariable(XYO::Shell::removeDirContentRecursively((arguments->index(0))->toString()));
				};

				static TPointer<Variable> removeDirRecursively(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- shell-remove-dir-recursively\n");
#endif
					return VariableBoolean::newVariable(XYO::Shell::removeDirRecursively((arguments->index(0))->toString()));
				};

				static TPointer<Variable> copyDirRecursively(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- shell-copy-dir-recursively\n");
#endif
					return VariableBoolean::newVariable(XYO::Shell::copyDirRecursively((arguments->index(0))->toString(), (arguments->index(1))->toString()));
				};

				static TPointer<Variable> removeFileRecursively(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- shell-remove-file-recursively\n");
#endif
					return VariableBoolean::newVariable(XYO::Shell::removeFileRecursively((arguments->index(0))->toString(), (arguments->index(1))->toString()));
				};

				static TPointer<Variable> copyFilesToDirectory(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- shell-copy-files-to-directory\n");
#endif
					return VariableBoolean::newVariable(XYO::Shell::copyFilesToDirectory((arguments->index(0))->toString(), (arguments->index(1))->toString()));
				};

				static TPointer<Variable> mkdirRecursivelyIfNotExists(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- shell-mkdir-recursively-if-not-exists\n");
#endif
					return VariableBoolean::newVariable(XYO::Shell::mkdirRecursivelyIfNotExists((arguments->index(0))->toString()));
				};

				static TPointer<Variable> mkdirFilePath(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- shell-mkdir-file-path\n");
#endif
					return VariableBoolean::newVariable(XYO::Shell::mkdirFilePath((arguments->index(0))->toString()));
				};

				static TPointer<Variable> copyFile(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- shell-copy-file\n");
#endif
					return VariableBoolean::newVariable(XYO::Shell::copyFile((arguments->index(0))->toString(), (arguments->index(1))->toString()));
				};

				static TPointer<Variable> fileReplaceText(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- shell-file-replace-text\n");
#endif
					TDynamicArray<TDynamicArray<String> > textInOut;
					if(TIsType<VariableArray>(arguments->index(2))) {
						VariableArray *pairList = static_cast<VariableArray *>((arguments->index(2)).value());
						VariableArray *scan;
						size_t k;
						for(k = 0; k < pairList->value->length(); ++k) {
							if(TIsType<VariableArray>(pairList->index(k))) {
								scan = static_cast<VariableArray *>((pairList->index(k)).value());
								textInOut[k][0] = (scan->index(0))->toString();
								textInOut[k][1] = (scan->index(1))->toString();
							};
						};
					};

					return VariableBoolean::newVariable(XYO::Shell::fileReplaceText(
								(arguments->index(0))->toString(),
								(arguments->index(1))->toString(),
								textInOut,
								(arguments->index(2))->toIndex()
							));
				};

				void registerInternalExtension(Executive *executive) {
					executive->registerInternalExtension("Shell", initExecutive);
				};

				void initExecutive(Executive *executive, void *extensionId) {

					String info = "Shell\r\n";
					info << License::shortContent();

					executive->setExtensionName(extensionId, "Shell");
					executive->setExtensionInfo(extensionId, info);
#ifndef QUANTUM_SCRIPT_EXTENSION_SHELL_NO_VERSION
					executive->setExtensionVersion(extensionId, Extension::Shell::Version::versionWithBuild());
#endif
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
				};

			};
		};
	};
};

#ifdef XYO_COMPILE_DYNAMIC_LIBRARY
extern "C" QUANTUM_SCRIPT_EXTENSION_SHELL_EXPORT void quantumScriptExtension(Quantum::Script::Executive *executive, void *extensionId) {
	Quantum::Script::Extension::Shell::initExecutive(executive, extensionId);
};
#endif

