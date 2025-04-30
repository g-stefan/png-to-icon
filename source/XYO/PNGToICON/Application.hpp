// PNG to ICON
// Copyright (c) 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_PNGTOICON_APPLICATION_HPP
#define XYO_PNGTOICON_APPLICATION_HPP

#ifndef XYO_PNGTOICON_DEPENDENCY_HPP
#	include <XYO/PNGToICON/Dependency.hpp>
#endif

namespace XYO::PNGToICON {

	class Application : public virtual IApplication {
			XYO_PLATFORM_DISALLOW_COPY_ASSIGN_MOVE(Application);

		public:
			inline Application(){};

			void showUsage();
			void showLicense();
			void showVersion();

			int main(int cmdN, char *cmdS[]);

			static void initMemory();
	};

};

#endif
