#include "mkqr_validator.h"
#include "mkqr_generator.h"

std::vector<std::string> MKQR::Validator::TokenizeString(const char* str, char delimiter) const
{
	std::vector<std::string> tokens;

	do
	{
		const char* begin = str;

		while (*str != delimiter && *str)
			str++;

		tokens.push_back(std::string(begin, str));
	} while (0 != *str++);

	return tokens;
}

bool MKQR::Validator::IsNumber(const std::string& str, bool isAnyNumber) const
{
	std::regex anyRegex("/^-?\\d+\\.?\\d*$/");
	std::regex naturalRegex("^[0-9]+$");
	return std::regex_match(str, isAnyNumber ? anyRegex : naturalRegex);
}

bool MKQR::Validator::IsParameterMandatory(const std::string& name) const
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

bool MKQR::Validator::IBAN(
	const std::string& ibanString,
	[[maybe_unused]] const std::string& param,
	std::string& outMessage) const
{
	outMessage += "Supplied IBAN is not valid.";
	const std::regex ibanRegex(
		"^([A-Z]{2}[ \\-]?[0-9]{2})(?=(?:[ \\-]?[A-Z0-9]){9,30}$)((?:[ \\-]?[A-Z0-9]{3,5}){2,7})([ \\-]?[A-Z0-9]{1,3})?$");
	return std::regex_match(ibanString, ibanRegex);
}

bool MKQR::Validator::AltIBAN(
	const std::string& aibanString,
	[[maybe_unused]] const std::string& param,
	std::string& outMessage) const
{
	const std::vector<std::string> tokens = TokenizeString(aibanString.c_str(), MKQR_STR_DELIMITER);
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

bool MKQR::Validator::IsEqual(const std::string& value, const std::string& param, std::string& outMessage) const
{
	outMessage = "Value " + value + " is not equal to " + param;
	return value == param;
}

bool MKQR::Validator::NonZeroMaxLength(const std::string& value, const std::string& param, std::string& outMessage) const
{
	outMessage = "Value " + value + " is not between 0 and " + param;
	size_t l = atoll(param.c_str());
	return value.length() > 0 && value.length() < l;
}

bool MKQR::Validator::FixedLength(const std::string& value, const std::string& param, std::string& outMessage) const
{
	outMessage = "Value " + value + " must have a fixed length of " + param;
	size_t l = atoll(param.c_str());
	return value.length() == l;
}

bool MKQR::Validator::FixedChars(const std::string& value, const std::string& param, std::string& outMessage) const
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

bool MKQR::Validator::CountryCode(
	const std::string& value,
	[[maybe_unused]] const std::string& param,
	std::string& outMessage) const
{
	outMessage = "Value " + value + " must be a valid country code";
	return std::find(mCountryCodes.begin(), mCountryCodes.end(), value) != mCountryCodes.end();
}

bool MKQR::Validator::DoublePositiveNumber(
	const std::string& value,
	[[maybe_unused]] const std::string& param,
	std::string& outMessage) const
{
	outMessage = "Value " + value + " must be a double-precision positive number";
	bool retVal = IsNumber(value);
	if (retVal)
	{
		const double amount = std::stod(value);
		retVal = (amount >= 0.0);
	}
	return retVal;
}

bool MKQR::Validator::CurrencyCode(
	const std::string& value,
	[[maybe_unused]] const std::string& param,
	std::string& outMessage) const
{
	outMessage = "Value " + value + " must be a valid currency code";
	return std::find(mCurrencyCodes.begin(), mCurrencyCodes.end(), value) != mCurrencyCodes.end();
}

bool MKQR::Validator::SwitchOnSK(const std::string& value, const std::string& param, std::string& outMessage) const
{
	bool retVal = false;
	outMessage = "Value " + value + " is out of range";
	const std::string& valueCat = mGenerator.GetParameterValue("cat");
	const std::vector<std::string>& tokens = TokenizeString(param.c_str(), MKQR_STR_DELIMITER);
	if (tokens.size() == 2)
	{
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
	else
	{
		outMessage = "Number of tokens in " + param + " is not correct";
	}

	return retVal;
}

bool MKQR::Validator::URL(const std::string& value, [[maybe_unused]] const std::string& param, std::string& outMessage)
{
	outMessage += "Supplied URL is not valid.";
	const std::regex urlRegex(
		"(http|https)://((\\w)*|([0-9]*)|([-|_])*)+([\\.|/]((\\w)*|([0-9]*)|([-|_])*))+");

	return std::regex_match(value, urlRegex);
}

bool MKQR::Validator::Number(const std::string& value, const std::string& param, std::string& outMessage)
{
	outMessage = "String " + value + " is not a number.";
	if (param == "0")
		return IsNumber(value, true);
	else if (param == "1")
		return IsNumber(value, false);
	return false;
}

std::vector<std::string> MKQR::Validator::GetMandatoryParameters() const
{
	std::vector<std::string> retVal = mMandatoryParameters;

	for (const std::string& cparam : mConditionalParameters)
	{
		if (IsParameterMandatory(cparam))
		{
			retVal.push_back(cparam);
		}
	}
	return retVal;
}

MKQR::Validator::SResult MKQR::Validator::ValidateParameter(const std::string& name, const std::string& value) const
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