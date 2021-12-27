#pragma once
#include <string>
#include <vector>
#include <unordered_map>
     
/*!@brief Indicates that no error has occured during the last action.Use this to check if
 * something has executed successfuly or not */
#define MKQR_ERR_NO 0 
/*!@brief Indicates that the last action has returned null or that  the handle that the last
 * function tried to access was null */
#define MKQR_ERR_NULL 1
/*!@brief Indicates that an argument was invalid. Set this when a validation fails */
#define MKQR_ERR_NONFATAL 2    
/*!@brief Indicates that an argument was invalid, but the argument was mandatory.
 * Set this when a validation of a mandatory arguemnt fails. */
#define MKQR_ERR_FATAL 3    
//==========================================================================================

//!@brief Yellow color, 24-bit expanded to 32-bit
#define MKQR_YELLOW 0xFFE500UL
//!@brief Red color, 24-bit expanded to 32-bit
#define MKQR_RED 0xCC0001UL
//!@brief White color, 24-bit expanded to 32-bit
#define MKQR_WHITE 0xFFFFFFUL
//!@brief Black color, 24-bit expanded to 32-bit
#define MKQR_BLACK 0x000000UL
//==========================================================================================


//!@brief The size of the logo in QR segments. Odd values give better aligned results
#define MKQR_LOGO_SIZE 13 
//==========================================================================================

//!@brief Sets the code and the message of the currently occuring error or warning
#define MKQR_ERR(code, message) { mLastErrorMessage = message; mLastErrorCode = code; }
//==========================================================================================

/*!@brief This is the default MKQR namespace.Use it to wrap anything that is related to
 * MKQR's internal systems */
namespace MKQR
{
	class Validator;
	/*!@brief The MKQR::Generator class contains functionality for generating and
	 * validating MKQR codes. MKQR::Generator implements all details of the MKQR 
	 * standard for generating such codes */
	class Generator
	{
		friend Validator;
	private:
		/*!@brief Instance of the validator class which validates each parameter */
		const Validator* mValidator;

		/*!@brief Holds all parameter values */
		std::unordered_map<std::string, std::string> mParameters;

		/*!@brief If true, a fatal error has occured and there is no need to continue
		 * executing any functions any further. 
		 */
		bool mFatalError = false;

		/*!@brief Gets a parameter value based on the name, used only for validation 
		 *
		 * @param name The name of the parameter
		 * 
		 * @return The value of the parameter
		*/
		[[nodiscard]] std::string GetParameterValue(const std::string& name) const noexcept;

		/*!@brief Iterates over all added parametersand generates a single MKQR URI string
		 * from them
		 * 
		 * @pre Should not execute if one of the mandatory parameters is invalid, should 
		 * execute if one of the non-mandatory parameters is invalid
		 *
		 * @return A string containing a valid MKQR URI */
		[[nodiscard]] std::string GenerateStringFromParameters() const noexcept;

		/*!@brief Maps value that is between low1 and high1 to a value that is the same 
		 * distance both from low2 and high2
		 * 
		 * @param value The value to be maped to a different range 
		 * @param low1 The lowest possible value in the original range 
		 * @param high1 The highest possible value in the original range 
		 * @param low2 The lowest possible value in the new range 
		 * @param high2 The highest possible value in the new range 
		 * 
		 * @return The value mapped to the new range */
		template<typename T>
		[[nodiscard]] T MapRange(T value, T low1, T high1, T low2, T high2) const noexcept
		{
			return low2 + (value - low1) * (high2 - low2) / (high1 - low1);
		}

		/*!@brief Interpolates linearly between a and b with a given normalized alpha value
		 * 
		 * @param alpha The point between a and b, 0.0f will return a, 1.0f will return b,
		 * 0.5f will return the mid point between a and b
		 * @param a The first point
		 * @param b The second point
		 * 
		 * @return The point between a and b based on the alpha
		*/
		template<typename T>
		[[nodiscard]] T Lerp(float alpha, T a, T b) const noexcept
		{
			return (T)(a + alpha * (b - a));
		}

		/*!@brief Interpolates linearly between color1 and color2 with the given alpha value 
		 *
		 * @param color1 The first color
		 * @param color2 The second color
		 * @param alpha The point between color1 and color2, 0.0f will return color1,
		 * 1.0f will return color2, 0.5f will return the mid point between color1 and color2
		 * 
		 * @return The point between color1 and color2 based on the alpha
		*/
		[[nodiscard]] uint32_t LerpColor(uint32_t color1, 
			uint32_t color2, float alpha) const noexcept;

		/*!@brief The size of the QR matrix in a single dimension, represents the number 
		 * of segments in the QR code. To calculate the total segments, square this */
		size_t mQrMatrixSize = 0;

		/*!@brief The QR matrix, true is for black, false is for white. 
		 * Contains mQrMatrixSize*mQrMatrixSize number of elements  */
		std::vector<bool> mQrMatrix;

		/*!@brief The QR image data (all pixels). Contains mQrMatrixSize * mSuperSampling
		 * number of elements. Each pixel is represented by a 32-bit value, of which
		 * only 24 are used, the alpha channel is always set to 255 */
		std::vector<uint32_t> mQrImageData;

		/*!@brief If set, the generated QR code will be black and white. Otherwise it will
		 * generate the colored version */
		bool mIsMonochrome = true;

		/*!@brief If set to 1 the resulting QR code will have the same amount of pixels 
		 * as segments. Setting it to 2 will result with the QR code image having 2 times
		 * as much pixels as segments, and so on*/
		size_t mSuperSampling = 1;

		/*!@brief Last recorded error message. Use the MKQR_ERR macro to set this */
		std::string mLastErrorMessage;

		/*!@brief Internal use only. Used for copying the last error message when
		 * it's removed */
		char* mTempErrorMessage = nullptr;

		/*!@brief Last recorded error code. Use the MKQR_ERR macro to set this */
		uint8_t mLastErrorCode = MKQR_ERR_NO;
	public:
		/*!@brief Default constructor. Used to initialize the validator */
		Generator();

		/*!@brief Default destructor. Is used to delete the temporary error message used 
		 * when copying if it exists */
		~Generator();

		/*!@brief Creates parameter with the given name and value. Additionaly, the 
		 * validation of the parameter is done inside this function. If a validation 
		 * fails this function will set the mLastErrorMessage and mLastErrorCode. 
		 * If it fails any subsequent calls to the Generate function will fail.
		 * to the 
		 * Example:  
		 * name=t
		 * value=0
		 * This will result with a new parameter t=0 which will be added to the map 
		 * of parameters 
		 * 
		 * @param name The name of the parameter
		 * @param value The value of the parameter
		 * 
		 * @return void
		 */
		void CreateParameter(const std::string& name, const std::string& value) noexcept;

		/*!@brief Validates a parameter value
		* 
		* @param name The name of the parameter
		* @param value The value of the parameter
		* 
		* @return If valid returns 0, if not valid and not mandatory returns 1, if 
		* not valid and mandatory returns 2
		*/
		uint8_t ValidateParameter(const std::string& name, 
							      const std::string& value) const noexcept;

		/*!@brief Generates the MKQR code based on the previously entered parameters.
		 * This function will fail if a previously created parameter is invalid. The
		 * result of the function is kept inside the mQrImageData variable, which can
		 * then be retrieved by the GetQrImageDataAtIndex function.
		 * 
		 * @param isMonochrome Can be either 0 or 1. Setting this parameter to 0 will
		 * create a MKQR code in full color. Setting it to 1 means the MKQR will be 
		 * black and white.
		 * @param superSampling Sets the super-sampling value. A value of 1 will 
		 * generate a bitmap with mQrMatrixSize*mQrMatrixSize pixels. A value of 2
		 * will generate twice*twice that, and so on.
		 * 
		 * @return void
		 */
		void Generate(uint8_t isMonochrome, size_t superSampling) noexcept;

		/*!@brief Gets the size of the QR image.
		 * 
		 * @return mQrMatrixSize * mSuperSampling
		 */
		[[nodiscard]] size_t GetQrSize() const noexcept 
		{ return mQrMatrixSize * mSuperSampling; }

		/*!@brief Gets the color of the pixel at the specified index 
		 * 
		 * @param  index The absolute index of the pixel. If you have 2 rows of 32
		 * pixels the fourth pixel in the second row will have an index of 36
		 * 
		 * @return 32-bit color value of the pixel, alpha is always 0xFF
		 */
		[[nodiscard]] uint32_t GetQrImageDataAtIndex(size_t index) const noexcept 
		{ return mQrImageData[index]; }

		/*!@brief Gets the last error message
		 * 
		 * @return The error message that occured most recently
		 */
		[[nodiscard]] const char* GetLastErrorMessage() noexcept;
		
		/*!@brief Gets the last error code
		 *
		 * @return The error code that occured most recently
		 */
		[[nodiscard]] uint8_t GetLastErrorCode() noexcept;
	};
}