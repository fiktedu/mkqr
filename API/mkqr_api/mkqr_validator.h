#pragma once
#include <regex>
#include <string>
#include <vector>

#define MKQR_STR_DELIMITER '|'

namespace MKQR
{
	/*!@brief This class contains functions that validate parameters. Please don't make it
	 * static or singleton, keep all functions const and refrain from using member variables*/
	class Validator
	{
	private:

		/*!@brief Splits the string into components
		 * 
		 * @param str The string to split
		 * @param delimiter The delimiter to split it with, use MKQR_STR_DELIMITER macro
		 * 
		 * @return Vector of strings derived from str
		 */
		std::vector<std::string> TokenizeString(const char* str, char delimiter) const
		{
			std::vector<std::string> tokens;

			while (0 != *str++)
			{
				const char* begin = str;

				while (*str != delimiter && *str)
					str++;

				tokens.push_back(std::string(begin, str));
			}

			return tokens;
		}

	public:
		/*!@brief Validate IBAN
		 * 
		 * @param ibanString The IBAN string to validate
		 * 
		 * @return True if valid, false if not valid
		 */
		bool ValidateStringIBAN(const std::string& ibanString) const
		{
			const std::regex ibanRegex(
				"^([A-Z]{2}[ \\-]?[0-9]{2})(?=(?:[ \\-]?[A-Z0-9]){9,30}$)((?:[ \\-]?[A-Z0-9]{3,5}){2,7})([ \\-]?[A-Z0-9]{1,3})?$");
			return std::regex_match(ibanString, ibanRegex);
		}

		/*!@brief Splits aibanString into components and then evaluates each component separately
		 *
		 * @param aibanString The alternative IBAN string to validate
		 *
		 * @return True if valid, false if not valid
		 */
		bool ValidateStringAltIBAN(const std::string& aibanString) const
		{
			std::vector<std::string> tokens = TokenizeString(aibanString.c_str(), MKQR_STR_DELIMITER);
			bool retVal = true;
			for (const std::string& token : tokens)
			{
				if (!ValidateStringIBAN(token))
				{
					retVal = false;
					break;
				}
			}

			return retVal;
		}
	};
}