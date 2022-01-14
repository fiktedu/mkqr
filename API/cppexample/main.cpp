#include <fstream>
#include <vector>
#include <string>
#include <thread>
#include <cmath>
#include <random>
#include "mkqrapi.h"

void WriteBMP(const std::string path, const size_t width, const size_t height, const int* const data)
{
	const size_t pad = (4 - (3 * width) % 4) % 4, filesize = 54 + (3 * width + pad) * height;
	char header[54] = { 'B','M', 0,0,0,0, 0,0,0,0, 54,0,0,0, 40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0,24,0 };

	for (size_t i = 0; i < 4; i++)
	{
		header[2 + i] = (char)((filesize >> (8 * i)) & 255);
		header[18 + i] = (char)((width >> (8 * i)) & 255);
		header[22 + i] = (char)((height >> (8 * i)) & 255);
	}

	char* img = new char[filesize];
	for (size_t i = 0; i < 54; i++)
		img[i] = header[i];

	for (size_t y = 0; y < height; y++)
	{
		for (size_t x = 0; x < width; x++)
		{
			const int color = data[x + (height - 1 - y) * width];
			const size_t i = 54 + 3 * x + y * (3 * width + pad);
			img[i] = (char)(color & 255);
			img[i + 1] = (char)((color >> 8) & 255);
			img[i + 2] = (char)((color >> 16) & 255);
		}

		for (size_t p = 0; p < pad; p++)
			img[54 + (3 * width + p) + y * (3 * width + pad)] = 0;
	}

	std::ofstream file(path, std::ios::out | std::ios::binary);
	file.write(img, filesize);
	file.close();
	delete[] img;
}

std::string GenerateRandomString(size_t numLetters)
{
	std::string str("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");

	std::random_device rd;
	std::mt19937 generator(rd());

	std::shuffle(str.begin(), str.end(), generator);

	return str.substr(0, numLetters);
}

void GenerateQR(size_t num)
{
	for (size_t i = 0; i < num; i++)
	{
		const size_t mkqr = MkqrCreateNew();

		MkqrCreateParameter(mkqr, "t", "MKD");
		MkqrCreateParameter(mkqr, "v", "1000");
		MkqrCreateParameter(mkqr, "c", "1");
		MkqrCreateParameter(mkqr, "iban", "DE89370400440532013000"); 
		MkqrCreateParameter(mkqr, "aiban", "DE89370400440532013000|DE89370400440532013000");
		MkqrCreateParameter(mkqr, "cat", "S");
		MkqrCreateParameter(mkqr, "cn", GenerateRandomString(20).c_str());
		MkqrCreateParameter(mkqr, "cadd1", "Лондонска бр. 8");
		MkqrCreateParameter(mkqr, "cz", "1000");
		MkqrCreateParameter(mkqr, "cg", "Скопје");
		MkqrCreateParameter(mkqr, "cc", "MK");
		MkqrCreateParameter(mkqr, "cur", "MKD");
		MkqrCreateParameter(mkqr, "ref", "NON");

		const size_t supersampling = 8;
		MkqrGenerate(mkqr, 0, supersampling);

		const size_t imageSize = MkqrGetImageSize(mkqr);

		std::vector<int32_t> imageData;
		for (size_t i = 0; i < imageSize * imageSize; i++)
		{
			imageData.push_back(MkqrGetImageDataAtIndex(mkqr, i));
		}

		const std::string name = GenerateRandomString(16);

		WriteBMP(("generated\\generatedQr" + name + ".bmp").c_str(), imageSize, imageSize, imageData.data());

		MkqrDelete(mkqr);
	}
}

int main()
{
	GenerateQR(1);

	std::vector<std::thread> threads;
	const size_t maxThreads = std::max((unsigned int)0, std::thread::hardware_concurrency() - 2);
	const size_t qrsToGenerate = 10000;
	const size_t itemsPerThread = qrsToGenerate / maxThreads;

	for (size_t i = 0; i < maxThreads; i++)
	{
		threads.push_back(std::thread(GenerateQR, itemsPerThread));
	}

	for (std::thread& thread : threads)
	{
		thread.join();
	}

	return 0;
}