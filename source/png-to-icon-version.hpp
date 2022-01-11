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

#define PNG_TO_ICON_VERSION_ABCD                1,15,0,28
#define PNG_TO_ICON_VERSION_STR                 "1.15.0"
#define PNG_TO_ICON_VERSION_STR_BUILD           "28"
#define PNG_TO_ICON_VERSION_STR_DATETIME        "2022-01-09 01:13:07"

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

