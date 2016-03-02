#include <fstream>
#include "hicolor.hpp"

using namespace std;

inline uint16_t divideAndCeil(uint16_t a, uint16_t b) {
	return (a + b - 1) / b;
}

void rgb48to15(const uint16_t * in, uint16_t * out, size_t n) {
	for (size_t i = 0; i < n; i++)
		out[i] = (in[3 * i] << 10) | (in[3 * i + 1] << 5) | in[3 * i + 2];
}

void ppm2hi(const char * input, const char * output, Type type) {
	uint16_t * data, width, height, colors;

	loadPPM(input, &data, &width, &height, &colors);

	if (!data)
		return;

	size_t n = width * height;
	uint16_t conv = divideAndCeil(colors + 1, maxColor + 1);

	for (size_t i = 0; i < 3 * n; i++)
		data[i] /= conv;

	ofstream ofs(output, ios::binary);

	ofs.write(reinterpret_cast<const char *>(&width), sizeof(uint16_t));
	ofs.write(reinterpret_cast<const char *>(&height), sizeof(uint16_t));

	// zkusíme uložit HIcolor s paletou barev (nesmí jich být víc jak 255)
	if (type == Palette) {
		uint16_t * tmpPalette = new uint16_t[3 * paletteSize];
		uint8_t * indexes = new uint8_t[n];
		size_t count = 0;
		bool success = true;

		for (size_t i = 0; i < n; i++) {
			bool found = false;

			// koukneme se, jestli už barva není v paletě barev
			for (size_t j = 0; j < count; j++) {
				if (data[3 * i] == tmpPalette[3 * j] && data[3 * i + 1] == tmpPalette[3 * j + 1] && data[3 * i + 2] == tmpPalette[3 * j + 2]) {
					found = true;
					indexes[i] = j;
					break;
				}
			}

			// pokud není barva v paletě barev a nevleze se do ní další, vzdáme to
			if (!found && count == paletteSize) {
				success = false;
				break;
			}

			if (!found) {
				tmpPalette[3 * count] = data[3 * i];
				tmpPalette[3 * count + 1] = data[3 * i + 1];
				tmpPalette[3 * count + 2] = data[3 * i + 2];
				indexes[i] = count;
				count++;
			}
		}

		// uložení s paletou barev
		if (success) {
			uint16_t tmp = (uint16_t)Palette;
			ofs.write(reinterpret_cast<const char *>(&tmp), sizeof(uint16_t));

			uint16_t * palette = new uint16_t[paletteSize]();
			rgb48to15(tmpPalette, palette, count);
			delete[] tmpPalette;

			ofs.write(reinterpret_cast<const char *>(palette), n * sizeof(uint16_t));
			ofs.write(reinterpret_cast<const char *>(indexes), n * sizeof(uint8_t));

			delete[] tmpPalette;
			delete[] indexes;
			delete[] data;

			ofs.close();
			return;
		}

		delete[] tmpPalette;
		delete[] indexes;
	}

	// Uložení s paletou barev se nepovedlo, nebo se to nevyžaduje
	uint16_t * result = new uint16_t[n]();
	rgb48to15(data, result, n);

	uint16_t tmp = (uint16_t)Data;
	ofs.write(reinterpret_cast<const char *>(&tmp), sizeof(uint16_t));
	ofs.write(reinterpret_cast<const char *>(result), n * sizeof(uint16_t));

	ofs.close();
	delete[] data;
	delete[] result;
}