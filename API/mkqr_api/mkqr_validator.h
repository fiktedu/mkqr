#pragma once
#include <regex>
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <queue>

/*!@brief This is the delimiter for combined strings */
#define MKQR_STR_DELIMITER '|'

/*!@brief Binds a function pointer as a validator function
 *
 * @param fnName The name of the function to do the validation
 * @param paramValue The value of the additional parameter. You can separate multiple
 * parameters by using the pipe | delimiter
 */
#define MKQR_VBIND(fnName, paramValue) { std::bind(&MKQR::Validator::fnName, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), paramValue }

 /*!@brief Defines a validator which is a pair of property name and queue of functions
 * that need to return true for the parameter to be validated
 *
 * example: MKQR_VALIDATOR("name", MKQR_VBIND(IsEqual, "name"), MKQR_VBIND(FixedLength, "4"))
 *
 * @param propertyName The name of the property of this validator queue
 * @param ... One or multiple MKQR_BIND calls which make the validator queue
 *
 */
#define MKQR_VALIDATOR(propertyName, ...) {propertyName,  { __VA_ARGS__ }}

namespace MKQR
{
	class Generator;

	/*!@brief This class contains functions that validate parameters. Please don't make it
	 * static or singleton and keep all functions const, inlined by default ;)
	 */
	class Validator
	{
		/*!@brief All validator functions must have 3x string parameters and must
		 * return a bool value (is the parameter valid) */
		typedef std::pair<
			std::function<bool(const std::string&, const std::string&, std::string&)>,
			std::string> ValidatorFunction;

		/*!@brief The queue which is executed each time a parameter is validated, contains
		 * n number of functions */
		typedef std::vector<ValidatorFunction> ValidatorQueue;
	private:
		/*!@brief A reference to the generator class needed to get already
		 * registered parameters */
		const Generator& mGenerator;

		/*!@brief The core variable in this class. Contains all parameters
		 * with their function execution queues.
		 *
		 * @param std::string The name of the parameter
		 * @param ValidatorQueue The queue of functions which validates the parameter
		 */
		const std::unordered_map<std::string, ValidatorQueue> mValidators =
		{
			MKQR_VALIDATOR("t", MKQR_VBIND(IsEqual, "MKD")),
			MKQR_VALIDATOR("v", MKQR_VBIND(FixedLength, "4"), MKQR_VBIND(Number, "1")),
			MKQR_VALIDATOR("c", MKQR_VBIND(FixedLength, "1"), MKQR_VBIND(FixedChars, "12")),
			MKQR_VALIDATOR("iban", MKQR_VBIND(IBAN, "")),
			MKQR_VALIDATOR("aiban", MKQR_VBIND(AltIBAN, "")),
			MKQR_VALIDATOR("cat", MKQR_VBIND(FixedLength, "1"), MKQR_VBIND(FixedChars, "SK")),
			MKQR_VALIDATOR("cn", MKQR_VBIND(NonZeroMaxLength, "70")),
			MKQR_VALIDATOR("cadd1", MKQR_VBIND(SwitchOnSK, "16|70")),
			MKQR_VALIDATOR("cadd2", MKQR_VBIND(SwitchOnSK, "16|70")),
			MKQR_VALIDATOR("cz", MKQR_VBIND(SwitchOnSK, "7|0"), MKQR_VBIND(Number, "1")),
			MKQR_VALIDATOR("cg", MKQR_VBIND(SwitchOnSK, "35|0")),
			MKQR_VALIDATOR("cc", MKQR_VBIND(FixedLength, "2"), MKQR_VBIND(CountryCode, "")),
			MKQR_VALIDATOR("a", MKQR_VBIND(IsDoublePositiveNumber, ""), MKQR_VBIND(NonZeroMaxLength, "1079")), // 64-bit IEEE double
			MKQR_VALIDATOR("cur", MKQR_VBIND(FixedLength, "3"), MKQR_VBIND(CurrencyCode, "")),
			MKQR_VALIDATOR("pat", MKQR_VBIND(FixedLength, "1"), MKQR_VBIND(FixedChars, "SK")),
			MKQR_VALIDATOR("pn", MKQR_VBIND(NonZeroMaxLength, "70")),
			MKQR_VALIDATOR("padd1", MKQR_VBIND(SwitchOnSK, "16|70")),
			MKQR_VALIDATOR("padd2", MKQR_VBIND(SwitchOnSK, "16|70")),
			MKQR_VALIDATOR("pz", MKQR_VBIND(SwitchOnSK, "7|0"),  MKQR_VBIND(Number, "1")),
			MKQR_VALIDATOR("pg", MKQR_VBIND(SwitchOnSK, "35|0")),
			MKQR_VALIDATOR("pc", MKQR_VBIND(FixedLength, "2"), MKQR_VBIND(CountryCode, "")),
			// TODO: missing rt and ref
			MKQR_VALIDATOR("pcd", MKQR_VBIND(FixedLength, "3"), MKQR_VBIND(IsDoublePositiveNumber, "")),
			MKQR_VALIDATOR("nac", MKQR_VBIND(FixedLength, "1"), MKQR_VBIND(IsDoublePositiveNumber, "")),
			MKQR_VALIDATOR("us50", MKQR_VBIND(FixedLength, "15"), MKQR_VBIND(Number, "1")),
			MKQR_VALIDATOR("usek50", MKQR_VBIND(FixedLength, "15"), MKQR_VBIND(Number, "1")),
			MKQR_VALIDATOR("us30", MKQR_VBIND(FixedLength, "15"), MKQR_VBIND(Number, "1")),
			MKQR_VALIDATOR("usek30", MKQR_VBIND(FixedLength, "15"), MKQR_VBIND(Number, "1")),
			// TODO: missing additional info field validation
			MKQR_VALIDATOR("curl", MKQR_VBIND(URL, ""))
			// TODO: missing alternative payment fields validation
		};

		/*!@brief Contains all the mandatory parameters */
		const std::vector<std::string> mMandatoryParameters =
		{
			"t", "v", "c", "iban", "cat", "cn", "cc", "cur", /*"rt",*/ "pcd" // TODO: add rt when validated
		};

		/*!@brief Contains all the conditional parameters which may or may not be mandatory
		 * depending on the context */
		const std::vector<std::string> mConditionalParameters =
		{
			"cadd1", "cz", "cg", "padd1", "pz", "pg"
		};

		/*!@brief Splits the string into components
		 *
		 * @param str The string to split
		 * @param delimiter The delimiter to split it with, use MKQR_STR_DELIMITER macro
		 *
		 * @return Vector of strings derived from str
		 */
		[[nodiscard]] std::vector<std::string> TokenizeString(const char* str, char delimiter) const;

		/*!@brief Checks if the provided string is a number.
		 *
		 * @param str The string to be checked
		 *
		 * @return True if str is a number, false if str is not a number
		 */
		[[nodiscard]] bool IsNumber(const std::string& str, bool isAnyNumber = 0) const;

		/*!@brief Checks if the provided string is a mandatory parameter **at the time**
		 * this check is executed. The parameter might appear not to be mandatory if all
		 * the prerequisites are not yet checked
		 *
		 * @param name The string to be checked
		 *
		 * @return True if name is a mandatory parameter, false if str is not a
		 * mandatory parameter
		 */
		[[nodiscard]] bool IsParameterMandatory(const std::string& name) const;

		// ==========================================================================================

		/*!@brief Validate IBAN
		 *
		 * @param ibanString The IBAN string to validate
		 * @param param Not used
		 * @param outMessage The message this function generates if it fails
		 *
		 * @return True if valid
		 */
		[[nodiscard]] bool IBAN(const std::string& ibanString,
			[[maybe_unused]] const std::string& param,
			std::string& outMessage) const;

		/*!@brief Splits aibanString into components and then evaluates each component separately
		 *
		 * @param aibanString The alternative IBANs to validate
		 * @param param Not used
		 * @param outMessage The message this function generates if it fails
		 *
		 * @return True if valid
		 */
		[[nodiscard]] bool AltIBAN(const std::string& aibanString,
			[[maybe_unused]] const std::string& param,
			std::string& outMessage) const;

		/*!@brief Checks if value is equal to param
		 *
		 * @param value The value to check
		 * @param param The value to check against
		 * @param outMessage The message this function generates if it fails
		 *
		 * @return True if value is equal to param
		*/
		[[nodiscard]] bool IsEqual(const std::string& value,
			const std::string& param,
			std::string& outMessage) const;

		/*!@brief Checks if the length is greater than zero and less than param
		 *
		 * @param value The value on which to check the length
		 * @param param The maximum value, must be a number
		 * @param outMessage The message this function generates if it fails
		 *
		 * @return True if the length of value is non-zero and less than param
		 */
		[[nodiscard]] bool NonZeroMaxLength(const std::string& value,
			const std::string& param,
			std::string& outMessage) const;

		/*!@brief Checks if value has a fixed length of param
		 *
		 * @param value The value on which to check the length
		 * @param param The actual length to check, must be a number
		 * @param outMessage The message this function generates if it fails
		 *
		 * @return True if the length of value is equal to param
		 */
		[[nodiscard]] bool FixedLength(const std::string& value,
			const std::string& param,
			std::string& outMessage) const;

		/*!@brief Checks if value has any/all characters specified in param
		 *
		 * @param value The string on which to check the characters
		 * @param param All possible characters that can appear in value
		 * @param outMessage The message this function generates if it fails
		 *
		 * @return True if value has only the characters specified in param
		 */
		[[nodiscard]] bool FixedChars(const std::string& value,
			const std::string& param,
			std::string& outMessage) const;

		/*!@brief Checks if value is a country code
		 *
		 * @param value The string to check
		 * @param param Not used
		 * @param outMessage The message this function generates if it fails
		 *
		 * @return True if value is a country code
		 */
		[[nodiscard]] bool CountryCode(const std::string& value,
			[[maybe_unused]] const std::string& param,
			std::string& outMessage) const;

		/*!@brief Checks if value is a positive number with double precision
		 *
		 * @param value The string to check
		 * @param param Not used
		 * @param outMessage The message this function generates if it fails
		 *
		 * @return True if value is a positive number with double precision
		 */
		[[nodiscard]] bool IsDoublePositiveNumber(const std::string& value,
			[[maybe_unused]] const std::string& param,
			std::string& outMessage) const;

		/*!@brief Checks if value is a currency code
		 *
		 * @param value The string to check
		 * @param param Not used
		 * @param outMessage The message this function generates if it fails
		 *
		 * @return True if value is a currency code
		 */
		[[nodiscard]] bool CurrencyCode(const std::string& value,
			[[maybe_unused]] const std::string& param,
			std::string& outMessage) const;

		/*!@brief Checks special constranints for "cat" and "pat" parameters
		 *
		 * @param value The string to check
		 * @param param Minimum and maximum length of characters for the
		 * specified parameter
		 * @param outMessage The message this function generates if it fails
		 *
		 * @return True if value is a currency code
		 */
		[[nodiscard]] bool SwitchOnSK(const std::string& value,
			const std::string& param,
			std::string& outMessage) const;

		/*!@brief Checks if the provided URL is valid
		 *
		 * @param value The string to check
		 * @param param Not used
		 * @param outMessage The message this function generates if it fails
		 *
		 * @return True if value is a valid URL
		 */
		[[nodiscard]] bool URL(const std::string& value,
			const std::string& param,
			std::string& outMessage);

		/*!@brief Checks if the provided string is a number
		 *
		 * @param value The string to check
		 * @param param 0 all numbers, 1 real numbers
		 * @param outMessage The message this function generates if it fails
		 *
		 * @return True if value is a valid number
		 */
		[[nodiscard]] bool Number(const std::string& value,
			const std::string& param,
			std::string& outMessage);

		// ==========================================================================================
	public:
		/*!@brief The validator needs a reference to the Generator, do that here
		 *
		 * @param owner The owner generator
		 */
		Validator(const Generator& owner)
			: mGenerator(owner) {}

		/*!@brief The validator needs a reference to the Generator, do that here
		 * The validator result, can be interpreted by the Validator easily
		*/
		struct SResult
		{
		public:

			/*!@brief
			* @param Ok All is good and validated
			* @param Warning Some non-mandatory validations failed
			* @param Error Validation failed on mandatory properties
			*/
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

			ELevel GetLevel() const { return mLevel; }
			const std::string& GetMessage() const { return mMessage; }
		};

		/*!@brief Gets all mandatory parameters at this point. This also 
		 * includes optional parameters which have been evaluated as 
		 * mandatory
		 * 
		 * @reutrn A vector of all mandatory parameters at this point
		 */
		[[nodiscard]] std::vector<std::string> GetMandatoryParameters() const;

		/*!@brief Gets all mandatory parameters at this point. This also
		 * includes optional parameters which have been evaluated as
		 * mandatory
		 *
		 * @return A vector of all mandatory parameters at this point
		 */
		[[nodiscard]] SResult ValidateParameter(
			const std::string& name, const std::string& value) const;

		// ==========================================================================================
	private:
		/*!@brief Holds all country codes as of year 2022*/
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

		/*!@brief Holds all currency codes as of year 2022*/
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