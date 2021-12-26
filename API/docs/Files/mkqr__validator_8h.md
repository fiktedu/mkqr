---
title: mkqr_api/mkqr_validator.h

---

# mkqr_api/mkqr_validator.h



## Namespaces

| Name           |
| -------------- |
| **[MKQR](/Namespaces/namespace_m_k_q_r.md)** <br>This is the default [MKQR]() namespace.Use it to wrap anything that is related to [MKQR]()'s internal systems.  |

## Classes

|                | Name           |
| -------------- | -------------- |
| class | **[MKQR::Validator](/Classes/class_m_k_q_r_1_1_validator.md)** <br>This class contains functions that validate parameters. Please don't make it static or singleton, keep all functions const and refrain from using member variables.  |

## Defines

|                | Name           |
| -------------- | -------------- |
|  | **[MKQR_STR_DELIMITER](/Files/mkqr__validator_8h.md#define-mkqr-str-delimiter)**  |




## Macros Documentation

### define MKQR_STR_DELIMITER

```cpp
#define MKQR_STR_DELIMITER '|'
```


## Source code

```cpp
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
```


-------------------------------

Updated on 2021-12-26 at 15:42:24 +0100
