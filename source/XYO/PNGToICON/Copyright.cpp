// PNG to ICON
// Copyright (c) 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <XYO/PNGToICON/Copyright.hpp>
#include <XYO/PNGToICON/Copyright.rh>

namespace XYO::PNGToICON::Copyright {

	static const char *copyright_ = XYO_PNGTOICON_COPYRIGHT;
	static const char *publisher_ = XYO_PNGTOICON_PUBLISHER;
	static const char *company_ = XYO_PNGTOICON_COMPANY;
	static const char *contact_ = XYO_PNGTOICON_CONTACT;

	const char *copyright() {
		return copyright_;
	};

	const char *publisher() {
		return publisher_;
	};

	const char *company() {
		return company_;
	};

	const char *contact() {
		return contact_;
	};

};
