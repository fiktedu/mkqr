---
title: MKQR::Generator
summary: The MKQR::Generator class contains functionality for generating and validating MKQR codes. MKQR::Generator implements all details of the MKQR standard for generating such codes. 

---

# MKQR::Generator



The [MKQR::Generator]() class contains functionality for generating and validating [MKQR](/Namespaces/namespace_m_k_q_r.md) codes. [MKQR::Generator]() implements all details of the [MKQR](/Namespaces/namespace_m_k_q_r.md) standard for generating such codes. 


`#include <mkqr_generator.h>`

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[Generator](/Classes/class_m_k_q_r_1_1_generator.md#function-generator)**()<br>Default constructor. Used to initialize the validator.  |
| | **[~Generator](/Classes/class_m_k_q_r_1_1_generator.md#function-~generator)**()<br>Default destructor. Is used to delete the temporary error message used when copying if it exists.  |
| void | **[CreateParameter](/Classes/class_m_k_q_r_1_1_generator.md#function-createparameter)**(const std::string & name, const std::string & value)<br>Creates parameter with the given name and value. Additionaly, the validation of the parameter is done inside this function. If a validation fails this function will set the mLastErrorMessage and mLastErrorCode. If it fails any subsequent calls to the Generate function will fail. to the Example:    name=t value=0 This will result with a new parameter t=0 which will be added to the map of parameters.  |
| uint8_t | **[ValidateParameter](/Classes/class_m_k_q_r_1_1_generator.md#function-validateparameter)**(const std::string & name, const std::string & value) const<br>Validates a parameter value.  |
| void | **[Generate](/Classes/class_m_k_q_r_1_1_generator.md#function-generate)**(uint8_t isMonochrome, size_t superSampling)<br>Generates the [MKQR](/Namespaces/namespace_m_k_q_r.md) code based on the previously entered parameters. This function will fail if a previously created parameter is invalid. The result of the function is kept inside the mQrImageData variable, which can then be retrieved by the GetQrImageDataAtIndex function.  |
| size_t | **[GetQrSize](/Classes/class_m_k_q_r_1_1_generator.md#function-getqrsize)**() const<br>Gets the size of the QR image.  |
| uint32_t | **[GetQrImageDataAtIndex](/Classes/class_m_k_q_r_1_1_generator.md#function-getqrimagedataatindex)**(size_t index) const<br>Gets the color of the pixel at the specified index.  |
| const char * | **[GetLastErrorMessage](/Classes/class_m_k_q_r_1_1_generator.md#function-getlasterrormessage)**()<br>Gets the last error message.  |
| uint8_t | **[GetLastErrorCode](/Classes/class_m_k_q_r_1_1_generator.md#function-getlasterrorcode)**()<br>Gets the last error code.  |

## Public Functions Documentation

### function Generator

```cpp
Generator()
```

Default constructor. Used to initialize the validator. 

### function ~Generator

```cpp
~Generator()
```

Default destructor. Is used to delete the temporary error message used when copying if it exists. 

### function CreateParameter

```cpp
void CreateParameter(
    const std::string & name,
    const std::string & value
)
```

Creates parameter with the given name and value. Additionaly, the validation of the parameter is done inside this function. If a validation fails this function will set the mLastErrorMessage and mLastErrorCode. If it fails any subsequent calls to the Generate function will fail. to the Example:    name=t value=0 This will result with a new parameter t=0 which will be added to the map of parameters. 

**Parameters**: 

  * **name** The name of the parameter 
  * **value** The value of the parameter


**Return**: void 

### function ValidateParameter

```cpp
uint8_t ValidateParameter(
    const std::string & name,
    const std::string & value
) const
```

Validates a parameter value. 

**Parameters**: 

  * **name** The name of the parameter 
  * **value** The value of the parameter


**Return**: If valid returns 0, if not valid and not mandatory returns 1, if not valid and mandatory returns 2 

### function Generate

```cpp
void Generate(
    uint8_t isMonochrome,
    size_t superSampling
)
```

Generates the [MKQR](/Namespaces/namespace_m_k_q_r.md) code based on the previously entered parameters. This function will fail if a previously created parameter is invalid. The result of the function is kept inside the mQrImageData variable, which can then be retrieved by the GetQrImageDataAtIndex function. 

**Parameters**: 

  * **isMonochrome** Can be either 0 or 1. Setting this parameter to 0 will create a [MKQR](/Namespaces/namespace_m_k_q_r.md) code in full color. Setting it to 1 means the [MKQR](/Namespaces/namespace_m_k_q_r.md) will be black and white. 
  * **superSampling** Sets the super-sampling value. A value of 1 will generate a bitmap with mQrMatrixSize*mQrMatrixSize pixels. A value of 2 will generate twice*twice that, and so on.


**Return**: void 

### function GetQrSize

```cpp
inline size_t GetQrSize() const
```

Gets the size of the QR image. 

**Return**: mQrMatrixSize * mSuperSampling 

### function GetQrImageDataAtIndex

```cpp
inline uint32_t GetQrImageDataAtIndex(
    size_t index
) const
```

Gets the color of the pixel at the specified index. 

**Parameters**: 

  * **index** The absolute index of the pixel. If you have 2 rows of 32 pixels the fourth pixel in the second row will have an index of 36


**Return**: 32-bit color value of the pixel, alpha is always 0xFF 

### function GetLastErrorMessage

```cpp
const char * GetLastErrorMessage()
```

Gets the last error message. 

**Return**: The error message that occured most recently 

### function GetLastErrorCode

```cpp
uint8_t GetLastErrorCode()
```

Gets the last error code. 

**Return**: The error code that occured most recently 

-------------------------------

Updated on 2021-12-27 at 23:28:27 +0100