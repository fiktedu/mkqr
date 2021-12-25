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
