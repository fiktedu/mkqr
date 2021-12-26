---
title: mkqr_api/mkqr_generator.h

---

# mkqr_api/mkqr_generator.h



## Namespaces

| Name           |
| -------------- |
| **[MKQR](/Namespaces/namespace_m_k_q_r.md)** <br>This is the default [MKQR]() namespace.Use it to wrap anything that is related to [MKQR]()'s internal systems.  |

## Classes

|                | Name           |
| -------------- | -------------- |
| class | **[MKQR::Generator](/Classes/class_m_k_q_r_1_1_generator.md)** <br>The [MKQR::Generator]() class contains functionality for generating and validating [MKQR](/Namespaces/namespace_m_k_q_r.md) codes. [MKQR::Generator]() implements all details of the [MKQR](/Namespaces/namespace_m_k_q_r.md) standard for generating such codes.  |

## Defines

|                | Name           |
| -------------- | -------------- |
|  | **[MKQR_ERR_NO](/Files/mkqr__generator_8h.md#define-mkqr-err-no)** <br>Indicates that no error has occured during the last action.Use this to check if something has executed successfuly or not.  |
|  | **[MKQR_ERR_NULL](/Files/mkqr__generator_8h.md#define-mkqr-err-null)** <br>Indicates that the last action has returned null or that the handle that the last function tried to access was null.  |
|  | **[MKQR_ERR_INVALID_ARG](/Files/mkqr__generator_8h.md#define-mkqr-err-invalid-arg)** <br>Indicates that an argument was invalid.Set this when a validation fails.  |
|  | **[MKQR_YELLOW](/Files/mkqr__generator_8h.md#define-mkqr-yellow)** <br>Yellow color, 24-bit expanded to 32-bit.  |
|  | **[MKQR_RED](/Files/mkqr__generator_8h.md#define-mkqr-red)** <br>Red color, 24-bit expanded to 32-bit.  |
|  | **[MKQR_WHITE](/Files/mkqr__generator_8h.md#define-mkqr-white)** <br>White color, 24-bit expanded to 32-bit.  |
|  | **[MKQR_BLACK](/Files/mkqr__generator_8h.md#define-mkqr-black)** <br>Black color, 24-bit expanded to 32-bit.  |
|  | **[MKQR_LOGO_SIZE](/Files/mkqr__generator_8h.md#define-mkqr-logo-size)** <br>The size of the logo in QR segments. Odd values give better aligned results.  |
|  | **[MKQR_ERR](/Files/mkqr__generator_8h.md#define-mkqr-err)**(code, message) <br>Sets the code and the message of the currently occuring error or warning.  |




## Macros Documentation

### define MKQR_ERR_NO

```cpp
#define MKQR_ERR_NO 0
```

Indicates that no error has occured during the last action.Use this to check if something has executed successfuly or not. 

### define MKQR_ERR_NULL

```cpp
#define MKQR_ERR_NULL 1
```

Indicates that the last action has returned null or that the handle that the last function tried to access was null. 

### define MKQR_ERR_INVALID_ARG

```cpp
#define MKQR_ERR_INVALID_ARG 2
```

Indicates that an argument was invalid.Set this when a validation fails. 

### define MKQR_YELLOW

```cpp
#define MKQR_YELLOW 0xFFE500UL
```

Yellow color, 24-bit expanded to 32-bit. 

### define MKQR_RED

```cpp
#define MKQR_RED 0xCC0001UL
```

Red color, 24-bit expanded to 32-bit. 

### define MKQR_WHITE

```cpp
#define MKQR_WHITE 0xFFFFFFUL
```

White color, 24-bit expanded to 32-bit. 

### define MKQR_BLACK

```cpp
#define MKQR_BLACK 0x000000UL
```

Black color, 24-bit expanded to 32-bit. 

### define MKQR_LOGO_SIZE

```cpp
#define MKQR_LOGO_SIZE 13
```

The size of the logo in QR segments. Odd values give better aligned results. 

### define MKQR_ERR

```cpp
#define MKQR_ERR(
    code,
    message
)
{ mLastErrorMessage = message; mLastErrorCode = code; }
```

Sets the code and the message of the currently occuring error or warning. 

## Source code

```cpp
#pragma once
#include <string>
#include <vector>
     
#define MKQR_ERR_NO 0 
#define MKQR_ERR_NULL 1
#define MKQR_ERR_INVALID_ARG 2    
//==========================================================================================

#define MKQR_YELLOW 0xFFE500UL
#define MKQR_RED 0xCC0001UL
#define MKQR_WHITE 0xFFFFFFUL
#define MKQR_BLACK 0x000000UL
//==========================================================================================


#define MKQR_LOGO_SIZE 13 
//==========================================================================================

#define MKQR_ERR(code, message) { mLastErrorMessage = message; mLastErrorCode = code; }
//==========================================================================================

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
        [[nodiscard]] T Lerp(float alpha, T a, T b) const noexcept
        {
            return (T)(a + alpha * (b - a));
        }

        [[nodiscard]] uint32_t LerpColor(uint32_t color1, 
            uint32_t color2, float alpha) const noexcept;

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

        [[nodiscard]] size_t GetQrSize() const noexcept 
        { return mQrMatrixSize * mSuperSampling; }

        [[nodiscard]] uint32_t GetQrImageDataAtIndex(size_t index) const noexcept 
        { return mQrImageData[index]; }

        [[nodiscard]] const char* GetLastErrorMessage() noexcept;
        
        [[nodiscard]] uint8_t GetLastErrorCode() noexcept;
    };
}
```


-------------------------------

Updated on 2021-12-26 at 15:42:24 +0100
