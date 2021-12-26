---
title: MKQR::Validator
summary: This class contains functions that validate parameters. Please don't make it static or singleton, keep all functions const and refrain from using member variables. 

---

# MKQR::Validator



This class contains functions that validate parameters. Please don't make it static or singleton, keep all functions const and refrain from using member variables. 


`#include <mkqr_validator.h>`

## Public Functions

|                | Name           |
| -------------- | -------------- |
| bool | **[ValidateStringIBAN](/Classes/class_m_k_q_r_1_1_validator.md#function-validatestringiban)**(const std::string & ibanString) const<br>Validate IBAN.  |
| bool | **[ValidateStringAltIBAN](/Classes/class_m_k_q_r_1_1_validator.md#function-validatestringaltiban)**(const std::string & aibanString) const<br>Splits aibanString into components and then evaluates each component separately.  |

## Public Functions Documentation

### function ValidateStringIBAN

```cpp
inline bool ValidateStringIBAN(
    const std::string & ibanString
) const
```

Validate IBAN. 

**Parameters**: 

  * **ibanString** The IBAN string to validate


**Return**: True if valid, false if not valid 

### function ValidateStringAltIBAN

```cpp
inline bool ValidateStringAltIBAN(
    const std::string & aibanString
) const
```

Splits aibanString into components and then evaluates each component separately. 

**Parameters**: 

  * **aibanString** The alternative IBAN string to validate


**Return**: True if valid, false if not valid 

-------------------------------

Updated on 2021-12-26 at 15:42:24 +0100