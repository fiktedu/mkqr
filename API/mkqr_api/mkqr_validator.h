#pragma once
#include <regex>
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <queue>

#define MKQR_STR_DELIMITER '|'
#define MKQR_VBIND(fnName, paramValue) { std::bind(&MKQR::Validator::fnName, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), paramValue }
#define MKQR_VALIDATOR(propertyName, ...) {propertyName,  { __VA_ARGS__ }}

namespace MKQR
{
	/*!@brief This class contains functions that validate parameters. Please don't make it
	 * static or singleton and keep all functions const, inlined by default ;)
	 * Splitting it to a cpp file is unnecesarry if you keep all functions const
	 */
	class Validator
	{
		typedef std::pair<
			std::function<bool(const std::string&, const std::string&, std::string&)>,
			std::string> ValidatorFunction;

		typedef std::vector<ValidatorFunction> ValidatorQueue;
	private:
		const Generator& mGenerator;

		const std::unordered_map<std::string, ValidatorQueue> mValidators =
		{
			MKQR_VALIDATOR("t", MKQR_VBIND(IsEqual, "MKD")),
			MKQR_VALIDATOR("v", MKQR_VBIND(FixedLength, "4")),
			MKQR_VALIDATOR("c", MKQR_VBIND(FixedLength, "1"), MKQR_VBIND(FixedChars, "12")),
			MKQR_VALIDATOR("iban", MKQR_VBIND(IBAN, "")),
			MKQR_VALIDATOR("aiban", MKQR_VBIND(AltIBAN, "")),
			MKQR_VALIDATOR("cat", MKQR_VBIND(FixedLength, "1"), MKQR_VBIND(FixedChars, "SK")),
			MKQR_VALIDATOR("cn", MKQR_VBIND(NonZeroMaxLength, "70")),
			MKQR_VALIDATOR("cadd1", MKQR_VBIND(SwitchOnSK, "16|70")),
			MKQR_VALIDATOR("cadd2", MKQR_VBIND(SwitchOnSK, "16|70")),
			MKQR_VALIDATOR("cz", MKQR_VBIND(SwitchOnSK, "7|0")),
			MKQR_VALIDATOR("cg", MKQR_VBIND(SwitchOnSK, "35|0")),
			MKQR_VALIDATOR("cc", MKQR_VBIND(FixedLength, "2"), MKQR_VBIND(CountryCode, "")),
			MKQR_VALIDATOR("a", MKQR_VBIND(IsDoublePositiveNumber, ""), MKQR_VBIND(NonZeroMaxLength, "1079")), // 64-bit IEEE double
			MKQR_VALIDATOR("cur", MKQR_VBIND(FixedLength, "3"), MKQR_VBIND(CurrencyCode, "")),
			MKQR_VALIDATOR("pat", MKQR_VBIND(FixedLength, "1"), MKQR_VBIND(FixedChars, "SK")),
			MKQR_VALIDATOR("pn", MKQR_VBIND(NonZeroMaxLength, "70")),
			MKQR_VALIDATOR("padd1", MKQR_VBIND(SwitchOnSK, "16|70")),
			MKQR_VALIDATOR("padd2", MKQR_VBIND(SwitchOnSK, "16|70")),
			MKQR_VALIDATOR("pz", MKQR_VBIND(SwitchOnSK, "7|0")),
			MKQR_VALIDATOR("pg", MKQR_VBIND(SwitchOnSK, "35|0")),
			MKQR_VALIDATOR("pc", MKQR_VBIND(FixedLength, "2"), MKQR_VBIND(CountryCode, "")),
			// TODO: missing rt and ref
			MKQR_VALIDATOR("pcd", MKQR_VBIND(FixedLength, "3"), MKQR_VBIND(IsDoublePositiveNumber, "")),
			MKQR_VALIDATOR("pcd", MKQR_VBIND(FixedLength, "1"), MKQR_VBIND(IsDoublePositiveNumber, "")),
			// TODO: missing validations for PP30 and PP50
			// TODO: missing additional info field validation
			// TODO: missing curl validation
			// TODO: missing alternative payment fields validation
		};

		const std::vector<std::string> mMandatoryParameters =
		{
			"t", "v", "c", "iban", "cat", "cn", "cc", "cur", "rt", "pcd"
		};

		/*!@brief Splits the string into components
		 *
		 * @param str The string to split
		 * @param delimiter The delimiter to split it with, use MKQR_STR_DELIMITER macro
		 *
		 * @return Vector of strings derived from str
		 */
		[[nodiscard]] std::vector<std::string> TokenizeString(const char* str, char delimiter) const
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


		[[nodiscard]] bool IsNumber(const std::string& str) const
		{
			for (char const& c : str)
				if (std::isdigit(c) == 0 || c == '.' || c == '-')
					return false;
			return true;
		}

		[[nodiscard]] bool IsParameterMandatory(const std::string& name) const
		{
			bool retVal = std::find(mMandatoryParameters.begin(), mMandatoryParameters.end(), name) != mMandatoryParameters.end();
			if (!retVal)
			{
				if (name == "cadd1")
				{
					const std::string& value = mGenerator.GetParameterValue("cat");
					retVal = (value == "K");
				}
				else if (name == "cz")
				{
					const std::string& value = mGenerator.GetParameterValue("cat");
					retVal = (value == "S");
				}
				else if (name == "cg")
				{
					const std::string& value = mGenerator.GetParameterValue("cat");
					retVal = (value == "S");
				}
				else if (name == "padd1")
				{
					const std::string& value = mGenerator.GetParameterValue("pat");
					retVal = (value == "K");
				}
				else if (name == "pz")
				{
					const std::string& value = mGenerator.GetParameterValue("pat");
					retVal = (value == "S");
				}
				else if (name == "pg")
				{
					const std::string& value = mGenerator.GetParameterValue("pat");
					retVal = (value == "S");
				}
			}

			return retVal;
		}

		// ==========================================================================================

		/*!@brief Validate IBAN
		 *
		 * @param ibanString The IBAN string to validate
		 *
		 * @return True if valid, false if not valid
		 */
		[[nodiscard]] bool IBAN(const std::string& ibanString,
			[[maybe_unused]] const std::string& param,
			std::string& outMessage) const
		{
			outMessage += "Supplied IBAN not valid.";
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
		[[nodiscard]] bool AltIBAN(const std::string& aibanString,
			[[maybe_unused]] const std::string& param,
			std::string& outMessage) const
		{
			std::vector<std::string> tokens = TokenizeString(aibanString.c_str(), MKQR_STR_DELIMITER);
			bool retVal = true;
			for (const std::string& token : tokens)
			{
				if (!IBAN(token, "", outMessage))
				{
					retVal = false;
					break;
				}
			}

			return retVal;
		}

		[[nodiscard]] bool IsEqual(const std::string& value,
			const std::string& param,
			std::string& outMessage) const
		{
			outMessage = "Value " + value + " is not equal to " + param;
			return value == param;
		}

		[[nodiscard]] bool NonZeroMaxLength(const std::string& value,
			const std::string& param,
			std::string& outMessage) const
		{
			outMessage = "Value " + value + " is not between 0 and " + param;
			size_t l = atoll(param.c_str());
			return value.length() > 0 && value.length() < l;
		}

		[[nodiscard]] bool FixedLength(const std::string& value,
			const std::string& param,
			std::string& outMessage) const
		{
			outMessage = "Value " + value + " must have a fixed length of " + param;
			size_t l = atoll(param.c_str());
			return value.length() == l;
		}

		[[nodiscard]] bool FixedChars(const std::string& value,
			const std::string& param,
			std::string& outMessage) const
		{
			outMessage = "Value " + value + " has invalid characters. Expected characters: " + param;
			for (char cv : value)
			{
				bool found = false;
				for (char cp : param)
				{
					if (cp == cv)
					{
						found = true;
						break;
					}
				}

				if (!found)
					return false;
			}
			return true;
		}

		[[nodiscard]] bool CountryCode(const std::string& value,
			[[maybe_unused]] const std::string& param,
			std::string& outMessage) const
		{
			outMessage = "Value " + value + " must be a valid country code";
			return std::find(mCountryCodes.begin(), mCountryCodes.end(), value) != mCountryCodes.end();
		}

		[[nodiscard]] bool IsDoublePositiveNumber(const std::string& value,
			[[maybe_unused]] const std::string& param,
			std::string& outMessage) const
		{
			outMessage = "Value " + value + " must be a double-precision positive number";
			bool retVal = IsNumber(value);
			if (retVal)
			{
				double amount = std::stod(value);
				retVal = (amount >= 0.0);
			}
			return retVal;
		}

		[[nodiscard]] bool CurrencyCode(const std::string& value,
			[[maybe_unused]] const std::string& param,
			std::string& outMessage) const
		{
			outMessage = "Value " + value + " must be a valid currency code";
			return std::find(mCurrencyCodes.begin(), mCurrencyCodes.end(), value) != mCurrencyCodes.end();
		}

		[[nodiscard]] bool SwitchOnSK(const std::string& value,
			[[maybe_unused]] const std::string& param,
			std::string& outMessage) const
		{
			bool retVal = false;
			outMessage = "Value " + value + " is out of range";
			const std::string& valueCat = mGenerator.GetParameterValue("cat");
			const std::vector<std::string>& tokens = TokenizeString(param.c_str(), MKQR_STR_DELIMITER);
			if (tokens.size() == 2)
			{
				outMessage = "Number of tokens in " + param + " is not correct";
				const uint8_t valueS = (uint8_t)atoi(tokens[0].c_str());
				const uint8_t valueK = (uint8_t)atoi(tokens[1].c_str());

				const size_t valueLength = value.length();

				if (valueCat == "S" && valueLength > 0 && valueLength <= valueS)
				{
					retVal = true;
				}
				else if (valueCat == "K" && valueLength > 0 && valueLength <= valueK)
				{
					retVal = true;
				}
			}

			return retVal;
		}

		// ==========================================================================================
	public:
		Validator(const Generator& owner)
			: mGenerator(owner) {}

		struct SResult
		{
		public:
			enum class ELevel
			{
				Ok,
				Warning,
				Error
			};
		private:
			ELevel mLevel;
			std::string mMessage;
		public:
			SResult()
				: mLevel(ELevel::Ok),
				mMessage("")
			{ }

			SResult(ELevel level, const std::string& message)
				: mLevel(level),
				mMessage(message)
			{ }

			ELevel GetLevel() { return mLevel; }
			const std::string& GetMessage() { return mMessage; }
		};

		[[nodiscard]] SResult ValidateParameter(const std::string& name, const std::string& value) const
		{
			SResult retVal;
			const auto& paramIt = mValidators.find(name);
			if (paramIt != mValidators.end())
			{
				const ValidatorQueue& queue = paramIt->second;
				for (const ValidatorFunction& fn : queue)
				{
					std::string message;
					bool success = fn.first(value, fn.second, message);
					bool isMandatory = IsParameterMandatory(name);
					if (!success)
					{
						retVal = SResult(isMandatory ? SResult::ELevel::Error : SResult::ELevel::Warning,
							"Parameter " + name + ": " + message);
						break;
					}
				}
			}
			else
			{
				retVal = SResult(SResult::ELevel::Error, "Parameter " + name + " does not exist");
			}

			return retVal;
		}

		// ==========================================================================================
	private:
		const std::vector<std::string> mCountryCodes = { "AF", "AX", "AL", "DZ", "AS", "AD",
"AO", "AI", "AQ", "AG", "AR", "AM", "AW", "AU", "AT", "AZ", "BS", "BH", "BD", "BB","BY", "BE",
"BZ", "BJ","BM","BT","BO","BA","BW","BV","BR","IO","BN","BG","BF","BI","KH","CM","CA","CV","KY",
"CF","TD","CL","CN","CX","CC","CO","KM","CG","CD","CK","CR","CI","HR","CU","CY","CZ","DK","DJ",
"DM","DO","EC","EG","SV","GQ","ER","EE","ET","FK","FO","FJ","FI","FR","GF","PF","TF","GA","GM",
"GE","DE","GH","GI","GR","GL","GD","GP","GU","GT","GG","GN","GW","GY","HT","HM","VA","HN","HK",
"HU","IS","IN","ID","IR","IQ","IE","IM","IL","IT","JM","JP","JE","JO","KZ","KE","KI","KR","KW",
"KG","LA","LV","LB","LS","LR","LY","LI","LT","LU","MO","MK","MG","MW","MY","MV","ML","MT","MH",
"MQ","MR","MU","YT","MX","FM","MD","MC","MN","ME","MS","MA","MZ","MM","NA","NR","NP","NL","AN",
"NC","NZ","NI","NE","NG","NU","NF","MP","NO","OM","PK","PW","PS","PA","PG","PY","PE","PH","PN",
"PL","PT","PR","QA","RE","RO","RU","RW","BL","SH","KN","LC","MF","PM","VC","WS","SM","ST","SA",
"SN","RS","SC","SL","SG","SK","SI","SB","SO","ZA","GS","ES","LK","SD","SR","SJ","SZ","SE","CH",
"SY","TW","TJ","TZ","TH","TL","TG","TK","TO","TT","TN","TR","TM","TC","TV","UG","UA","AE","GB",
"US","UM","UY","UZ","VU","VE","VN","VG","VI","WF","EH","YE","ZM","ZW" };

		const std::vector<std::string> mCurrencyCodes = {
"AFA","AWG","AUD","ARS","AZN","BSD","BDT","BBD","BYR","BOB","BRL","GBP","BGN","KHR","CAD","KYD",
"CLP","CNY","COP","CRC","HRK","CPY","CZK","DKK","DOP","XCD","EGP","ERN","EEK","EUR","GEL","GHC",
"GIP","GTQ","HNL","HKD","HUF","ISK","INR","IDR","ILS","JMD","JPY","KZT","KES","KWD","LVL","LBP",
"LTL","MOP","MKD","MGA","MYR","MTL","BAM","MUR","MXN","MZM","NPR","ANG","TWD","NZD","NIO","NGN",
"KPW","NOK","OMR","PKR","PYG","PEN","PHP","QAR","RON","RUB","SAR","CSD","SCR","SGD","SKK","SIT",
"ZAR","KRW","LKR","SRD","SEK","CHF","TZS","THB","TTD","TRY","AED","USD","UGX","UAH","UYU","UZS",
"VEB","VND","AMK","ZWD"
		};
	};
}