---
title: MKQR::Validator
summary: This class contains functions that validate parameters. Please don't make it static or singleton and keep all functions const, inlined by default ;) 

---

# MKQR::Validator



This class contains functions that validate parameters. Please don't make it static or singleton and keep all functions const, inlined by default ;) 


`#include <mkqr_validator.h>`

## Public Classes

|                | Name           |
| -------------- | -------------- |
| struct | **[SResult](/Classes/struct_m_k_q_r_1_1_validator_1_1_s_result.md)** <br>The validator needs a reference to the [Generator](/Classes/class_m_k_q_r_1_1_generator.md), do that here The validator result, can be interpreted by the [Validator](/Classes/class_m_k_q_r_1_1_validator.md) easily.  |

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[Validator](/Classes/class_m_k_q_r_1_1_validator.md#function-validator)**(const [Generator](/Classes/class_m_k_q_r_1_1_generator.md) & owner)<br>The validator needs a reference to the [Generator](/Classes/class_m_k_q_r_1_1_generator.md), do that here.  |
| std::vector< std::string > | **[GetMandatoryParameters](/Classes/class_m_k_q_r_1_1_validator.md#function-getmandatoryparameters)**() const<br>Gets all mandatory parameters at this point. This also includes optional parameters which have been evaluated as mandatory.  |
| [SResult](/Classes/struct_m_k_q_r_1_1_validator_1_1_s_result.md) | **[ValidateParameter](/Classes/class_m_k_q_r_1_1_validator.md#function-validateparameter)**(const std::string & name, const std::string & value) const<br>Gets all mandatory parameters at this point. This also includes optional parameters which have been evaluated as mandatory.  |

## Public Functions Documentation

### function Validator

```cpp
inline Validator(
    const Generator & owner
)
```

The validator needs a reference to the [Generator](/Classes/class_m_k_q_r_1_1_generator.md), do that here. 

**Parameters**: 

  * **owner** The owner generator 


### function GetMandatoryParameters

```cpp
std::vector< std::string > GetMandatoryParameters() const
```

Gets all mandatory parameters at this point. This also includes optional parameters which have been evaluated as mandatory. 

@reutrn A vector of all mandatory parameters at this point 


### function ValidateParameter

```cpp
SResult ValidateParameter(
    const std::string & name,
    const std::string & value
) const
```

Gets all mandatory parameters at this point. This also includes optional parameters which have been evaluated as mandatory. 

**Return**: A vector of all mandatory parameters at this point 

-------------------------------

Updated on 2022-01-15 at 21:45:57 +0100