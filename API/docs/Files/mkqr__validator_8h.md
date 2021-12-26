---
title: D:/Projects/mkqr/API/mkqr_api/mkqr_validator.h

---

# D:/Projects/mkqr/API/mkqr_api/mkqr_validator.h



## Namespaces

| Name           |
| -------------- |
| **[MKQR](/Namespaces/namespace_m_k_q_r.md)** <br>This is the default [MKQR]() namespace.Use it to wrap anything that is related to [MKQR]()'s internal systems.  |

## Classes

|                | Name           |
| -------------- | -------------- |
| class | **[MKQR::Validator](/Classes/class_m_k_q_r_1_1_validator.md)** <br>This class contains functions that validate parameters. Please don't make it static or singleton and keep all functions const, inlined by default ;)  |

## Defines

|                | Name           |
| -------------- | -------------- |
|  | **[MKQR_STR_DELIMITER](/Files/mkqr__validator_8h.md#define-mkqr-str-delimiter)**  |
|  | **[MKQR_VBIND](/Files/mkqr__validator_8h.md#define-mkqr-vbind)**(fnName, paramValue)  |
|  | **[MKQR_VALIDATOR](/Files/mkqr__validator_8h.md#define-mkqr-validator)**(propertyName, ...)  |




## Macros Documentation

### define MKQR_STR_DELIMITER

```cpp
#define MKQR_STR_DELIMITER '|'
```


### define MKQR_VBIND

```cpp
#define MKQR_VBIND(
    fnName,
    paramValue
)
{ std::bind(&MKQR::Validator::fnName, this, std::placeholders::_1, std::placeholders::_2), paramValue }
```


### define MKQR_VALIDATOR

```cpp
#define MKQR_VALIDATOR(
    propertyName,
    ...
)
{propertyName,  { __VA_ARGS__ }}
```


## Source code

```cpp
#pragma once
#include <regex>
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <queue>

#define MKQR_STR_DELIMITER '|'
#define MKQR_VBIND(fnName, paramValue) { std::bind(&MKQR::Validator::fnName, this, std::placeholders::_1, std::placeholders::_2), paramValue }
#define MKQR_VALIDATOR(propertyName, ...) {propertyName,  { __VA_ARGS__ }}

namespace MKQR
{
    class Validator
    {
        typedef std::pair<
            std::function<bool(const std::string&, const std::string&)>,
            std::string> ValidatorFunction;

        typedef std::vector<ValidatorFunction> ValidatorQueue;
    private:

        const std::unordered_map<std::string, ValidatorQueue> mValidators =
        {
            MKQR_VALIDATOR("t", MKQR_VBIND(IsEqual, "MKD")),
            MKQR_VALIDATOR("v", MKQR_VBIND(FixedLength, "4")),
            MKQR_VALIDATOR("c", MKQR_VBIND(FixedLength, "1"), MKQR_VBIND(FixedChars, "12")),
            MKQR_VALIDATOR("iban", MKQR_VBIND(IBAN, "")),
            MKQR_VALIDATOR("aiban", MKQR_VBIND(AltIBAN, "")),
            MKQR_VALIDATOR("cat", MKQR_VBIND(FixedLength, "1"), MKQR_VBIND(FixedChars, "SK")),
            MKQR_VALIDATOR("cn", MKQR_VBIND(NonZeroMaxLength, "70")),
            // TODO: missing checks for conditional parameters
            MKQR_VALIDATOR("cc", MKQR_VBIND(FixedLength, "2"), MKQR_VBIND(CountryCode, "")),
            MKQR_VALIDATOR("a", MKQR_VBIND(IsPositiveNumber, ""), MKQR_VBIND(NonZeroMaxLength, "1079")), // 64-bit IEEE double
            MKQR_VALIDATOR("cur", MKQR_VBIND(FixedLength, "3"), MKQR_VBIND(CurrencyCode, "")),
            MKQR_VALIDATOR("pat", MKQR_VBIND(FixedLength, "1"), MKQR_VBIND(FixedChars, "SK")),
            MKQR_VALIDATOR("pn", MKQR_VBIND(NonZeroMaxLength, "70")),
            // TODO: missing checks for conditional parameters
            MKQR_VALIDATOR("pc", MKQR_VBIND(FixedLength, "2"), MKQR_VBIND(CountryCode, "")),
            // TODO: missing rt and ref
            MKQR_VALIDATOR("pcd", MKQR_VBIND(FixedLength, "3"), MKQR_VBIND(IsPositiveNumber, "")),
            MKQR_VALIDATOR("pcd", MKQR_VBIND(FixedLength, "1"), MKQR_VBIND(IsPositiveNumber, "")),
            // TODO: missing validations for PP30 and PP50
            // TODO: missing additional info field validation
            // TODO: missing curl validation
            // TODO: missing alternative payment fields validation
        };

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


        bool IsNumber(const std::string& str) const
        {
            for (char const& c : str)
                if (std::isdigit(c) == 0 || c == '.' || c == '-')
                    return false;
            return true;
        }

        // ==========================================================================================

        bool IBAN(const std::string& ibanString, [[maybe_unused]] const std::string& param) const
        {
            const std::regex ibanRegex(
                "^([A-Z]{2}[ \\-]?[0-9]{2})(?=(?:[ \\-]?[A-Z0-9]){9,30}$)((?:[ \\-]?[A-Z0-9]{3,5}){2,7})([ \\-]?[A-Z0-9]{1,3})?$");
            return std::regex_match(ibanString, ibanRegex);
        }

        bool AltIBAN(const std::string& aibanString, [[maybe_unused]] const std::string& param) const
        {
            std::vector<std::string> tokens = TokenizeString(aibanString.c_str(), MKQR_STR_DELIMITER);
            bool retVal = true;
            for (const std::string& token : tokens)
            {
                if (!IBAN(token, ""))
                {
                    retVal = false;
                    break;
                }
            }

            return retVal;
        }

        bool IsEqual(const std::string& value, const std::string& param) const
        {
            return value == param;
        }

        bool NonZeroMaxLength(const std::string& value, const std::string& param) const
        {
            size_t l = atoll(param.c_str());
            return value.length() > 0 && value.length() < l;
        }

        bool FixedLength(const std::string& value, const std::string& param) const
        {
            size_t l = atoll(param.c_str());
            return value.length() == l;
        }

        bool FixedChars(const std::string& value, const std::string& param) const
        {
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

        bool CountryCode(const std::string& value, [[maybe_unused]] const std::string& param) const
        {
            return std::find(mCountryCodes.begin(), mCountryCodes.end(), value) != mCountryCodes.end();
        }

        bool IsPositiveNumber(const std::string& value, [[maybe_unused]] const std::string& param) const
        {
            bool retVal = IsNumber(value);
            if (retVal)
            {
                double amount = std::stod(value);
                retVal = (amount >= 0.0);
            }
            return retVal;
        }

        bool CurrencyCode(const std::string& value, [[maybe_unused]] const std::string& param) const
        {
            return std::find(mCurrencyCodes.begin(), mCurrencyCodes.end(), value) != mCurrencyCodes.end();
        }

        // ==========================================================================================
    public:
        bool ValidateParameter(const std::string& name, const std::string& value) const
        {
            bool retVal = true;
            const auto& paramIt = mValidators.find(name);
            if (paramIt != mValidators.end())
            {
                const ValidatorQueue& queue = paramIt->second;
                for (const ValidatorFunction& fn : queue)
                {
                    retVal = fn.first(value, fn.second);
                    if (!retVal)
                        break;
                }
            }
            else
            {
                retVal = false;
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
```


-------------------------------

Updated on 2021-12-26 at 18:31:40 +0100
