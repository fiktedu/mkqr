---
title: D:/Projects/mkqr/API/mkqr_api/mkqr_generator.cpp

---

# D:/Projects/mkqr/API/mkqr_api/mkqr_generator.cpp






## Source code

```cpp
#include "mkqr_generator.h"
#include "mkqr_validator.h"
#include "qrcodegen.h"
#include "logo.h"

#include <regex>

std::string MKQR::Generator::GetParameterValue(const std::string& name) const noexcept
{
    std::string retVal = "";
    const auto& paramIt = mParameters.find(name);
    if (paramIt != mParameters.end())
    {
        retVal = paramIt->second;
    }

    return retVal;
}

std::string MKQR::Generator::GenerateStringFromParameters() const noexcept
{
    std::string retVal = "mkqr://pay?";
    for (const auto& param : mParameters)
    {
        retVal += param.first + "=" + param.second + "&";
    }

    retVal.pop_back();
    return retVal;
}

uint32_t MKQR::Generator::LerpColor(uint32_t color1, uint32_t color2, float alpha) const noexcept
{
    uint8_t r1 = (uint8_t)((color1 & 0xFF0000UL) >> 16); // truncating components
    uint8_t g1 = ((color1 & 0x00FF00UL) >> 8);
    uint8_t b1 = color1 & 0x0000FFUL;

    uint8_t r2 = (uint8_t)((color2 & 0xFF0000UL) >> 16); // truncating components
    uint8_t g2 = (color2 & 0x00FF00UL) >> 8;
    uint8_t b2 = color2 & 0x0000FFUL;

    uint8_t lr = Lerp(alpha, r1, r2);
    uint8_t lg = Lerp(alpha, g1, g2);
    uint8_t lb = Lerp(alpha, b1, b2);

    return lr << 16 | lg << 8 | lb;
}

MKQR::Generator::Generator()
    : mValidator(new Validator(*this))
{
}

MKQR::Generator::~Generator()
{
    delete mTempErrorMessage;
    delete mValidator;
}

void MKQR::Generator::CreateParameter(const std::string& name, const std::string& value) noexcept
{
    if (mFatalError)
        return;

    if (name.empty())
        MKQR_ERR(MKQR_ERR_FATAL, "Parameter name is empty.");

    if (value.empty())
        MKQR_ERR(MKQR_ERR_FATAL, "Parameter value is empty.");

    Validator::SResult result = mValidator->ValidateParameter(name, value);

    if (result.GetLevel() == Validator::SResult::ELevel::Warning)
    {
        MKQR_ERR(MKQR_ERR_NONFATAL, result.GetMessage());
    }
    else if (result.GetLevel() == Validator::SResult::ELevel::Error)
    {
        MKQR_ERR(MKQR_ERR_FATAL, result.GetMessage());
        mFatalError = true;
    }
    else if (result.GetLevel() == Validator::SResult::ELevel::Ok)
    {
        mParameters.insert_or_assign(name, value);
    }
}

uint8_t MKQR::Generator::ValidateParameter(const std::string& name, const std::string& value) const noexcept
{
    const Validator::SResult result = mValidator->ValidateParameter(name, value);
    if (result.GetLevel() == Validator::SResult::ELevel::Ok)
        return 0;
    else if (result.GetLevel() == Validator::SResult::ELevel::Warning)
        return 1;
    else if (result.GetLevel() == Validator::SResult::ELevel::Error)
        return 2;
    
    return 2;
}

void MKQR::Generator::Generate(uint8_t isMonochrome, size_t superSampling) noexcept
{
    if (mFatalError)
        return;

    const std::vector<std::string>& mandatoryParams = mValidator->GetMandatoryParameters();
    for (const std::string& mandParam : mandatoryParams)
    {
        if (mParameters.find(mandParam) == mParameters.end())
        {
            MKQR_ERR(MKQR_ERR_FATAL, "Not all mandatory parameters are present");
            return;
        }
    }

    mSuperSampling = superSampling;
    mIsMonochrome = isMonochrome;

    mQrMatrix.clear();

    const std::string string = GenerateStringFromParameters();

    const qrcodegen::QrCode::Ecc errCorLvl = qrcodegen::QrCode::Ecc::LOW;
    const qrcodegen::QrCode qrCode = qrcodegen::QrCode::encodeText(string.c_str(), errCorLvl);

    mQrMatrixSize = qrCode.getSize();

    mQrMatrix.resize(mQrMatrixSize * mQrMatrixSize);

    for (size_t i = 0; i < mQrMatrixSize * mQrMatrixSize; i++)
    {
        mQrMatrix[i] = qrCode.getModule((int)(i % mQrMatrixSize), (int)(i / mQrMatrixSize));
    }

    const size_t numPixels = GetQrSize() * GetQrSize();
    mQrImageData.resize(numPixels);

    for (size_t i = 0; i < numPixels; i++)
    {
        const size_t finalSize = GetQrSize();
        const size_t logoSize = MKQR_LOGO_SIZE * mSuperSampling;

        const size_t logoPos = (finalSize / 2) - (logoSize / 2);

        const size_t sampledIndex = i / mSuperSampling;

        const size_t unsampledColumn = i % finalSize;
        const size_t unsampledRow = i / finalSize;

        const size_t column = sampledIndex % mQrMatrixSize;
        const size_t row = sampledIndex / mQrMatrixSize;

        const size_t sampleRow = unsampledRow % mSuperSampling == 0 ? row / mSuperSampling : unsampledRow / mSuperSampling;

        uint32_t finalValue = 0;

        if (unsampledColumn >= logoPos && unsampledColumn <= logoPos + logoSize
            && unsampledRow >= logoPos && unsampledRow <= logoPos + logoSize)
        {
            const size_t finalColumn = MapRange<size_t>((unsampledColumn - logoPos), 0, logoSize, 0, gLogoSize);
            const size_t finalRow = MapRange<size_t>((unsampledRow - logoPos), 0, logoSize, 0, gLogoSize) * 2;

            const uint32_t underlyingColor = mQrMatrix[sampleRow * mQrMatrixSize + column] ? MKQR_BLACK : MKQR_WHITE;
            const uint32_t originalColor = gLogo[finalRow * gLogoSize + finalColumn];

            const uint32_t color1 = LerpColor((mIsMonochrome == 0) ? MKQR_YELLOW : MKQR_WHITE, underlyingColor, 0.2f);
            const uint32_t color2 = LerpColor((mIsMonochrome == 0) ? MKQR_RED : MKQR_BLACK, underlyingColor, 0.2f);

            finalValue = originalColor < 0x88 ? color1 : color2;
        }
        else
        {
            finalValue = mQrMatrix[sampleRow * mQrMatrixSize + column] ? MKQR_BLACK : MKQR_WHITE;
            if (!mIsMonochrome)
            {
                float alpha = (float)(unsampledRow / (float)finalSize);

                const uint32_t lerpedColor = LerpColor(MKQR_RED, MKQR_BLACK, alpha);
                finalValue = (finalValue > 0x800000UL) ? MKQR_WHITE : lerpedColor;
            }
        }

        mQrImageData[i] = finalValue;
    }
}

const char* MKQR::Generator::GetLastErrorMessage() noexcept
{
    delete mTempErrorMessage;
    mTempErrorMessage = new char[mLastErrorMessage.length()+1];
    strcpy_s(mTempErrorMessage, mLastErrorMessage.length()+1, mLastErrorMessage.c_str());
    mLastErrorMessage.clear();
    return mTempErrorMessage;
}

uint8_t MKQR::Generator::GetLastErrorCode() noexcept
{
    uint8_t retVal = mLastErrorCode;
    mLastErrorCode = MKQR_ERR_NO;
    return retVal;  
}
```


-------------------------------

Updated on 2022-01-15 at 21:45:57 +0100
