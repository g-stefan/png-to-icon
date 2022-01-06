//
// PNG To Icon
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef PNG_TO_ICON_COPYRIGHT_HPP
#define PNG_TO_ICON_COPYRIGHT_HPP

#define PNG_TO_ICON_COPYRIGHT            "Copyright (c) Grigore Stefan."
#define PNG_TO_ICON_PUBLISHER            "Grigore Stefan"
#define PNG_TO_ICON_COMPANY              PNG_TO_ICON_PUBLISHER
#define PNG_TO_ICON_CONTACT              "g_stefan@yahoo.com"
#define PNG_TO_ICON_FULL_COPYRIGHT       PNG_TO_ICON_COPYRIGHT " <" PNG_TO_ICON_CONTACT ">"

#ifndef XYO_RC

namespace PNGToIcon {
	namespace Copyright {
		const char *copyright();
		const char *publisher();
		const char *company();
		const char *contact();
		const char *fullCopyright();
	};
};

#endif
#endif
