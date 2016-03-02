#include <iostream>
#include <cstring>
#include "hicolor.hpp"

const char * appName = "hicolor";
const char * appVersion = "1.0.0";

const uint16_t R_mask = 0x7c00;
const uint16_t G_mask = 0x03e0;
const uint16_t B_mask = 0x001f;

const unsigned int paletteSize = 256;

const unsigned int maxColor = 31;
const char * ASCII = "P3";
const char * BIN = "P6";

using namespace std;

void printHelp() {
	cerr << "Utility to convert images from *.ppm (Portable PixMap) to *.HI (HIcolor) and back." << endl;
	cerr << endl;
	cerr << "Version:  " << appVersion << endl;
	cerr << "Syntax:   " << appName << " [options] [file ...]" << endl;
	cerr << endl;
	cerr << "Examples: " << appName << " LOADING.HI" << endl;
	cerr << "          " << appName << " -ap LOADING.HI DLG_ZLAT.ppm DLG_MAPA.ppm" << endl;
	cerr << "          " << appName << " --palette DLG_DEMO.ppm" << endl;
	cerr << endl;
	cerr << "Options:  -a --ascii     PPM images will be in ASCII format (magic number \"P3\"). Default is binary (\"P6\")." << endl;
	cerr << "          -p --palette   Attempt to save images in indexed colors (for HIcolor images)." << endl;
	cerr << endl;
	cerr << "2015 - Matej Pokorny <pokorny.matej@gmail.com>" << endl;
}

void parseArgs(int argc, char const * argv[], char const * format[], Type * type, char const *** files, size_t * n) {
	if (argc < 2) {
		printHelp();
		exit(1);
	}

	int i;
	*format = BIN;
	*type = Data;

	for (i = 1; i < argc; i++) {
		// konec přepínačů
		if (argv[i][0] != '-') break;

		if (!strcmp(argv[i], "-a") || !strcmp(argv[i], "--ascii"))
			*format = ASCII;
		else if (!strcmp(argv[i], "-p") || !strcmp(argv[i], "--palette"))
			*type = Palette;
		else if (!strcmp(argv[i], "-ap") || !strcmp(argv[i], "-pa")) {
			*type = Palette;
			*format = ASCII;
		}
		else {
			cerr << appName << ": unrecognized option '" << argv[i] << '\'' << endl;
			exit(1);
		}
	}

	*n = argc - i;

	if (*n == 0) {
		cerr << appName << ": no input files" << endl;
		exit(1);
	}

	*files = argv + i;
}

int main(int argc, char const * argv[]) {
	const char * format;
	char const ** files;
	Type type;
	size_t n;

	parseArgs(argc, argv, &format, &type, &files, &n);

	for (size_t i = 0; i < n; i++) {
		size_t length = strlen(files[i]);

		if (length > 4 && (!strcmp(files[i] + length - 4, ".ppm") || !strcmp(files[i] + length - 4, ".PPM")))
			ppm2hi(files[i], (string(files[i], length - 4) + ".HI").c_str(), type);
		else if (length > 3 && (!strcmp(files[i] + length - 3, ".hi") || !strcmp(files[i] + length - 3, ".HI")))
			hi2ppm(files[i], (string(files[i], length - 3) + ".ppm").c_str(), format);
		else
			cerr << appName << ": unknown file " << files[i] << ", skipping" << endl;
	}

	return 0;
}