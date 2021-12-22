# MKQR предлог стандард v1.0.0



## Вовед

**MKQR** претставува Македонски **отворен** и **независен** предлог стандард за лесно енкодирање и пренос на информации за плаќање и операции поврзани со финансии како на пример: донации, евиденција на потрошувачка и сл. 

Податоците дефинирани во стандардот се енкодираат во форма на специјално составен стринг лесен за читање од мобилни апликации, а е претставен во форма на стандарден QR (quick response) код.



### Предлагачи на стандардот:

Предлагач на стандардот е Факултетот за Информатички и Комуникациски Технологии ([ФИКТ](http://fikt.uklo.edu.mk)) - Битола, во рамките на Универзитетот Св. Климент Охридски - Битола.

<img src="https://fikt.uklo.edu.mk/wp-content/uploads/sites/2/2021/12/fikt1.png" style="zoom:20%;" />

Во план е да се формира непрофитен конзорциум на поддржувачи и имплементатори на стандардот кој ќе го води и обликува според потребите на сите вклучени страни.

 

### Верзионирање

Стандардот ќе се верзионира семантички. Онсовната верзија е верзија 1.0.0





## Приказ на предлог стандардот v.1.0.0

Полињата на атрибутите, нивното значење, можни вредности и сл, се дадени во следната табела:

| Име на атрибутот                  | Кратенка | Општа дефиниција                                             |                                                              | Забелешка                                                    | тип на податок |
| --------------------------------- | -------- | ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ | -------------- |
| QRType                            | t        | тип на стандардот. Фиксен податок **"MKD"**,                 | Фиксна должина, 3 алфанумерички карактери                    | "MKD" - фиксна вредност                                      | Задолжителен   |
| Version                           | v        | верзија на спецификацијата според која овој код е генериран. Засега верзијата е 1.0.0, и ќе се користат само првите два елементи без точката со водечка нула до максимум две цифри. | Фиксна должина, 4 бројки                                     | За верзија 1.0.0, v=0100; За верзија 1.2.4, v=0102           | Задолжителен   |
| Coding                            | c        | тип на кодна страна на податоците во стрингот                | фиксна должина, 1 бројка                                     | фиксна вредност 1 за UTF-8 енкодирање но само со подмножество на латинични карактери, фиксна вредност 2 за UTF-8 енкодирање со сите карактери вклучително и кирилица | Задолжителен   |
| IBAN                              | iban     | IBAN број на сметката на кредиторот (каде треба да се префрлат средствата) | фиксна должина, 15 алфанумерички карактери                   |                                                              | Задолжителен   |
| Alternative IBAN                  | aiban    | Алтернативни IBAN сметки на кредиторот одделени со pipe (\|) карактерот. | Максимум 77 карактери                                        | Алтернативните сметки се подредени по преференца на кредиторот | Опционен       |
| Address Type                      | cat      | Тип на адресата на кредиторот. "S" - структурирана адреса во повеќе полиња, "K" - комбинирана адреса во две полиња (линии) | фиксна должина, 1 алфанумерички карактер                     |                                                              | Задолжителен   |
| Creditor Name                     | cn       | Име на кредиторот или име на компанијата кон која ќе се изврши плаќањето | Максимум 70 карактери                                        |                                                              | Задолжителен   |
| Street or address line 1          | cadd1    | за структурирана адреса (S) се става името на улицата или поштенскиот фах на кредиторот. За Комбинирана адреса (К) се става првата линија од адресата, улица и број или поштенски фах. | (S) - максимум 16 карактери, (К) - максимум 70 карактери     | Ако cat e "K"(комбинирана адреса), тогаш овој податок мора да постои | Опционен       |
| Building number or address line 2 | cadd2    | за структурирана адреса (S) се става бројот на адресата на кредиторот. За Комбинирана адреса (К) се става втората линија од адресата: градот и поштенскиот број на кредиторот. | (S) - максимум 16 карактери, (К) - максимум 70 карактери     |                                                              | Опционен       |
| Postal Code                       | cz       | за структурирана адреса (S) се става поштенскиот број        | (S) - максимум 7 карактери, (К) - не се става                | Ако cat e "S"(структурирана адреса), тогаш овој податок мора да постои | Опционен       |
| Town Name                         | cg       | Името на градот на кредиторот                                | (S) - максимум 35 карактери, (К) - не се става               | Ако cat e "S"(структурирана адреса), тогаш овој податок мора да постои | Опционен       |
| Country                           | cc       | Држава на кредиторот                                         | фиксна должина, 2 алфанумерички карактери                    | Кратенките се според ISO 3166-1                              | Задолжителен   |
| Amount                            | а        | сумата за плаќање                                            | децимален број со две децимални места, максимум 12 карактери заедно со децималната точка | Сумата треба да е внесена без водечки нули, со децимален сепаратор и две децимални места. Максимум 12 бројки заедно со децималниот сепаратор. За децимален сепаратор се користи исклучиво точка (.) | Опционен       |
| Currency                          | cur      | Валута                                                       | фиксна должина, 3 алфанумерички карактери                    | Валута на плаќањето. Според ISO 4217, дозволени вредности се само "MKD" и "EUR" | Задолжителен   |
| Ultimate Debtor address type      | pat      | Типот на адреса на лицето за кое се врши плаќањето (ако не е наменето за истиот плаќач) | фиксна должина, 1 алфанумерички карактери                    | Address type The address type is specified using a code. The following codes are defined: "S" - structured address "K" - combined address elements (address in 2 lines) | Опционен       |
| Ultimate Debtor Name              | pn       | Име и презиме на лицето за кое се врши плаќањето (ако не е наменето за истиот плаќач) | максимум 70 карактери . Пример: Име и презиме или  Име на компанијата |                                                              | Опционен       |
| Street Name or Address Line 1     | paddr1   | Street or address line 1 Structured Address: Street/P.O. Box from ultimate debtor’s address Combined address elements: Address line 1 including street and building number or P.O. Box | максимум 70 карактери .                                      |                                                              | Опционен       |
| Building Number or Address Line 2 | paddr2   | Building number or address line 2 Structured Address: Building number from ultimate debtor’s address Combined address elements: Address line 2 including postal code and town from ultimate debtor’s address | Structured Address: max. 16 characters allowed Combined address elements: maximum 70 characters permitted Must be provided for address type "K". |                                                              | Опционен       |
| Postal Code                       | pz       | Postal code from ultimate debtor’s address                   | Maximum 16 characters permitted The postal code is must be provided without a country code prefix. Combined address elements: must not be provided |                                                              | Опционен       |
| Town Name                         | pn       | Town Town from ultimate debtor’s address                     | Maximum 35 characters permitted Combined address elements: must not be provided |                                                              |                |
| Country                           | pc       | Country Country from ultimate debtor’s address               | Two-digit country code according to ISO 3166-1               |                                                              |                |
| Payment Reference Type            | rt       | Reference type, QR, none                                     |                                                              |                                                              | Задолжителен   |
| Payment Reference                 | Ref      | Payment reference number issued by the issuer of the bill    | Maximum 27 characters, alphanumeric                          |                                                              | Опционен       |
| Payment Code                      | pc       | Payment Code for Macedonian Banks reference                  | Fixed length: three-digit, numeric                           |                                                              | Задолжителен   |
| Payment Type                      | nac      | Payment Type (начин) for Macedonian Banks reference          | Fixed length: one-digit, numeric                             |                                                              | Опционен       |
| PP50 Uplatna smetka               | us       |                                                              |                                                              |                                                              | Опционен       |
| PP50 Smetka Edinka Korisnik       | usek     |                                                              |                                                              |                                                              |                |
| PP50 Prihodna sifra               | ps       |                                                              | Fixed length: two-digit, alphanumeric                        | V1                                                           |                |
| PP50 Programa                     | pr       |                                                              |                                                              |                                                              |                |
| Additional info, USTRD            | i        |                                                              |                                                              |                                                              |                |
| CheckURL                          | curl     |                                                              |                                                              |                                                              |                |
| Alternative Payment Scheme        | ap       |                                                              |                                                              |                                                              |                |
| Alternative Payment Values        | av       |                                                              |                                                              |                                                              |                |
| Alternative Payment Description   | ad       |                                                              |                                                              |                                                              |                |
| Alternative Payment Currency      | ac       |                                                              |                                                              |                                                              |                |






