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
| struct | **[MKQR::Validator::SResult](/Classes/struct_m_k_q_r_1_1_validator_1_1_s_result.md)** <br>The validator needs a reference to the [Generator](/Classes/class_m_k_q_r_1_1_generator.md), do that here The validator result, can be interpreted by the [Validator](/Classes/class_m_k_q_r_1_1_validator.md) easily.  |

## Defines

|                | Name           |
| -------------- | -------------- |
|  | **[MKQR_STR_DELIMITER](/Files/mkqr__validator_8h.md#define-mkqr-str-delimiter)** <br>This is the delimiter for combined strings.  |
|  | **[MKQR_VBIND](/Files/mkqr__validator_8h.md#define-mkqr-vbind)**(fnName, paramValue) <br>Binds a function pointer as a validator function.  |
|  | **[MKQR_VALIDATOR](/Files/mkqr__validator_8h.md#define-mkqr-validator)**(propertyName, ...) <br>Defines a validator which is a pair of property name and queue of functions that need to return true for the parameter to be validated.  |




## Macros Documentation

### define MKQR_STR_DELIMITER

```cpp
#define MKQR_STR_DELIMITER '|'
```

This is the delimiter for combined strings. 

### define MKQR_VBIND

```cpp
#define MKQR_VBIND(
    fnName,
    paramValue
)
{ std::bind(&MKQR::Validator::fnName, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), paramValue }
```

Binds a function pointer as a validator function. 

**Parameters**: 

  * **fnName** The name of the function to do the validation 
  * **paramValue** The value of the additional parameter. You can separate multiple parameters by using the pipe | delimiter 


### define MKQR_VALIDATOR

```cpp
#define MKQR_VALIDATOR(
    propertyName,
    ...
)
{propertyName,  { __VA_ARGS__ }}
```

Defines a validator which is a pair of property name and queue of functions that need to return true for the parameter to be validated. 

**Parameters**: 

  * **propertyName** The name of the property of this validator queue 
  * **...** One or multiple MKQR_BIND calls which make the validator queue 


example: MKQR_VALIDATOR("name", MKQR_VBIND(IsEqual, "name"), MKQR_VBIND(FixedLength, "4"))


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

#define MKQR_VBIND(fnName, paramValue) { std::bind(&MKQR::Validator::fnName, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), paramValue }

#define MKQR_VALIDATOR(propertyName, ...) {propertyName,  { __VA_ARGS__ }}

namespace MKQR
{
    class Generator;

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
            MKQR_VALIDATOR("v", MKQR_VBIND(FixedLength, "4"), MKQR_VBIND(Number, "1")),
            MKQR_VALIDATOR("c", MKQR_VBIND(FixedLength, "1"), MKQR_VBIND(FixedChars, "12")),
            MKQR_VALIDATOR("iban", MKQR_VBIND(IBAN, "")),
            MKQR_VALIDATOR("aiban", MKQR_VBIND(AltIBAN, "")),
            MKQR_VALIDATOR("cat", MKQR_VBIND(FixedLength, "1"), MKQR_VBIND(FixedChars, "SK")),
            MKQR_VALIDATOR("cn", MKQR_VBIND(NonZeroMaxLength, "70")),
            MKQR_VALIDATOR("cadd1", MKQR_VBIND(SwitchOnSK, "16|70|cat")),
            MKQR_VALIDATOR("cadd2", MKQR_VBIND(SwitchOnSK, "16|70|cat")),
            MKQR_VALIDATOR("cz", MKQR_VBIND(SwitchOnSK, "7|0|cat"), MKQR_VBIND(Number, "1")),
            MKQR_VALIDATOR("cg", MKQR_VBIND(SwitchOnSK, "35|0|cat")),
            MKQR_VALIDATOR("cc", MKQR_VBIND(FixedLength, "2"), MKQR_VBIND(CountryCode, "")),
            MKQR_VALIDATOR("a", MKQR_VBIND(DoublePositiveNumber, ""), MKQR_VBIND(NonZeroMaxLength, "1079")), // 64-bit IEEE double
            MKQR_VALIDATOR("cur", MKQR_VBIND(FixedLength, "3"), MKQR_VBIND(CurrencyCode, "")),
            MKQR_VALIDATOR("pat", MKQR_VBIND(FixedLength, "1"), MKQR_VBIND(FixedChars, "SK")),
            MKQR_VALIDATOR("pn", MKQR_VBIND(NonZeroMaxLength, "70")),
            MKQR_VALIDATOR("padd1", MKQR_VBIND(SwitchOnSK, "16|70|pat")),
            MKQR_VALIDATOR("padd2", MKQR_VBIND(SwitchOnSK, "16|70|pat")),
            MKQR_VALIDATOR("pz", MKQR_VBIND(SwitchOnSK, "7|0|pat"),  MKQR_VBIND(Number, "1")),
            MKQR_VALIDATOR("pg", MKQR_VBIND(SwitchOnSK, "35|0|pat")),
            MKQR_VALIDATOR("pc", MKQR_VBIND(FixedLength, "2"), MKQR_VBIND(CountryCode, "")),
            MKQR_VALIDATOR("rt", MKQR_VBIND(FixedStr, "QRR|SCOR|NON")),
            MKQR_VALIDATOR("ref", MKQR_VBIND(NonZeroMaxLength, "27")),
            MKQR_VALIDATOR("pcd", MKQR_VBIND(FixedLength, "3"), MKQR_VBIND(Number, "1")),
            MKQR_VALIDATOR("nac", MKQR_VBIND(FixedLength, "1"), MKQR_VBIND(Number, "1")),
            MKQR_VALIDATOR("us50", MKQR_VBIND(FixedLength, "15"), MKQR_VBIND(Number, "1")),
            MKQR_VALIDATOR("usek50", MKQR_VBIND(FixedLength, "15"), MKQR_VBIND(Number, "1")),
            MKQR_VALIDATOR("us30", MKQR_VBIND(FixedLength, "15"), MKQR_VBIND(Number, "1")),
            MKQR_VALIDATOR("usek30", MKQR_VBIND(FixedLength, "15"), MKQR_VBIND(Number, "1")),
            MKQR_VALIDATOR("i", MKQR_VBIND(NonZeroMaxLength, "140")),
            MKQR_VALIDATOR("curl", MKQR_VBIND(URL, "")),
            MKQR_VALIDATOR("ap", MKQR_VBIND(NonZeroMaxLength, "20")),
            MKQR_VALIDATOR("av", MKQR_VBIND(DoublePositiveNumber, ""), MKQR_VBIND(NonZeroMaxLength, "1079")),
            MKQR_VALIDATOR("ad", MKQR_VBIND(NonZeroMaxLength, "240")),
            MKQR_VALIDATOR("ac", MKQR_VBIND(FixedLength, "3"), MKQR_VBIND(CurrencyCode, ""))
        };

        const std::vector<std::string> mMandatoryParameters =
        {
            "t", "v", "c", "iban", "cat", "cn", "cc", "cur", "rt", "pcd" // TODO: add rt when validated
        };

        const std::vector<std::string> mConditionalParameters =
        {
            "cadd1", "cz", "cg", "padd1", "pz", "pg"
        };

        [[nodiscard]] std::vector<std::string> TokenizeString(const char* str, char delimiter) const;

        [[nodiscard]] bool IsNumber(const std::string& str, bool isAnyNumber = 0) const;

        [[nodiscard]] bool IsParameterMandatory(const std::string& name) const;

        // ==========================================================================================

        [[nodiscard]] bool IBAN(const std::string& ibanString,
            [[maybe_unused]] const std::string& param,
            std::string& outMessage) const;

        [[nodiscard]] bool AltIBAN(const std::string& aibanString,
            [[maybe_unused]] const std::string& param,
            std::string& outMessage) const;

        [[nodiscard]] bool IsEqual(const std::string& value,
            const std::string& param,
            std::string& outMessage) const;

        [[nodiscard]] bool NonZeroMaxLength(const std::string& value,
            const std::string& param,
            std::string& outMessage) const;

        [[nodiscard]] bool FixedLength(const std::string& value,
            const std::string& param,
            std::string& outMessage) const;

        [[nodiscard]] bool FixedChars(const std::string& value,
            const std::string& param,
            std::string& outMessage) const;

        [[nodiscard]] bool FixedStr(const std::string& value,
            const std::string& param,
            std::string& outMessage) const;

        [[nodiscard]] bool CountryCode(const std::string& value,
            [[maybe_unused]] const std::string& param,
            std::string& outMessage) const;

        [[nodiscard]] bool DoublePositiveNumber(const std::string& value,
            [[maybe_unused]] const std::string& param,
            std::string& outMessage) const;

        [[nodiscard]] bool CurrencyCode(const std::string& value,
            [[maybe_unused]] const std::string& param,
            std::string& outMessage) const;

        [[nodiscard]] bool SwitchOnSK(const std::string& value,
            const std::string& param,
            std::string& outMessage) const;

        [[nodiscard]] bool URL(const std::string& value,
            const std::string& param,
            std::string& outMessage);

        [[nodiscard]] bool Number(const std::string& value,
            const std::string& param,
            std::string& outMessage);

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

            ELevel GetLevel() const { return mLevel; }
            const std::string& GetMessage() const { return mMessage; }
        };

        [[nodiscard]] std::vector<std::string> GetMandatoryParameters() const;

        [[nodiscard]] SResult ValidateParameter(
            const std::string& name, const std::string& value) const;

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

Updated on 2022-01-15 at 21:45:57 +0100
