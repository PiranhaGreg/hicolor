#include <fstream>
#include "hicolor.hpp"

using namespace std;

void rgb15to24(const uint16_t * in, uint8_t * out, size_t n) {
	for (size_t i = 0; i < n; i++) {
		out[3 * i] = (in[i] & R_mask) >> 10;
		out[3 * i + 1] = (in[i] & G_mask) >> 5;
		out[3 * i + 2] = in[i] & B_mask;
	}
}

void hi2ppm(const char * input, const char * output, const char * format) {
	ifstream image(input, ios::binary);
	uint16_t width, height, type;

	// hlavička
	image.read(reinterpret_cast<char *>(&width), sizeof(uint16_t));
	image.read(reinterpret_cast<char *>(&height), sizeof(uint16_t));
	image.read(reinterpret_cast<char *>(&type), sizeof(uint16_t));

	size_t n = width * height;
	uint8_t * data = new uint8_t[3 * n];

	// načtení dat podle typu
	if (type == Data) {
		uint16_t * tmp = new uint16_t[n];
		
		image.read(reinterpret_cast<char *>(tmp), n * sizeof(uint16_t));
		rgb15to24(tmp, data, n);
		
		delete[] tmp;
	}
	else if (type == Palette) {
		uint16_t * palette = new uint16_t[paletteSize];
		uint8_t * newPalette = new uint8_t[3 * paletteSize];
		uint8_t * tmp = new uint8_t[n];
		
		image.read(reinterpret_cast<char *>(palette), paletteSize * sizeof(uint16_t));
		image.read(reinterpret_cast<char *>(newPalette), 3 * paletteSize * sizeof(uint8_t));
		rgb15to24(palette, newPalette, paletteSize);

		for (size_t i = 0; i < n; i++) {
			data[3 * i] = newPalette[tmp[i]];
			data[3 * i + 1] = newPalette[tmp[i] + 1];
			data[3 * i + 2] = newPalette[tmp[i] + 2];
		}

		delete[] palette;
		delete[] newPalette;
		delete[] tmp;
	}

	image.close();

	savePPM(output, data, width, height, format);

	delete[] data;
}