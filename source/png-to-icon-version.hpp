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

#define PNG_TO_ICON_VERSION_ABCD                1,13,0,22
#define PNG_TO_ICON_VERSION_STR                 "1.13.0"
#define PNG_TO_ICON_VERSION_STR_BUILD           "22"
#define PNG_TO_ICON_VERSION_STR_DATETIME        "2022-01-01 21:19:37"

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

