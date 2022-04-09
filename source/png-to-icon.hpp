//
// PNG To Icon
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef PNG_TO_ICON_HPP

namespace PNGToIcon {
	using namespace XYO;
	using namespace XYO::Pixel32;

	class Application : public virtual IMain {
			XYO_DISALLOW_COPY_ASSIGN_MOVE(Application);

		protected:
			void showUsage();
			void showLicense();
			void showVersion();

		public:
			inline Application(){};
			int main(int cmdN, char *cmdS[]);
	};

};

#endif
