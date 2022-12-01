// Quantum Script Extension Shell
// Copyright (c) 2022 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2022 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_EXTENSION_SHELL_LICENSE_HPP
#define XYO_QUANTUMSCRIPT_EXTENSION_SHELL_LICENSE_HPP

#ifndef XYO_QUANTUMSCRIPT_EXTENSION_SHELL_DEPENDENCY_HPP
#	include <XYO/QuantumScript.Extension/Shell/Dependency.hpp>
#endif

namespace XYO::QuantumScript::Extension::Shell::License {

	XYO_QUANTUMSCRIPT_EXTENSION_SHELL_EXPORT const char *licenseHeader();
	XYO_QUANTUMSCRIPT_EXTENSION_SHELL_EXPORT const char *licenseBody();
	XYO_QUANTUMSCRIPT_EXTENSION_SHELL_EXPORT const char *shortLicense();

};

#endif