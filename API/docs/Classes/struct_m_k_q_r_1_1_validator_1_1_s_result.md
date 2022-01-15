---
title: MKQR::Validator::SResult
summary: The validator needs a reference to the Generator, do that here The validator result, can be interpreted by the Validator easily. 

---

# MKQR::Validator::SResult



The validator needs a reference to the [Generator](/Classes/class_m_k_q_r_1_1_generator.md), do that here The validator result, can be interpreted by the [Validator](/Classes/class_m_k_q_r_1_1_validator.md) easily. 


`#include <mkqr_validator.h>`

## Public Types

|                | Name           |
| -------------- | -------------- |
| enum class| **[ELevel](/Classes/struct_m_k_q_r_1_1_validator_1_1_s_result.md#enum-elevel)** { Ok, Warning, Error} |

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[SResult](/Classes/struct_m_k_q_r_1_1_validator_1_1_s_result.md#function-sresult)**() |
| | **[SResult](/Classes/struct_m_k_q_r_1_1_validator_1_1_s_result.md#function-sresult)**([ELevel](/Classes/struct_m_k_q_r_1_1_validator_1_1_s_result.md#enum-elevel) level, const std::string & message) |
| [ELevel](/Classes/struct_m_k_q_r_1_1_validator_1_1_s_result.md#enum-elevel) | **[GetLevel](/Classes/struct_m_k_q_r_1_1_validator_1_1_s_result.md#function-getlevel)**() const |
| const std::string & | **[GetMessage](/Classes/struct_m_k_q_r_1_1_validator_1_1_s_result.md#function-getmessage)**() const |

## Public Types Documentation

### enum ELevel

| Enumerator | Value | Description |
| ---------- | ----- | ----------- |
| Ok | |   |
| Warning | |   |
| Error | |   |




**Parameters**: 

  * **Ok** All is good and validated 
  * **Warning** Some non-mandatory validations failed 
  * **Error** Validation failed on mandatory properties 


## Public Functions Documentation

### function SResult

```cpp
inline SResult()
```


### function SResult

```cpp
inline SResult(
    ELevel level,
    const std::string & message
)
```


### function GetLevel

```cpp
inline ELevel GetLevel() const
```


### function GetMessage

```cpp
inline const std::string & GetMessage() const
```


-------------------------------

Updated on 2022-01-15 at 21:45:57 +0100