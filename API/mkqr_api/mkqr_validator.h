#pragma once
#include <regex>
#include <string>
#include <vector>

#define MKQR_STR_DELIMITER '|'

namespace MKQR
{
	class Validator
	{
	private:
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
		bool ValidateStringIBAN(const std::string& ibanString) const
		{
			const std::regex ibanRegex(
				"^([A-Z]{2}[ \\-]?[0-9]{2})(?=(?:[ \\-]?[A-Z0-9]){9,30}$)((?:[ \\-]?[A-Z0-9]{3,5}){2,7})([ \\-]?[A-Z0-9]{1,3})?$");
			return std::regex_match(ibanString, ibanRegex);
		}

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