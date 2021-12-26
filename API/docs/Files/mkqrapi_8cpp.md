---
title: D:/Projects/mkqr/API/mkqr_api/mkqrapi.cpp

---

# D:/Projects/mkqr/API/mkqr_api/mkqrapi.cpp



## Functions

|                | Name           |
| -------------- | -------------- |
| MKQR_API size_t | **[MkqrCreateNew](/Files/mkqrapi_8cpp.md#function-mkqrcreatenew)**()<br>Creates a new unmanaged instance of the [MKQR::Generator](/Classes/class_m_k_q_r_1_1_generator.md). Returns a handle. Save this handle in a variable of the appropriate type and size on your platform. Send this handle to other functions in the API to manipulate the [MKQR::Generator](/Classes/class_m_k_q_r_1_1_generator.md) instance. Remember to call MkqrDelete with this instance when you're ready to free the memory.  |
| MKQR_API void | **[MkqrDelete](/Files/mkqrapi_8cpp.md#function-mkqrdelete)**(size_t instance)<br>Deletes the [MKQR::Generator](/Classes/class_m_k_q_r_1_1_generator.md) handle if it exists. Remember to call this function whenever you don't need the [MKQR::Generator](/Classes/class_m_k_q_r_1_1_generator.md) instance anymore.  |
| MKQR_API void | **[MkqrCreateParameter](/Files/mkqrapi_8cpp.md#function-mkqrcreateparameter)**(size_t instance, const char * name, const char * value)<br>Tries to create a new parameter on the supplied instance of [MKQR::Generator](/Classes/class_m_k_q_r_1_1_generator.md). If it fails to validate the value of the parameter it will log a message that you can retrieve by calling MkqrGetLastErrorMessage.  |
| MKQR_API void | **[MkqrGenerate](/Files/mkqrapi_8cpp.md#function-mkqrgenerate)**(size_t instance, uint8_t isMonochrome, size_t superSampling)<br>Tries to generate a new [MKQR](/Namespaces/namespace_m_k_q_r.md) code from the supplied parameters. Call only after you have created all parameters using MkqrCreateParameter. The generator may fail because of an invalid parameter, in this case a [MKQR](/Namespaces/namespace_m_k_q_r.md) code will not be generated and a message with a code will be logged. You can retrieve the message and the code using the MkqrGetLastErrorMessage and MkqrGetLastErrorCode functions.  |
| MKQR_API size_t | **[MkqrGetImageSize](/Files/mkqrapi_8cpp.md#function-mkqrgetimagesize)**(size_t instance)<br>Gets the total number of pixels (height*width) in the QR image.  |
| MKQR_API uint32_t | **[MkqrGetImageDataAtIndex](/Files/mkqrapi_8cpp.md#function-mkqrgetimagedataatindex)**(size_t instance, size_t index)<br>Gets the pixel color data at a given pixel index.  |
| MKQR_API const char * | **[MkqrGetLastErrorMessage](/Files/mkqrapi_8cpp.md#function-mkqrgetlasterrormessage)**(size_t instance)<br>Gets the last error message.  |
| MKQR_API uint8_t | **[MkqrGetLastErrorCode](/Files/mkqrapi_8cpp.md#function-mkqrgetlasterrorcode)**(size_t instance)<br>Gets the last error code.  |


## Functions Documentation

### function MkqrCreateNew

```cpp
MKQR_API size_t MkqrCreateNew()
```

Creates a new unmanaged instance of the [MKQR::Generator](/Classes/class_m_k_q_r_1_1_generator.md). Returns a handle. Save this handle in a variable of the appropriate type and size on your platform. Send this handle to other functions in the API to manipulate the [MKQR::Generator](/Classes/class_m_k_q_r_1_1_generator.md) instance. Remember to call MkqrDelete with this instance when you're ready to free the memory. 

**Return**: Returns a 32-bit or 64-bit integer which represents the pointer (address) to the newly created instance of the [MKQR::Generator](/Classes/class_m_k_q_r_1_1_generator.md) class. 

### function MkqrDelete

```cpp
MKQR_API void MkqrDelete(
    size_t instance
)
```

Deletes the [MKQR::Generator](/Classes/class_m_k_q_r_1_1_generator.md) handle if it exists. Remember to call this function whenever you don't need the [MKQR::Generator](/Classes/class_m_k_q_r_1_1_generator.md) instance anymore. 

**Parameters**: 

  * **instance** The instance created using MkqrCreateNew()


**Return**: void 

### function MkqrCreateParameter

```cpp
MKQR_API void MkqrCreateParameter(
    size_t instance,
    const char * name,
    const char * value
)
```

Tries to create a new parameter on the supplied instance of [MKQR::Generator](/Classes/class_m_k_q_r_1_1_generator.md). If it fails to validate the value of the parameter it will log a message that you can retrieve by calling MkqrGetLastErrorMessage. 

**Parameters**: 

  * **instance** The instance created using MkqrCreateNew() 
  * **name** The name of the new parameter (might need marshalling) 
  * **value** The value of the new parameter (might need marshalling)


**Return**: void 

### function MkqrGenerate

```cpp
MKQR_API void MkqrGenerate(
    size_t instance,
    uint8_t isMonochrome,
    size_t superSampling
)
```

Tries to generate a new [MKQR](/Namespaces/namespace_m_k_q_r.md) code from the supplied parameters. Call only after you have created all parameters using MkqrCreateParameter. The generator may fail because of an invalid parameter, in this case a [MKQR](/Namespaces/namespace_m_k_q_r.md) code will not be generated and a message with a code will be logged. You can retrieve the message and the code using the MkqrGetLastErrorMessage and MkqrGetLastErrorCode functions. 

**Parameters**: 

  * **instance** The instance created using MkqrCreateNew() 
  * **isMonochrome** If 0 the generated [MKQR](/Namespaces/namespace_m_k_q_r.md) code will be colored, if 1 the generated [MKQR](/Namespaces/namespace_m_k_q_r.md) code will be black and white 
  * **superSampling** If set to 1 then for every QR segment (square) that is generated, the resulting image will have exactly 1 pixel. If set to 2 the resulting image will have 4 pixels for each segment, and so on


**Return**: void 

### function MkqrGetImageSize

```cpp
MKQR_API size_t MkqrGetImageSize(
    size_t instance
)
```

Gets the total number of pixels (height*width) in the QR image. 

**Parameters**: 

  * **instance** The instance created using MkqrCreateNew()


**Return**: Total number of pixels 

### function MkqrGetImageDataAtIndex

```cpp
MKQR_API uint32_t MkqrGetImageDataAtIndex(
    size_t instance,
    size_t index
)
```

Gets the pixel color data at a given pixel index. 

**Parameters**: 

  * **instance** The instance created using MkqrCreateNew() 
  * **index** The absolute index at which to get the color data. To calculate the index from column/row value use: row * rowSize + column


**Return**: 32-bit integer which contains the color data for the given pixel, alpha is always set to 0xFF 

### function MkqrGetLastErrorMessage

```cpp
MKQR_API const char * MkqrGetLastErrorMessage(
    size_t instance
)
```

Gets the last error message. 

**Parameters**: 

  * **instance** The instance created using MkqrCreateNew()


**Return**: The error message as a string (**requres marshalling!**) 

### function MkqrGetLastErrorCode

```cpp
MKQR_API uint8_t MkqrGetLastErrorCode(
    size_t instance
)
```

Gets the last error code. 

**Parameters**: 

  * **instance** The instance created using MkqrCreateNew()


**Return**: 8-bit error code which categorizes the error 



## Source code

```cpp
#include "mkqrapi.h"
#include "mkqr_generator.h"

#include <stdexcept>

MKQR_API size_t MkqrCreateNew()
{
    const MKQR::Generator* const ptr = new MKQR::Generator();
    if (!ptr)
    {
        return 0;
    }

    return (size_t)ptr;
}

MKQR_API void MkqrDelete(size_t instance)
{
    const MKQR::Generator* const instancePtr = reinterpret_cast<MKQR::Generator*>(instance);
    if (instancePtr)
    {
        delete instancePtr;
    }
    else
    {
        return;
    }
}

MKQR_API void MkqrCreateParameter(size_t instance, const char* name, const char* value)
{
    MKQR::Generator* const instancePtr = reinterpret_cast<MKQR::Generator*>(instance);

    if (!instancePtr)
    {
        return;
    }

    instancePtr->CreateParameter(name, value);
}

MKQR_API void MkqrGenerate(size_t instance, uint8_t isMonochrome, size_t superSampling)
{
    MKQR::Generator* const instancePtr = reinterpret_cast<MKQR::Generator*>(instance);

    if (!instancePtr)
    {
        return;
    }

    instancePtr->Generate(isMonochrome, superSampling);
}

MKQR_API size_t MkqrGetImageSize(size_t instance)
{
    const MKQR::Generator* const instancePtr = reinterpret_cast<MKQR::Generator*>(instance);

    if (!instancePtr)
    {
        return 0;
    }

    return instancePtr->GetQrSize();
}



MKQR_API uint32_t MkqrGetImageDataAtIndex(size_t instance, size_t index)
{
    const MKQR::Generator* const instancePtr = reinterpret_cast<MKQR::Generator*>(instance);

    if (!instancePtr)
    {
        return 0;
    }

    return instancePtr->GetQrImageDataAtIndex(index);
}

MKQR_API const char* MkqrGetLastErrorMessage(size_t instance)
{
    MKQR::Generator* const instancePtr = reinterpret_cast<MKQR::Generator*>(instance);

    if (!instancePtr)
    {
        return "Instance is invalid";
    }

    return instancePtr->GetLastErrorMessage();
}

MKQR_API uint8_t MkqrGetLastErrorCode(size_t instance)
{
    MKQR::Generator* const instancePtr = reinterpret_cast<MKQR::Generator*>(instance);

    if (!instancePtr)
    {
        return 0;
    }

    return instancePtr->GetLastErrorCode();
}
```


-------------------------------

Updated on 2021-12-26 at 18:31:40 +0100
