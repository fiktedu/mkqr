---
title: qrcodegen::QrCode

---

# qrcodegen::QrCode





## Public Types

|                | Name           |
| -------------- | -------------- |
| enum class| **[Ecc](/Classes/classqrcodegen_1_1_qr_code.md#enum-ecc)** { LOW = 0, MEDIUM, QUARTILE, HIGH} |

## Public Functions

|                | Name           |
| -------------- | -------------- |
| QrCode | **[encodeText](/Classes/classqrcodegen_1_1_qr_code.md#function-encodetext)**(const char * text, Ecc ecl) |
| QrCode | **[encodeBinary](/Classes/classqrcodegen_1_1_qr_code.md#function-encodebinary)**(const std::vector< std::uint8_t > & data, Ecc ecl) |
| QrCode | **[encodeSegments](/Classes/classqrcodegen_1_1_qr_code.md#function-encodesegments)**(const std::vector< QrSegment > & segs, Ecc ecl, int minVersion =1, int maxVersion =40, int mask =-1, bool boostEcl =true) |
| | **[QrCode](/Classes/classqrcodegen_1_1_qr_code.md#function-qrcode)**(int ver, Ecc ecl, const std::vector< std::uint8_t > & dataCodewords, int msk) |
| int | **[getVersion](/Classes/classqrcodegen_1_1_qr_code.md#function-getversion)**() const |
| int | **[getSize](/Classes/classqrcodegen_1_1_qr_code.md#function-getsize)**() const |
| Ecc | **[getErrorCorrectionLevel](/Classes/classqrcodegen_1_1_qr_code.md#function-geterrorcorrectionlevel)**() const |
| int | **[getMask](/Classes/classqrcodegen_1_1_qr_code.md#function-getmask)**() const |
| bool | **[getModule](/Classes/classqrcodegen_1_1_qr_code.md#function-getmodule)**(int x, int y) const |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| constexpr int | **[MIN_VERSION](/Classes/classqrcodegen_1_1_qr_code.md#variable-min-version)**  |
| constexpr int | **[MAX_VERSION](/Classes/classqrcodegen_1_1_qr_code.md#variable-max-version)**  |

## Public Types Documentation

### enum Ecc

| Enumerator | Value | Description |
| ---------- | ----- | ----------- |
| LOW | 0|   |
| MEDIUM | |   |
| QUARTILE | |   |
| HIGH | |   |




## Public Functions Documentation

### function encodeText

```cpp
static QrCode encodeText(
    const char * text,
    Ecc ecl
)
```


### function encodeBinary

```cpp
static QrCode encodeBinary(
    const std::vector< std::uint8_t > & data,
    Ecc ecl
)
```


### function encodeSegments

```cpp
static QrCode encodeSegments(
    const std::vector< QrSegment > & segs,
    Ecc ecl,
    int minVersion =1,
    int maxVersion =40,
    int mask =-1,
    bool boostEcl =true
)
```


### function QrCode

```cpp
QrCode(
    int ver,
    Ecc ecl,
    const std::vector< std::uint8_t > & dataCodewords,
    int msk
)
```


### function getVersion

```cpp
int getVersion() const
```


### function getSize

```cpp
int getSize() const
```


### function getErrorCorrectionLevel

```cpp
Ecc getErrorCorrectionLevel() const
```


### function getMask

```cpp
int getMask() const
```


### function getModule

```cpp
bool getModule(
    int x,
    int y
) const
```


## Public Attributes Documentation

### variable MIN_VERSION

```cpp
static constexpr int MIN_VERSION = 1;
```


### variable MAX_VERSION

```cpp
static constexpr int MAX_VERSION = 40;
```


-------------------------------

Updated on 2021-12-26 at 15:42:24 +0100