---
title: Classes

---

# Classes




* **namespace [MKQR](/Namespaces/namespace_m_k_q_r.md)** <br>This is the default [MKQR]() namespace.Use it to wrap anything that is related to [MKQR]()'s internal systems. 
    * **class [Generator](/Classes/class_m_k_q_r_1_1_generator.md)** <br>The [MKQR::Generator]() class contains functionality for generating and validating [MKQR](/Namespaces/namespace_m_k_q_r.md) codes. [MKQR::Generator]() implements all details of the [MKQR](/Namespaces/namespace_m_k_q_r.md) standard for generating such codes. 
    * **class [Validator](/Classes/class_m_k_q_r_1_1_validator.md)** <br>This class contains functions that validate parameters. Please don't make it static or singleton and keep all functions const, inlined by default ;) 
        * **struct [SResult](/Classes/struct_m_k_q_r_1_1_validator_1_1_s_result.md)** <br>The validator needs a reference to the [Generator](/Classes/class_m_k_q_r_1_1_generator.md), do that here The validator result, can be interpreted by the [Validator](/Classes/class_m_k_q_r_1_1_validator.md) easily. 
* **namespace [qrcodegen](/Namespaces/namespaceqrcodegen.md)** 
    * **class [BitBuffer](/Classes/classqrcodegen_1_1_bit_buffer.md)** 
    * **class [QrCode](/Classes/classqrcodegen_1_1_qr_code.md)** 
    * **class [QrSegment](/Classes/classqrcodegen_1_1_qr_segment.md)** 
        * **class [Mode](/Classes/classqrcodegen_1_1_qr_segment_1_1_mode.md)** 
    * **class [data_too_long](/Classes/classqrcodegen_1_1data__too__long.md)** 
* **namespace [std](/Namespaces/namespacestd.md)** 



-------------------------------

Updated on 2022-01-15 at 21:45:57 +0100
