#pragma once
#include <string>
#include <vector>

//===================================================================
// 
//     ERROR CODES:                                                   
//                                                                   
#define MKQR_ERR_NO 0 
#define MKQR_ERR_NULL 1
#define MKQR_ERR_INVALID_ARG 1    
//===================================================================

#define MKQR_YELLOW 0xFFE500UL
#define MKQR_RED 0xCC0001UL
#define MKQR_WHITE 0xFFFFFFUL
#define MKQR_BLACK 0x000000UL
#define MKQR_LOGO_SIZE 13 // in qr squares, looks better if it's odd

#define MKQR_ERR(code, message) { mLastErrorMessage = message; mLastErrorCode = code; }

namespace MKQR
{
	class Validator;

	class Generator
	{
	private:
		const Validator& mValidator;
		std::vector<std::string> mParameters;

		std::string GenerateStringFromParameters() const noexcept;

		template<typename T>
		[[nodiscard]] T MapRange(T value, T low1, T high1, T low2, T high2) const noexcept
		{
			return low2 + (value - low1) * (high2 - low2) / (high1 - low1);
		}

		template<typename T>
		[[nodiscard]] T Lerp(float t, T a, T b) const noexcept
		{
			return (T)(a + t * (b - a));
		}

		uint32_t LerpColor(uint32_t color1, uint32_t color2, float alpha) const noexcept;

		std::string mString;
		std::string mStringSVG;

		size_t mQrMatrixSize = 0;

		std::vector<bool> mQrMatrix;
		std::vector<uint32_t> mQrImageData;

		bool mIsMonochrome = true;
		size_t mSuperSampling = 1;

		std::string mLastErrorMessage;
		char* mTempErrorMessage;
		uint8_t mLastErrorCode = MKQR_ERR_NO;
	public:
		Generator();
		~Generator() { delete mTempErrorMessage; }

		void CreateParameter(const std::string& name, const std::string& value) noexcept;
		void Generate(uint8_t isMonochrome, size_t superSampling) noexcept;

		[[nodiscard]] size_t GetQrSize() const noexcept { return mQrMatrixSize * mSuperSampling; }
		[[nodiscard]] uint32_t GetQrImageDataAtIndex(size_t index) const noexcept { return mQrImageData[index]; }

		[[nodiscard]] const char* GetLastErrorMessage() noexcept;
		[[nodiscard]] uint8_t GetLastErrorCode() noexcept;
	};
}