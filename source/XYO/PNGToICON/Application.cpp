// PNG to ICON
// Copyright (c) 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <XYO/PNGToICON/Application.hpp>
#include <XYO/PNGToICON/Copyright.hpp>
#include <XYO/PNGToICON/License.hpp>
#include <XYO/PNGToICON/Version.hpp>

namespace XYO::PNGToICON {

	void Application::showUsage() {
		printf("png-to-icon - Convert PNG to ICON");
		showVersion();
		printf("%s\n\n", PNGToICON::Copyright::copyright());

		printf("Usage:\n");
		printf("\tpng-to-icon --license\n");
		printf("\tpng-to-icon [--level XX] [-background XXXXXX] in.png out.ico\n");
		printf("\tpng-to-icon [--level XX] [-background XXXXXX] --list in out.ico\n");
		printf("\tpng-to-icon [--level XX] [-background XXXXXX] --list in --list-x 16,24,32... out.ico\n");
		printf("\n");
	};

	void Application::showLicense() {
		printf("%s", XYO::Pixel32::License::license());
	};

	void Application::showVersion() {
		printf("version %s build %s [%s]\n", PNGToICON::Version::version(), PNGToICON::Version::build(), PNGToICON::Version::datetime());
	};

	void Application::initMemory() {
		String::initMemory();
		TDynamicArray<String>::initMemory();
	};

	int Application::main(int cmdN, char *cmdS[]) {
		char *inPng;
		char *outIco;
		int k;
		int optVar;
		bool optShowLicense;
		int optLevel;
		bool optUseBackgroundAlpha;
		Pixel optBackground;
		uint32_t optCR;
		uint32_t optCG;
		uint32_t optCB;
		bool optIsList;

		optVar = 0;
		optShowLicense = false;
		optLevel = 48;
		optUseBackgroundAlpha = false;
		optBackground = 0;
		optIsList = false;

		String listX;

		for (k = 1; k < cmdN; ++k) {
			if (strcmp(cmdS[k], "--license") == 0) {
				optShowLicense = true;
				continue;
			};
			if (strcmp(cmdS[k], "--list") == 0) {
				optIsList = true;
				continue;
			};
			if (strcmp(cmdS[k], "--list-x") == 0) {
				++k;
				if (k < cmdN) {
					listX = cmdS[k];
					continue;
				};
				showUsage();
				continue;
			};
			if (strcmp(cmdS[k], "--level") == 0) {
				++k;
				if (k < cmdN) {
					if (sscanf(cmdS[k], "%02X", &optLevel) != 1) {
						optLevel = 48;
					};
					continue;
				};
				showUsage();
				return 1;
			};
			if (strcmp(cmdS[k], "--background") == 0) {
				++k;
				if (k < cmdN) {
					if (sscanf(cmdS[k], "%02X%02X%02X", &optCR, &optCG, &optCB) != 3) {
						optCR = 0;
						optCG = 0;
						optCB = 0;
					};
					optBackground = XYO_PIXEL32_PIXEL(optCR, optCG, optCB, 0xFF);
					optUseBackgroundAlpha = true;
					continue;
				};
				showUsage();
				return 1;
			};
			if (optVar == 0) {
				inPng = cmdS[k];
				++optVar;
				continue;
			};
			if (optVar == 1) {
				outIco = cmdS[k];
				++optVar;
				continue;
			};
		};

		if (optShowLicense) {
			showLicense();
		};

		if (optVar < 2) {
			if (optShowLicense) {
				return 0;
			};
			showUsage();
			return 1;
		};

		if (optIsList) {

			if (listX.length() > 0) {
				TDynamicArray<String> resolutionList;
				if (String::explode(",", listX, resolutionList)) {

					for (k = 0; k < resolutionList.length(); ++k) {
						resolutionList[k] = String(inPng) + "-" + resolutionList[k] + ".png";
					};

					if (Process::pngToIcon(resolutionList, outIco, optLevel, optUseBackgroundAlpha, optBackground)) {
						return 0;
					};

					return 1;
				};
			};

			String file48;
			String file32;
			String file24;
			String file16;

			file48 = inPng;
			file48 << "-48.png";
			file32 = inPng;
			file32 << "-32.png";
			file24 = inPng;
			file24 << "-24.png";
			file16 = inPng;
			file16 << "-16.png";

			if (Process::pngToIcon(file48, file32, file24, file16, outIco, optLevel, optUseBackgroundAlpha, optBackground)) {
				return 0;
			};

			return 1;
		};

		if (Process::pngToIcon(inPng, outIco, optLevel, optUseBackgroundAlpha, optBackground)) {
			return 0;
		};

		return 1;
	};
};

#ifndef XYO_PNGTOICON_LIBRARY
XYO_APPLICATION_MAIN(XYO::PNGToICON::Application);
#endif
