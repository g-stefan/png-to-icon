//
// PNG To Icon
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef PNG_TO_ICON_VERSION_HPP
#define PNG_TO_ICON_VERSION_HPP

#define PNG_TO_ICON_VERSION_ABCD $VERSION_ABCD
#define PNG_TO_ICON_VERSION_STR "$VERSION_VERSION"
#define PNG_TO_ICON_VERSION_STR_BUILD "$VERSION_BUILD"
#define PNG_TO_ICON_VERSION_STR_DATETIME "$VERSION_DATETIME"

#ifndef XYO_RC

namespace PNGToIcon {
	namespace Version {
		const char *version();
		const char *build();
		const char *versionWithBuild();
		const char *datetime();
	};
};

#endif
#endif
