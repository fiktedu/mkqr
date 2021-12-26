---
title: qrcodegen::QrSegment

---

# qrcodegen::QrSegment





## Public Classes

|                | Name           |
| -------------- | -------------- |
| class | **[Mode](/Classes/classqrcodegen_1_1_qr_segment_1_1_mode.md)**  |

## Public Functions

|                | Name           |
| -------------- | -------------- |
| QrSegment | **[makeBytes](/Classes/classqrcodegen_1_1_qr_segment.md#function-makebytes)**(const std::vector< std::uint8_t > & data) |
| QrSegment | **[makeNumeric](/Classes/classqrcodegen_1_1_qr_segment.md#function-makenumeric)**(const char * digits) |
| QrSegment | **[makeAlphanumeric](/Classes/classqrcodegen_1_1_qr_segment.md#function-makealphanumeric)**(const char * text) |
| std::vector< QrSegment > | **[makeSegments](/Classes/classqrcodegen_1_1_qr_segment.md#function-makesegments)**(const char * text) |
| QrSegment | **[makeEci](/Classes/classqrcodegen_1_1_qr_segment.md#function-makeeci)**(long assignVal) |
| bool | **[isNumeric](/Classes/classqrcodegen_1_1_qr_segment.md#function-isnumeric)**(const char * text) |
| bool | **[isAlphanumeric](/Classes/classqrcodegen_1_1_qr_segment.md#function-isalphanumeric)**(const char * text) |
| int | **[getTotalBits](/Classes/classqrcodegen_1_1_qr_segment.md#function-gettotalbits)**(const std::vector< QrSegment > & segs, int version) |
| | **[QrSegment](/Classes/classqrcodegen_1_1_qr_segment.md#function-qrsegment)**(const Mode & md, int numCh, const std::vector< bool > & dt) |
| | **[QrSegment](/Classes/classqrcodegen_1_1_qr_segment.md#function-qrsegment)**(const Mode & md, int numCh, std::vector< bool > && dt) |
| const Mode & | **[getMode](/Classes/classqrcodegen_1_1_qr_segment.md#function-getmode)**() const |
| int | **[getNumChars](/Classes/classqrcodegen_1_1_qr_segment.md#function-getnumchars)**() const |
| const std::vector< bool > & | **[getData](/Classes/classqrcodegen_1_1_qr_segment.md#function-getdata)**() const |

## Public Functions Documentation

### function makeBytes

```cpp
static QrSegment makeBytes(
    const std::vector< std::uint8_t > & data
)
```


### function makeNumeric

```cpp
static QrSegment makeNumeric(
    const char * digits
)
```


### function makeAlphanumeric

```cpp
static QrSegment makeAlphanumeric(
    const char * text
)
```


### function makeSegments

```cpp
static std::vector< QrSegment > makeSegments(
    const char * text
)
```


### function makeEci

```cpp
static QrSegment makeEci(
    long assignVal
)
```


### function isNumeric

```cpp
static bool isNumeric(
    const char * text
)
```


### function isAlphanumeric

```cpp
static bool isAlphanumeric(
    const char * text
)
```


### function getTotalBits

```cpp
static int getTotalBits(
    const std::vector< QrSegment > & segs,
    int version
)
```


### function QrSegment

```cpp
QrSegment(
    const Mode & md,
    int numCh,
    const std::vector< bool > & dt
)
```


### function QrSegment

```cpp
QrSegment(
    const Mode & md,
    int numCh,
    std::vector< bool > && dt
)
```


### function getMode

```cpp
const Mode & getMode() const
```


### function getNumChars

```cpp
int getNumChars() const
```


### function getData

```cpp
const std::vector< bool > & getData() const
```


-------------------------------

Updated on 2021-12-26 at 15:42:24 +0100