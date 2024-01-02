// Quantum Script Extension Shell
// Copyright (c) 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_EXTENSION_SHELL_VERSION_HPP
#define XYO_QUANTUMSCRIPT_EXTENSION_SHELL_VERSION_HPP

#ifndef XYO_QUANTUMSCRIPT_EXTENSION_SHELL_DEPENDENCY_HPP
#	include <XYO/QuantumScript.Extension/Shell/Dependency.hpp>
#endif

namespace XYO::QuantumScript::Extension::Shell::Version {

	XYO_QUANTUMSCRIPT_EXTENSION_SHELL_EXPORT const char *version();
	XYO_QUANTUMSCRIPT_EXTENSION_SHELL_EXPORT const char *build();
	XYO_QUANTUMSCRIPT_EXTENSION_SHELL_EXPORT const char *versionWithBuild();
	XYO_QUANTUMSCRIPT_EXTENSION_SHELL_EXPORT const char *datetime();

};

#endif
