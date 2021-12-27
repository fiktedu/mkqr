#pragma once
#include <stdint.h>

#define MKQR_API __declspec(dllexport)

#ifdef __cplusplus
extern "C" {
#endif
	/*!@brief Creates a new unmanaged instance of the MKQR::Generator. Returns a
	 * handle. Save this handle in a variable of the appropriate type and size 
	 * on your platform. Send this handle to other functions in the API to 
	 * manipulate the MKQR::Generator instance. Remember to call MkqrDelete with 
	 * this instance when you're ready to free the memory.
	 * 
	 * @return Returns a 32-bit or 64-bit integer which represents the pointer 
	 * (address) to the newly created instance of the MKQR::Generator class. 
	 */
	extern MKQR_API size_t MkqrCreateNew();

	/*!@brief Deletes the MKQR::Generator handle if it exists. Remember to call
	 * this function whenever you don't need the MKQR::Generator instance
	 * anymore.
	 * 
	 * @param instance The instance created using MkqrCreateNew()
	 * 
	 * @return void
	 */
	extern MKQR_API void MkqrDelete(size_t instance);

	/*!@brief Tries to create a new parameter on the supplied instance of 
	 * MKQR::Generator.  If it fails to validate the value of the parameter it 
	 * will log a message that you can retrieve by calling 
	 * MkqrGetLastErrorMessage.
	 * 
	 * @param instance The instance created using MkqrCreateNew()
	 * @param name The name of the new parameter (might need marshalling)
	 * @param value The value of the new parameter (might need marshalling)
	 * 
	 * @return void
	 */
	extern MKQR_API void MkqrCreateParameter(size_t instance, const char* name, 
											 const char* value);
	
	/*!@brief Tries to generate a new MKQR code from the supplied parameters.
	 * Call only after you have created all parameters using 
	 * MkqrCreateParameter. The generator may fail because of an invalid 
	 * parameter, in this case a MKQR code will not be generated and a message
	 * with a code will be logged. You can retrieve the message and the code
	 * using the MkqrGetLastErrorMessage and MkqrGetLastErrorCode functions.
	 * 
	 * @param instance The instance created using MkqrCreateNew()
	 * @param isMonochrome If 0 the generated MKQR code will be colored, if 1
	 * the generated MKQR code will be black and white
	 * @param superSampling If set to 1 then for every QR segment (square) 
	 * that is generated, the resulting image will have exactly 1 pixel. If 
	 * set to 2 the resulting image will have 4 pixels for each segment, and 
	 * so on 
	 * 
	 * @return void
	 */
	extern MKQR_API void MkqrGenerate(size_t instance, uint8_t isMonochrome, 
									  size_t superSampling);

	/*!@brief Gets the total number of pixels (height*width) in the QR image.
	* 
	* @param instance The instance created using MkqrCreateNew()
	* 
	* @return Total number of pixels
	*/
	extern MKQR_API size_t MkqrGetImageSize(size_t instance);

	/*!@brief Gets the pixel color data at a given pixel index.
	 * 
	 * @param instance The instance created using MkqrCreateNew()
	 * @param index The absolute index at which to get the color data.
	 * To calculate the index from column/row value use:
	 * row * rowSize + column
	 * 
	 * @return 32-bit integer which contains the color data for the 
	 * given pixel, alpha is always set to 0xFF
	 */
	extern MKQR_API uint32_t MkqrGetImageDataAtIndex(size_t instance,
													 size_t index);

	/*!@brief Gets the last error message.
	 *
	 * @param instance The instance created using MkqrCreateNew() 
	 * 
	 * @return The error message as a string (**requres marshalling!**)
	*/
	extern MKQR_API const char* MkqrGetLastErrorMessage(size_t instance);

	/*!@brief Gets the last error code.
	 *
	 * @param instance The instance created using MkqrCreateNew()
	 *
	 * @return 8-bit error code which categorizes the error
	*/
	extern MKQR_API uint8_t MkqrGetLastErrorCode(size_t instance);

	/*!@brief Validate a parameter
	 *
	 * @param instance The instance created using MkqrCreateNew()
	 * @param name The name of the parameter
	 * @param value The value of the parameter
	 *
	 * @return If valid returns 0, if not valid and not mandatory returns 1, if 
	 * not valid and mandatory returns 2
	*/
	extern MKQR_API uint8_t MkqrValidateParameter(size_t instance, 
												  const char* name, 
												  const char* value);
#ifdef __cplusplus
}
#endif