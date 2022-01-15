#include <fstream>
#include <vector>
#include <string>
#include <thread>
#include <cmath>
#include <random>
#include <filesystem>

#define CATCH_CONFIG_MAIN
#include "Catch2.hpp"

#include "mkqrapi.h"

#define GENERATED_PATH "generated"

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

		WriteBMP((GENERATED_PATH "\\generatedQr" + name + ".bmp").c_str(), imageSize, imageSize, imageData.data());

		MkqrDelete(mkqr);
	}
}

TEST_CASE("Create instance", "[Core]")
{
	const size_t mkqr = MkqrCreateNew();
	REQUIRE(MkqrGetLastErrorCode(mkqr) == 0);
	MkqrDelete(mkqr);
}

TEST_CASE("Parameter t", "[ParameterValidation]")
{
	const size_t mkqr = MkqrCreateNew();
	REQUIRE(MkqrValidateParameter(mkqr, "t", "MKD") == 0);
	REQUIRE(MkqrValidateParameter(mkqr, "t", "ALB") == 2);

	MkqrDelete(mkqr);
}

TEST_CASE("Parameter v", "[ParameterValidation]")
{
	const size_t mkqr = MkqrCreateNew();
	REQUIRE(MkqrValidateParameter(mkqr, "v", "1000") == 0);
	REQUIRE(MkqrValidateParameter(mkqr, "v", "SM23") == 2);

	MkqrDelete(mkqr);
}

TEST_CASE("Parameter c", "[ParameterValidation]")
{
	const size_t mkqr = MkqrCreateNew();
	REQUIRE(MkqrValidateParameter(mkqr, "c", "1") == 0);
	REQUIRE(MkqrValidateParameter(mkqr, "c", "3") == 2);

	MkqrDelete(mkqr);
}

TEST_CASE("Parameter iban", "[ParameterValidation]")
{
	const size_t mkqr = MkqrCreateNew();
	REQUIRE(MkqrValidateParameter(mkqr, "iban", "IT07W0326843450000354472850") == 0);
	REQUIRE(MkqrValidateParameter(mkqr, "iban", "IT07W0326843450000354472850|DE89370400440532013000") == 2);
	REQUIRE(MkqrValidateParameter(mkqr, "iban", "MKW032684340000354472850") == 2);
	REQUIRE(MkqrValidateParameter(mkqr, "iban", "07W0326843450000354472850") == 2);
	REQUIRE(MkqrValidateParameter(mkqr, "iban", "0326843450000354472850") == 2);

	MkqrDelete(mkqr);
}

TEST_CASE("Parameter aiban", "[ParameterValidation]")
{
	const size_t mkqr = MkqrCreateNew();
	REQUIRE(MkqrValidateParameter(mkqr, "aiban", "IT07W0326843450000354472850") == 0);
	REQUIRE(MkqrValidateParameter(mkqr, "aiban", "IT07W0326843450000354472850|DE89370400440532013000") == 0);
	REQUIRE(MkqrValidateParameter(mkqr, "aiban", "MKW032684340000354472850") == 1);
	REQUIRE(MkqrValidateParameter(mkqr, "aiban", "07W0326843450000354472850") == 1);
	REQUIRE(MkqrValidateParameter(mkqr, "aiban", "0326843450000354472850") == 1);

	MkqrDelete(mkqr);
}

TEST_CASE("Parameter cat", "[ParameterValidation]")
{
	const size_t mkqr = MkqrCreateNew();
	REQUIRE(MkqrValidateParameter(mkqr, "cat", "S") == 0);
	REQUIRE(MkqrValidateParameter(mkqr, "cat", "K") == 0);
	REQUIRE(MkqrValidateParameter(mkqr, "cat", "M") == 2);
	REQUIRE(MkqrValidateParameter(mkqr, "cat", "SK") == 2);

	MkqrDelete(mkqr);
}

TEST_CASE("Parameter cn", "[ParameterValidation]")
{
	const size_t mkqr = MkqrCreateNew();
	REQUIRE(MkqrValidateParameter(mkqr, "cn", "S") == 0);
	REQUIRE(MkqrValidateParameter(mkqr, "cn", "FASDF9FA8HSD9FH249R8F4UIFDSF98EHIOU34EFRD98FHIDROFSDF98AHSEIODFASDFA9S") == 0);
	REQUIRE(MkqrValidateParameter(mkqr, "cn", "FASDF9FA8HSD9FH249R8F4UIFDSF98EHIOU34EFRD98FHIDROFSDF98AHSEIODFASDFA9S1") == 2);

	MkqrDelete(mkqr);
}

TEST_CASE("Parameter cadd1", "[ParameterValidation]")
{
	SECTION("With S")
	{
		const size_t mkqr = MkqrCreateNew();
		REQUIRE(MkqrValidateParameter(mkqr, "cadd1", "S") == 1);
		MkqrCreateParameter(mkqr, "cat", "S");
		REQUIRE(MkqrValidateParameter(mkqr, "cadd1", "FDSFAJSD90FJJ94E") == 0);
		REQUIRE(MkqrValidateParameter(mkqr, "cadd1", "FDSFAJSD90FJJ94E3") == 1);

		MkqrDelete(mkqr);
	}
	SECTION("With K")
	{
		const size_t mkqr = MkqrCreateNew();
		REQUIRE(MkqrValidateParameter(mkqr, "cadd1", "K") == 1);
		MkqrCreateParameter(mkqr, "cat", "K");
		REQUIRE(MkqrValidateParameter(mkqr, "cadd1", "F90SDJF0A9SDJF0A9SDJFIASDFASD8HFIASDFASDFASD98FNIOASDFASDFASD8F9ASDFAS") == 0);
		REQUIRE(MkqrValidateParameter(mkqr, "cadd1", "F90SDJF0A9SDJF0A9SDJFIASDFASD8HFIASDFASDFASD98FNIOASDFASDFASD8F9ASDFAS1") == 2);

		MkqrDelete(mkqr);
	}
}

TEST_CASE("Parameter cadd2", "[ParameterValidation]")
{
	SECTION("With S")
	{
		const size_t mkqr = MkqrCreateNew();
		REQUIRE(MkqrValidateParameter(mkqr, "cadd2", "S") == 1);
		MkqrCreateParameter(mkqr, "cat", "S");
		REQUIRE(MkqrValidateParameter(mkqr, "cadd2", "FDSFAJSD90FJJ94E") == 0);
		REQUIRE(MkqrValidateParameter(mkqr, "cadd2", "FDSFAJSD90FJJ94E3") == 1);

		MkqrDelete(mkqr);
	}
	SECTION("With K")
	{
		const size_t mkqr = MkqrCreateNew();
		REQUIRE(MkqrValidateParameter(mkqr, "cadd2", "K") == 1);
		MkqrCreateParameter(mkqr, "cat", "K");
		REQUIRE(MkqrValidateParameter(mkqr, "cadd2", "F90SDJF0A9SDJF0A9SDJFIASDFASD8HFIASDFASDFASD98FNIOASDFASDFASD8F9ASDFAS") == 0);
		REQUIRE(MkqrValidateParameter(mkqr, "cadd2", "F90SDJF0A9SDJF0A9SDJFIASDFASD8HFIASDFASDFASD98FNIOASDFASDFASD8F9ASDFAS1") == 1);

		MkqrDelete(mkqr);
	}
}

TEST_CASE("Parameter cz", "[ParameterValidation]")
{
	SECTION("With S")
	{
		const size_t mkqr = MkqrCreateNew();
		REQUIRE(MkqrValidateParameter(mkqr, "cz", "1") == 1);
		MkqrCreateParameter(mkqr, "cat", "S");
		REQUIRE(MkqrValidateParameter(mkqr, "cz", "1234567") == 0);
		REQUIRE(MkqrValidateParameter(mkqr, "cz", "12345678") == 2);
		REQUIRE(MkqrValidateParameter(mkqr, "cz", "174.2244") == 2);
		REQUIRE(MkqrValidateParameter(mkqr, "cz", "-14.44") == 2);

		MkqrDelete(mkqr);
	}
	SECTION("With K")
	{
		const size_t mkqr = MkqrCreateNew();
		REQUIRE(MkqrValidateParameter(mkqr, "cz", "1") == 1);
		MkqrCreateParameter(mkqr, "cat", "K");
		REQUIRE(MkqrValidateParameter(mkqr, "cz", "12") == 1);

		MkqrDelete(mkqr);
	}
}

TEST_CASE("Parameter cg", "[ParameterValidation]")
{
	SECTION("With S")
	{
		const size_t mkqr = MkqrCreateNew();
		REQUIRE(MkqrValidateParameter(mkqr, "cg", "1") == 1);
		MkqrCreateParameter(mkqr, "cat", "S");
		REQUIRE(MkqrValidateParameter(mkqr, "cg", "89FSD0F9JASD09FJAISDFASDF0A9JSIODFK") == 0);
		REQUIRE(MkqrValidateParameter(mkqr, "cg", "89FSD0F9JASD09FJAISDFASDF0A9JSIODFK1") == 2);

		MkqrDelete(mkqr);
	}
	SECTION("With K")
	{
		const size_t mkqr = MkqrCreateNew();
		REQUIRE(MkqrValidateParameter(mkqr, "cg", "1") == 1);
		MkqrCreateParameter(mkqr, "cat", "K");
		REQUIRE(MkqrValidateParameter(mkqr, "cg", "12") == 1);

		MkqrDelete(mkqr);
	}
}

TEST_CASE("Parameter cc", "[ParameterValidation]")
{
	const size_t mkqr = MkqrCreateNew();
	REQUIRE(MkqrValidateParameter(mkqr, "cc", "MK") == 0);
	REQUIRE(MkqrValidateParameter(mkqr, "cc", "DD") == 2);
	REQUIRE(MkqrValidateParameter(mkqr, "cc", "SRB") == 2);

	MkqrDelete(mkqr);
}

TEST_CASE("Parameter a", "[ParameterValidation]")
{
	const size_t mkqr = MkqrCreateNew();
	REQUIRE(MkqrValidateParameter(mkqr, "a", "1.03234") == 0);
	REQUIRE(MkqrValidateParameter(mkqr, "a", "42342.98423947234723984723984729347239847239847293472839429347293472934723423492387498472934723984723984729347283942934729347293472342349238749847293472398472398472934728394293472934729347234234923874984729347239847239847293472839429347293472934723423492387498472934723984723984729347283942934729347293472342349238749847293472398472398472934728394293472934729347234234923874984729347239847239847293472839429347293472934723423492387498472934723984723984729347283942934729347293472342349238749847293472398472398472934728394293472934729347234234923874984729347239847239847293472839429347293472934723423492387498472934723984723984729347283942934729347293472342349238749847293472398472398472934728394293472934729347234234923874984729347239847239847293472839429347293472934723423492387498472934723984723984729347283942934729347293472342349238749847293472398472398472934728394293472934729347234234923874984729347239847239847293472839429347293472934723423492387498472934723984723984729347283942934729347293472342349238749847293479045839458394589345894389485498549948584959485934958499999") == 0);
	REQUIRE(MkqrValidateParameter(mkqr, "a", "42342.984239472347239847239847293472398472398472934728394293472934729347234234923874984729347239847239847293472839429347293472934723423492387498472934723984723984729347283942934729347293472342349238749847293472398472398472934728394293472934729347234234923874984729347239847239847293472839429347293472934723423492387498472934723984723984729347283942934729347293472342349238749847293472398472398472934728394293472934729347234234923874984729347239847239847293472839429347293472934723423492387498472934723984723984729347283942934729347293472342349238749847293472398472398472934728394293472934729347234234923874984729347239847239847293472839429347293472934723423492387498472934723984723984729347283942934729347293472342349238749847293472398472398472934728394293472934729347234234923874984729347239847239847293472839429347293472934723423492387498472934723984723984729347283942934729347293472342349238749847293472398472398472934728394293472934729347234234923874984729347239847239847293472839429347293472934723423492387498472934790458394583945893458943894854985499485849594859349584999991") == 1);

	MkqrDelete(mkqr);
}

TEST_CASE("Parameter cur", "[ParameterValidation]")
{
	const size_t mkqr = MkqrCreateNew();
	REQUIRE(MkqrValidateParameter(mkqr, "cur", "MKD") == 0);
	REQUIRE(MkqrValidateParameter(mkqr, "cur", "MK") == 2);
	REQUIRE(MkqrValidateParameter(mkqr, "cur", "SRB") == 2);

	MkqrDelete(mkqr);
}

TEST_CASE("Parameter pat", "[ParameterValidation]")
{
	const size_t mkqr = MkqrCreateNew();
	REQUIRE(MkqrValidateParameter(mkqr, "pat", "S") == 0);
	REQUIRE(MkqrValidateParameter(mkqr, "pat", "K") == 0);
	REQUIRE(MkqrValidateParameter(mkqr, "pat", "M") == 1);
	REQUIRE(MkqrValidateParameter(mkqr, "pat", "SK") == 1);

	MkqrDelete(mkqr);
}

TEST_CASE("Parameter pn", "[ParameterValidation]")
{
	const size_t mkqr = MkqrCreateNew();
	REQUIRE(MkqrValidateParameter(mkqr, "pn", "S") == 0);
	REQUIRE(MkqrValidateParameter(mkqr, "pn", "FASDF9FA8HSD9FH249R8F4UIFDSF98EHIOU34EFRD98FHIDROFSDF98AHSEIODFASDFA9S") == 0);
	REQUIRE(MkqrValidateParameter(mkqr, "pn", "FASDF9FA8HSD9FH249R8F4UIFDSF98EHIOU34EFRD98FHIDROFSDF98AHSEIODFASDFA9S1") == 1);

	MkqrDelete(mkqr);
}

TEST_CASE("Parameter padd1", "[ParameterValidation]")
{
	SECTION("With S")
	{
		const size_t mkqr = MkqrCreateNew();
		REQUIRE(MkqrValidateParameter(mkqr, "padd1", "S") == 1);
		MkqrCreateParameter(mkqr, "pat", "S");
		REQUIRE(MkqrValidateParameter(mkqr, "padd1", "FDSFAJSD90FJJ94E") == 0);
		REQUIRE(MkqrValidateParameter(mkqr, "padd1", "FDSFAJSD90FJJ94E3") == 1);

		MkqrDelete(mkqr);
	}
	SECTION("With K")
	{
		const size_t mkqr = MkqrCreateNew();
		REQUIRE(MkqrValidateParameter(mkqr, "padd1", "K") == 1);
		MkqrCreateParameter(mkqr, "pat", "K");
		REQUIRE(MkqrValidateParameter(mkqr, "padd1", "F90SDJF0A9SDJF0A9SDJFIASDFASD8HFIASDFASDFASD98FNIOASDFASDFASD8F9ASDFAS") == 0);
		REQUIRE(MkqrValidateParameter(mkqr, "padd1", "F90SDJF0A9SDJF0A9SDJFIASDFASD8HFIASDFASDFASD98FNIOASDFASDFASD8F9ASDFAS1") == 2);

		MkqrDelete(mkqr);
	}
}

TEST_CASE("Parameter padd2", "[ParameterValidation]")
{
	SECTION("With S")
	{
		const size_t mkqr = MkqrCreateNew();
		REQUIRE(MkqrValidateParameter(mkqr, "padd2", "S") == 1);
		MkqrCreateParameter(mkqr, "pat", "S");
		REQUIRE(MkqrValidateParameter(mkqr, "padd2", "FDSFAJSD90FJJ94E") == 0);
		REQUIRE(MkqrValidateParameter(mkqr, "padd2", "FDSFAJSD90FJJ94E3") == 1);

		MkqrDelete(mkqr);
	}
	SECTION("With K")
	{
		const size_t mkqr = MkqrCreateNew();
		REQUIRE(MkqrValidateParameter(mkqr, "padd2", "K") == 1);
		MkqrCreateParameter(mkqr, "pat", "K");
		REQUIRE(MkqrValidateParameter(mkqr, "padd2", "F90SDJF0A9SDJF0A9SDJFIASDFASD8HFIASDFASDFASD98FNIOASDFASDFASD8F9ASDFAS") == 0);
		REQUIRE(MkqrValidateParameter(mkqr, "padd2", "F90SDJF0A9SDJF0A9SDJFIASDFASD8HFIASDFASDFASD98FNIOASDFASDFASD8F9ASDFAS1") == 1);

		MkqrDelete(mkqr);
	}
}

TEST_CASE("Parameter pz", "[ParameterValidation]")
{
	SECTION("With S")
	{
		const size_t mkqr = MkqrCreateNew();
		REQUIRE(MkqrValidateParameter(mkqr, "pz", "1") == 1);
		MkqrCreateParameter(mkqr, "pat", "S");
		REQUIRE(MkqrValidateParameter(mkqr, "pz", "1234567") == 0);
		REQUIRE(MkqrValidateParameter(mkqr, "pz", "12345678") == 2);
		REQUIRE(MkqrValidateParameter(mkqr, "pz", "174.2244") == 2);
		REQUIRE(MkqrValidateParameter(mkqr, "pz", "-14.44") == 2);

		MkqrDelete(mkqr);
	}
	SECTION("With K")
	{
		const size_t mkqr = MkqrCreateNew();
		REQUIRE(MkqrValidateParameter(mkqr, "pz", "1") == 1);
		MkqrCreateParameter(mkqr, "pat", "K");
		REQUIRE(MkqrValidateParameter(mkqr, "pz", "12") == 1);

		MkqrDelete(mkqr);
	}
}

TEST_CASE("Parameter pg", "[ParameterValidation]")
{
	SECTION("With S")
	{
		const size_t mkqr = MkqrCreateNew();
		REQUIRE(MkqrValidateParameter(mkqr, "pg", "1") == 1);
		MkqrCreateParameter(mkqr, "pat", "S");
		REQUIRE(MkqrValidateParameter(mkqr, "pg", "89FSD0F9JASD09FJAISDFASDF0A9JSIODFK") == 0);
		REQUIRE(MkqrValidateParameter(mkqr, "pg", "89FSD0F9JASD09FJAISDFASDF0A9JSIODFK1") == 2);

		MkqrDelete(mkqr);
	}
	SECTION("With K")
	{
		const size_t mkqr = MkqrCreateNew();
		REQUIRE(MkqrValidateParameter(mkqr, "pg", "1") == 1);
		MkqrCreateParameter(mkqr, "pat", "K");
		REQUIRE(MkqrValidateParameter(mkqr, "pg", "12") == 1);

		MkqrDelete(mkqr);
	}
}

TEST_CASE("Parameter pc", "[ParameterValidation]")
{
	const size_t mkqr = MkqrCreateNew();
	REQUIRE(MkqrValidateParameter(mkqr, "pc", "MK") == 0);
	REQUIRE(MkqrValidateParameter(mkqr, "pc", "DD") == 1);
	REQUIRE(MkqrValidateParameter(mkqr, "pc", "SRB") == 1);

	MkqrDelete(mkqr);
}

TEST_CASE("Parameter rt", "[ParameterValidation]")
{
	const size_t mkqr = MkqrCreateNew();
	REQUIRE(MkqrValidateParameter(mkqr, "rt", "QRR") == 0);
	REQUIRE(MkqrValidateParameter(mkqr, "rt", "SCOR") == 0);
	REQUIRE(MkqrValidateParameter(mkqr, "rt", "NON") == 0);
	REQUIRE(MkqrValidateParameter(mkqr, "rt", "A") == 2);
	REQUIRE(MkqrValidateParameter(mkqr, "rt", "NONONONON") == 2);

	MkqrDelete(mkqr);
}

TEST_CASE("Parameter ref", "[ParameterValidation]")
{
	const size_t mkqr = MkqrCreateNew();
	REQUIRE(MkqrValidateParameter(mkqr, "ref", "AAABBB") == 0);
	REQUIRE(MkqrValidateParameter(mkqr, "ref", "F89DSHFA9SDFUAISDHFASDF78HS") == 0);
	REQUIRE(MkqrValidateParameter(mkqr, "ref", "F89DSHFA9SDFUAISDHFASDF78HS1") == 1);

	MkqrDelete(mkqr);
}

TEST_CASE("Parameter pcd", "[ParameterValidation]")
{
	const size_t mkqr = MkqrCreateNew();
	REQUIRE(MkqrValidateParameter(mkqr, "pcd", "123") == 0);
	REQUIRE(MkqrValidateParameter(mkqr, "pcd", "4.03004500300") == 2);
	REQUIRE(MkqrValidateParameter(mkqr, "pcd", "4.2") == 2);

	MkqrDelete(mkqr);
}

TEST_CASE("Parameter nac", "[ParameterValidation]")
{
	const size_t mkqr = MkqrCreateNew();
	REQUIRE(MkqrValidateParameter(mkqr, "nac", "1") == 0);
	REQUIRE(MkqrValidateParameter(mkqr, "nac", "4.03004500300") == 1);
	REQUIRE(MkqrValidateParameter(mkqr, "nac", "4.2") == 1);
	REQUIRE(MkqrValidateParameter(mkqr, "nac", "42") == 1);

	MkqrDelete(mkqr);
}

TEST_CASE("Parameter us50", "[ParameterValidation]")
{
	const size_t mkqr = MkqrCreateNew();
	REQUIRE(MkqrValidateParameter(mkqr, "us50", "331124123434242") == 0);
	REQUIRE(MkqrValidateParameter(mkqr, "us50", "33") == 1);
	REQUIRE(MkqrValidateParameter(mkqr, "us50", "4.2") == 1);
	REQUIRE(MkqrValidateParameter(mkqr, "us50", "42") == 1);

	MkqrDelete(mkqr);
}

TEST_CASE("Parameter usek50", "[ParameterValidation]")
{
	const size_t mkqr = MkqrCreateNew();
	REQUIRE(MkqrValidateParameter(mkqr, "usek50", "331124123434242") == 0);
	REQUIRE(MkqrValidateParameter(mkqr, "usek50", "33") == 1);
	REQUIRE(MkqrValidateParameter(mkqr, "usek50", "4.2") == 1);
	REQUIRE(MkqrValidateParameter(mkqr, "usek50", "42") == 1);

	MkqrDelete(mkqr);
}

TEST_CASE("Parameter us30", "[ParameterValidation]")
{
	const size_t mkqr = MkqrCreateNew();
	REQUIRE(MkqrValidateParameter(mkqr, "us30", "331124123434242") == 0);
	REQUIRE(MkqrValidateParameter(mkqr, "us30", "33") == 1);
	REQUIRE(MkqrValidateParameter(mkqr, "us30", "4.2") == 1);
	REQUIRE(MkqrValidateParameter(mkqr, "us30", "42") == 1);

	MkqrDelete(mkqr);
}

TEST_CASE("Parameter usek30", "[ParameterValidation]")
{
	const size_t mkqr = MkqrCreateNew();
	REQUIRE(MkqrValidateParameter(mkqr, "usek30", "331124123434242") == 0);
	REQUIRE(MkqrValidateParameter(mkqr, "usek30", "33") == 1);
	REQUIRE(MkqrValidateParameter(mkqr, "usek30", "4.2") == 1);
	REQUIRE(MkqrValidateParameter(mkqr, "usek30", "42") == 1);

	MkqrDelete(mkqr);
}

TEST_CASE("Parameter i", "[ParameterValidation]")
{
	const size_t mkqr = MkqrCreateNew();
	REQUIRE(MkqrValidateParameter(mkqr, "i", "FUIASDHFAS9D8FAHSDUFASDFA8SDHUFASDFA89SDHFUASDFAS9D8FASNDUFASDIFNASDFHASDFASDF9A7SDHFAIUSDFASD9F8AHSDFJKASN4F083N4F2IUNFKJDSNFA87NE4FKJSDNAF") == 0);
	REQUIRE(MkqrValidateParameter(mkqr, "i", "FUIASDHFAS9D8FAHSDUFASDFA8SDHUFASDFA89SDHFUASDFAS9D8FASNDUFASDIFNASDFHASDFASDF9A7SDHFAIUSDFASD9F8AHSDFJKASN4F083N4F2IUNFKJDSNFA87NE4FKJSDNAF1") == 1);

	MkqrDelete(mkqr);
}

TEST_CASE("Parameter curl", "[ParameterValidation]")
{
	const size_t mkqr = MkqrCreateNew();
	REQUIRE(MkqrValidateParameter(mkqr, "curl", "https://twitter.com/home") == 0);
	REQUIRE(MkqrValidateParameter(mkqr, "curl", "http:www.example.com/main.html") == 1);
	REQUIRE(MkqrValidateParameter(mkqr, "curl", "f.com") == 1);

	MkqrDelete(mkqr);
}

TEST_CASE("Parameter ap", "[ParameterValidation]")
{
	const size_t mkqr = MkqrCreateNew();
	REQUIRE(MkqrValidateParameter(mkqr, "ap", "VEROTOKEN") == 0);
	REQUIRE(MkqrValidateParameter(mkqr, "ap", "F89DHF98ASDHFASDUFHS") == 0);
	REQUIRE(MkqrValidateParameter(mkqr, "ap", "F89DHF98ASDHFASDUFHS1") == 1);

	MkqrDelete(mkqr);
}

TEST_CASE("Parameter av", "[ParameterValidation]")
{
	const size_t mkqr = MkqrCreateNew();
	REQUIRE(MkqrValidateParameter(mkqr, "av", "1.03234") == 0);
	REQUIRE(MkqrValidateParameter(mkqr, "av", "42342.98423947234723984723984729347239847239847293472839429347293472934723423492387498472934723984723984729347283942934729347293472342349238749847293472398472398472934728394293472934729347234234923874984729347239847239847293472839429347293472934723423492387498472934723984723984729347283942934729347293472342349238749847293472398472398472934728394293472934729347234234923874984729347239847239847293472839429347293472934723423492387498472934723984723984729347283942934729347293472342349238749847293472398472398472934728394293472934729347234234923874984729347239847239847293472839429347293472934723423492387498472934723984723984729347283942934729347293472342349238749847293472398472398472934728394293472934729347234234923874984729347239847239847293472839429347293472934723423492387498472934723984723984729347283942934729347293472342349238749847293472398472398472934728394293472934729347234234923874984729347239847239847293472839429347293472934723423492387498472934723984723984729347283942934729347293472342349238749847293479045839458394589345894389485498549948584959485934958499999") == 0);
	REQUIRE(MkqrValidateParameter(mkqr, "av", "42342.984239472347239847239847293472398472398472934728394293472934729347234234923874984729347239847239847293472839429347293472934723423492387498472934723984723984729347283942934729347293472342349238749847293472398472398472934728394293472934729347234234923874984729347239847239847293472839429347293472934723423492387498472934723984723984729347283942934729347293472342349238749847293472398472398472934728394293472934729347234234923874984729347239847239847293472839429347293472934723423492387498472934723984723984729347283942934729347293472342349238749847293472398472398472934728394293472934729347234234923874984729347239847239847293472839429347293472934723423492387498472934723984723984729347283942934729347293472342349238749847293472398472398472934728394293472934729347234234923874984729347239847239847293472839429347293472934723423492387498472934723984723984729347283942934729347293472342349238749847293472398472398472934728394293472934729347234234923874984729347239847239847293472839429347293472934723423492387498472934790458394583945893458943894854985499485849594859349584999991") == 1);

	MkqrDelete(mkqr);
}

TEST_CASE("Parameter ad", "[ParameterValidation]")
{
	const size_t mkqr = MkqrCreateNew();
	REQUIRE(MkqrValidateParameter(mkqr, "ad", "98FAUIN4FASEDJFASE89FHASDUFJANSEDF98ASNEFUIJASEF98ASENFJAKSEFASE98FNASEJKFNAS98FENJKFASEN98FANSEJFKANSEF89ASNEFJKAFASENF8SF88FE8EUF89EUJEJFFUDHF98ASDHFUAISDHFA98SDFHAUSDFA98SDFHAUSDFAS98DFHAUSIDFASD98FHASUEFASUEJFASDFASFE9F8EHFEFEA98DSFJEFE") == 0);
	REQUIRE(MkqrValidateParameter(mkqr, "ad", "98FAUIN4FASEDJFASE89FHASDUFJANSEDF98ASNEFUIJASEF98ASENFJAKSEFASE98FNASEJKFNAS98FENJKFASEN98FANSEJFKANSEF89ASNEFJKAFASENF8SF88FE8EUF89EUJEJFFUDHF98ASDHFUAISDHFA98SDFHAUSDFA98SDFHAUSDFAS98DFHAUSIDFASD98FHASUEFASUEJFASDFASFE9F8EHFEFEA98DSFJEFE1") == 1);

	MkqrDelete(mkqr);
}

TEST_CASE("Parameter ac", "[ParameterValidation]")
{
	const size_t mkqr = MkqrCreateNew();
	REQUIRE(MkqrValidateParameter(mkqr, "ac", "MKD") == 0);
	REQUIRE(MkqrValidateParameter(mkqr, "ac", "MK") == 1);
	REQUIRE(MkqrValidateParameter(mkqr, "ac", "SRB") == 1);

	MkqrDelete(mkqr);
}

TEST_CASE("Multiple threads for creating files.", "[CreatingFiles]")
{
	if (std::filesystem::is_directory(GENERATED_PATH))
		std::filesystem::remove_all(GENERATED_PATH);

	std::filesystem::create_directory(GENERATED_PATH);

	std::vector<std::thread> threads;
	const size_t maxThreads = std::max((unsigned int)0, std::thread::hardware_concurrency() - 2);
	const size_t qrsToGenerate = 50;
	const size_t itemsPerThread = qrsToGenerate / maxThreads;

	for (size_t i = 0; i < maxThreads; i++)
	{
		threads.push_back(std::thread(GenerateQR, itemsPerThread));
	}

	for (std::thread& thread : threads)
	{
		thread.join();
	}

	auto dirIter = std::filesystem::directory_iterator(GENERATED_PATH);

	size_t fileCount = std::count_if(
		begin(dirIter),
		end(dirIter),
		[](auto& entry) { return entry.is_regular_file(); }
	);

	REQUIRE(fileCount == (maxThreads * itemsPerThread));
}

TEST_CASE("Create single QR code file", "[CreatingFiles]")
{
	if (std::filesystem::is_directory(GENERATED_PATH))
		std::filesystem::remove_all(GENERATED_PATH);

	std::filesystem::create_directory(GENERATED_PATH);

	GenerateQR(1);

	auto dirIter = std::filesystem::directory_iterator(GENERATED_PATH);

	size_t fileCount = std::count_if(
		begin(dirIter),
		end(dirIter),
		[](auto& entry) { return entry.is_regular_file(); }
	);

	REQUIRE(fileCount == 1);
}