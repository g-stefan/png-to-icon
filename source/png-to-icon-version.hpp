//
// Png To Icon
//
// Copyright (c) 2020-2021 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef PNG_TO_ICON_VERSION_HPP
#define PNG_TO_ICON_VERSION_HPP

#define PNG_TO_ICON_VERSION_ABCD                1,6,0,8
#define PNG_TO_ICON_VERSION_STR                 "1.6.0"
#define PNG_TO_ICON_VERSION_STR_BUILD           "8"
#define PNG_TO_ICON_VERSION_STR_DATETIME        "2021-07-06 15:11:46"

#ifndef XYO_RC

namespace PngToIcon {
	namespace Version {
		const char *version();
		const char *build();
		const char *versionWithBuild();
		const char *datetime();
	};
};

#endif
#endif

