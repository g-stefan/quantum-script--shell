//
// Quantum Script Extension Shell
//
// Copyright (c) 2020-2021 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_EXTENSION_SHELL_HPP
#define QUANTUM_SCRIPT_EXTENSION_SHELL_HPP

#ifndef QUANTUM_SCRIPT_HPP
#include "quantum-script.hpp"
#endif

#ifndef QUANTUM_SCRIPT_EXTENSION_SHELL__EXPORT_HPP
#include "quantum-script-extension-shell--export.hpp"
#endif

#ifndef QUANTUM_SCRIPT_EXTENSION_SHELL_COPYRIGHT_HPP
#include "quantum-script-extension-shell-copyright.hpp"
#endif

#ifndef QUANTUM_SCRIPT_EXTENSION_SHELL_LICENSE_HPP
#include "quantum-script-extension-shell-license.hpp"
#endif

#ifndef QUANTUM_SCRIPT_EXTENSION_SHELL_VERSION_HPP
#include "quantum-script-extension-shell-version.hpp"
#endif

namespace Quantum {
	namespace Script {
		namespace Extension {
			namespace Shell {

				using namespace Quantum::Script;

				QUANTUM_SCRIPT_EXTENSION_SHELL_EXPORT void initExecutive(Executive *executive, void *extensionId);
				QUANTUM_SCRIPT_EXTENSION_SHELL_EXPORT void registerInternalExtension(Executive *executive);

			};
		};
	};
};

#endif

