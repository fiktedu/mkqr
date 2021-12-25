#pragma once
#include <stdint.h>

#define MKQR_API __declspec(dllexport)

#ifdef __cplusplus
extern "C" {
#endif
	extern MKQR_API size_t MkqrCreateNew();
	extern MKQR_API void MkqrDelete(size_t instance);
	extern MKQR_API void MkqrCreateParameter(size_t instance, const char* name, const char* value);
	extern MKQR_API void MkqrGenerate(size_t instance, uint8_t isMonochrome, size_t superSampling);
	extern MKQR_API size_t MkqrGetImageSize(size_t instance);
	extern MKQR_API uint32_t MkqrGetImageDataAtIndex(size_t instance, size_t index);
	extern MKQR_API const char* MkqrGetLastErrorMessage(size_t instance);
	extern MKQR_API uint8_t MkqrGetLastErrorCode(size_t instance);
#ifdef __cplusplus
}
#endif