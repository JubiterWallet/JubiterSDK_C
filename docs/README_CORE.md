# Core module
In [JUB_CORE.h](../include/JUB_CORE.h).

Function list:
* [JUB_GenerateMnemonic](#JUB_GenerateMnemonic)
* [JUB_CheckMnemonic](#JUB_CheckMnemonic)

### JUB_GenerateMnemonic
* **Function:** Generate mnemonic for software wallet.
* **IN param:**
strength - mnemonic strength:
    JUB_ENUM_MNEMONIC_STRENGTH::STRENGTH128(12 words),
    JUB_ENUM_MNEMONIC_STRENGTH::STRENGTH192(18 words),
    JUB_ENUM_MNEMONIC_STRENGTH::STRENGTH256(24 words)
* **OUT param:**
mnemonic - generated mnemonic.
* **Return:** JUB_OK or !JUB_OK for error.
```
JUB_RV JUB_GenerateMnemonic(
    IN JUB_ENUM_MNEMONIC_STRENGTH strength,
    OUT JUB_CHAR_PTR_PTR mnemonic
);
```

### JUB_CheckMnemonic
* **Function:** Check if the mnemonic is valid.
* **IN param:**
mnemonic - mnemonic.
* **OUT param:** none.
* **Return:** JUB_OK or !JUB_OK for error.
```
JUB_RV JUB_CheckMnemonic(
    IN JUB_CHAR_CPTR mnemonic
);
```
