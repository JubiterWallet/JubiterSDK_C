# Introduction
JuBiter iOS Demo is a demo for testing [JuBiter SDK](https://github.com/JubiterWallet/JubiterSDK_C.git).

The UI part of the Demo is in another project called '[JuBiterSDKDemoUI-iOS](https://github.com/JubiterWallet/JuBiterSDKDemoUI-iOS.git)'. Therefore, before using the Demo project, it is necessary to put its UI project under the same level directory of the Demo project for co-compilation, as described below for detailed steps.
* * *
# Supported Blockchains
Consistent with what is supported by the [JuBiter SDK](https://github.com/JubiterWallet/JubiterSDK_C.git), refer to the [JuBiter SDK](https://github.com/JubiterWallet/JubiterSDK_C.git) for details.

* * *
# Dependency & related projects
| Name | URL | Note |
| ---- | ---- | ---- |
| **JsonCpp** | https://github.com/open-source-parsers/jsoncpp.git | Source level integeration, based on master. Using for parsing test scripts in JSON. |
||||
| **JuBiter Demo UI** | https://github.com/JubiterWallet/JuBiterSDKDemoUI-iOS.git | JuBiter iOS Demo UI, using master branch. |
| **JuBiter SDK** | https://github.com/JubiterWallet/JubiterSDK_C.git | JuBiter SDK is what the demo tests. |

* * *
# Code branch management
* 'Master' and 'nonuse_pb' is the development line, continuously integrated development of new coins and SDK features.
* The branch named 'nonuse_pb_x.x' is used to test the JuBiter SDK directly.
* The branch named 'x.x' is to test calling the SDK via the PB data transformation layer.

The corresponding table of DEMO branch and JuBiter SDK branch is as follows:

| JuBiter SDK | iOS Demo | Note |
| ---- | ---- | ---- |
| master | nonuse_pb | The the development line. |
| 2.2 | nonuse_pb_2.2 | Release v2.2. |
| 2.3 | nonuse_pb_2.3 | Release v2.3. |

* * *
# Compiler Installation
Step1. Download the project's dependency libraries.
```bash
git submodule update --init --recursive
```
Step2. In the same directory, download the demo-related projects, including the '[JuBiterSDKDemoUI-iOS](https://github.com/JubiterWallet/JuBiterSDKDemoUI-iOS.git)' and '[JuBiter SDK](https://github.com/JubiterWallet/JubiterSDK_C.git)'.

> Note that the Demo branch must match the SDK branch. For example, if you need to test JuBiter SDK 2.3 branch, the SDK needs to be switched to 2.3 and the Demo needs to be switched to nonuse_pb_2.3.

Step3. Multi-project co-compilation. You can refer to the .xcworkspace file in the project directory. Some 'Build settings' may need to be modified according to your own situation, such as
 - 'Signing & Capabilities -> Signing'
 - 'Search Paths -> Headers Search Paths'
 - 'Search Paths -> Library Search Paths'
etc.
***
# Test case description
The test case uses configuration files in JSON, as detailed in the [json directory](testJSONScript).

Test case mindmap:
![Test case mindmap](../images/DemoTestCaseMindmap.png)
