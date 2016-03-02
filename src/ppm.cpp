#include <fstream>
#include <limits>
#include "hicolor.hpp"

using namespace std;

void skipComments(ifstream & input) {
	char c;

	for (input >> c; c == '#'; c = input.get())
		input.ignore(numeric_limits<streamsize>::max(), '\n');

	input.unget();
}

void loadPPM(const char * input, uint16_t ** data, uint16_t * width, uint16_t * height, uint16_t * colors) {
	ifstream image(input);
	string format;

	// hlavička
	skipComments(image);
	image >> format;
	skipComments(image);
	image >> *width;
	skipComments(image);
	image >> *height;
	skipComments(image);
	image >> *colors;
	image.get();

	*data = NULL;

	if (!image || !(*width) || !(*height) || !(*colors) || (format != ASCII && format != BIN))
		return;

	size_t n = *width * *height;
	*data = new uint16_t[3 * n];

	// načtení dat podle formátu
	if (format == ASCII) {
		for (size_t i = 0; i < n; i++) {
			image >> (*data)[3 * i];
			image >> (*data)[3 * i + 1];
			image >> (*data)[3 * i + 2];
		}
	}
	else {
		if (*colors < 256) {
			uint8_t * tmp = new uint8_t[3 * n];
			image.read(reinterpret_cast<char *>(tmp), 3 * n * sizeof(uint8_t));

			for (size_t i = 0; i < 3 * n; i++)
				(*data)[i] = (uint16_t)tmp[i];

			delete[] tmp;
		}
		else {
			image.read(reinterpret_cast<char *>(*data), 3 * n * sizeof(uint16_t));

			// uint16_t je potřeba převést z Big-endian na Little-endian
			for (size_t i = 0; i < 3 * n; i++)
				swap(*reinterpret_cast<uint8_t *>(*data + i), *(reinterpret_cast<uint8_t *>(*data + i) + 1));
		}
	}

	image.close();
}

void savePPM(const char * file, const uint8_t * data, uint16_t width, uint16_t height, const char * format) {
	ofstream result(file);

	result << format << endl;
	result << width << ' ' << height << endl;
	result << maxColor << endl;

	if (format == BIN)
		result.write(reinterpret_cast<const char *>(data), 3 * width * height * sizeof(uint8_t));
	else {
		for (uint16_t y = 0; y < height; y++) {
			for (uint16_t x = 0; x < width; x++)
				result << +data[3 * y * width + 3 * x] << ' '
					<< +data[3 * y * width + 3 * x + 1] << ' '
					<< +data[3 * y * width + 3 * x + 2] << ' ';

			result << endl;
		}
	}
	
	result.close();
}