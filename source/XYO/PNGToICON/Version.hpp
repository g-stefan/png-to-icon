// PNG to ICON
// Copyright (c) 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_PNGTOICON_VERSION_HPP
#define XYO_PNGTOICON_VERSION_HPP

#ifndef XYO_PNGTOICON_DEPENDENCY_HPP
#	include <XYO/PNGToICON/Dependency.hpp>
#endif

namespace XYO::PNGToICON::Version {

	const char *version();
	const char *build();
	const char *versionWithBuild();
	const char *datetime();

};

#endif
