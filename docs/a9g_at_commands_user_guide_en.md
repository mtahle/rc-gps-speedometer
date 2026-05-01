# A9G AT Commands User Guide V1.1

## Overview
This document describes the meaning, syntax, and response content of the AT commands used by the Ai-Thinker GPRS series modules.

## Disclaimer and Copyright Notice
The information in this document, including reference URLs, is subject to change without notice.
The document is provided "as is" without any warranty of any kind, including any warranty of merchantability, fitness for a particular purpose, or non-infringement, and any warranty mentioned elsewhere in proposals, specifications, or samples. This document shall not be held liable for any infringement of any patent rights arising from the use of the information in this document. This document does not grant any license, whether express or implied, by estoppel or otherwise, to any intellectual property rights.
All trademarks, brand names, and registered trademarks mentioned in this document are the property of their respective owners and are hereby acknowledged.

## Note
Due to product version upgrades or other reasons, the contents of this manual may change. Shenzhen Ai-Thinker Technology Co., Ltd. reserves the right to modify the contents of this manual without any notice or prompt. This manual is intended as a guide only. Shenzhen Ai-Thinker Technology Co., Ltd. makes every effort to provide accurate information in this manual, but does not guarantee that the contents are completely free of errors. All statements, information, and suggestions in this manual do not constitute any express or implied warranty.

## Revision History
| Date | Version | Author | Description |
| :--- | :--- | :--- | :--- |
| 2017.12.30 | V1.0 | Ai-Thinker | Initial version |
| 2018.09.27 | V1.1 | xing | 1. Modified the sending packet length of transparent transmission parameter configuration to 10~65535<br>2. Set the default mode of CREG and modified the response format of AT+CREGD<br>3. Added AT examples for configuring heartbeat packets<br>4. Modified the reference material for AT+SLEEP<br>5. Modified the reference material for AT+GPSLP<br>6. Added GPRS and GPS LED status table<br>7. Modified the reference material for machine cloud<br>8. Added an MQTTPUB execution command |

---
**Page 1**

## Table of Contents
1. **Overview** ................................................................................................................ 8
    1.1 AT Command Abbreviations and Conventions ........................................................ 8
    1.2 AT Command Syntax .............................................................................................. 8
        1.2.1 AT Command Syntax ................................................................................... 8
2. **General Commands** ............................................................................................... 10
    2.1 AT Test Command ............................................................................................... 10
    2.2 AT&W Save User Configuration Parameters ........................................................ 10
    2.3 AT&F Set All TA Parameters to Factory Configuration ......................................... 10
    2.4 ATZ Reset to Default Configuration ..................................................................... 11
    2.5 ATI Get Manufacturer Information ....................................................................... 12
    2.6 AT+CGMI Request Manufacturer ID Information ............................................... 12
    2.7 AT+CGMM Get Product Model ........................................................................... 13
    2.8 AT+CGMR Get Software Version Information .................................................... 13
    2.9 AT+CGSN Get Device IMEI ................................................................................. 14
    2.10 AT+EGMR Read/Write IMEI ............................................................................. 14
    2.11 ATE Command Echo Mode ............................................................................... 15
    2.12 ATQ Set Result Code Display Mode .................................................................. 16
    2.13 ATV Set TA Response Content Format .............................................................. 17
    2.14 ATS3 Set Command Line Termination Character .............................................. 18
    2.15 ATS4 Set Response Formatting Character ......................................................... 18
    2.16 ATS5 Set Command Line Editing Character ....................................................... 19
    2.17 ATX Set CONNECT Result Code Format and Monitor Call Process .................. 19
    2.18 AT+IPR Set Serial Communication Baud Rate ................................................... 20
    2.19 AT+CSCS Select TE Character Set ..................................................................... 22
    2.20 AT+CIMI Get Device IMSI ................................................................................. 23
    2.21 AT+LPOFF Power Off Option Settings ............................................................... 24
3. **Status Control Commands** ..................................................................................... 25
    3.1 AT+CPAS Module Activity Status ........................................................................ 25
    3.2 AT+CFUN Set Module Functionality ................................................................... 26
    3.3 AT+CBC Query Battery Charge and Charging Status ........................................... 26
    3.4 AT+CCLK Get Real-Time Time ............................................................................ 27
    3.5 AT+RST Close or Restart Mobile Device ............................................................. 29
    3.6 AT+SLEEP Module Enters Low Power Mode ....................................................... 30
4. **SIM Card Related Commands** ............................................................................... 32
    4.1 CPOL Preferred Operator List ............................................................................... 32
    4.2 AT+CCID Query CCID .......................................................................................... 32
5. **Call Control Commands** ........................................................................................ 34
    5.1 ATD Initiate Call ................................................................................................... 34
    5.2 ATA Answer Call .................................................................................................. 35
    5.3 ATH Terminate Current Call ................................................................................. 36
    5.4 ATS0 Auto Answer ................................................................................................ 37
    5.5 ATO Switch from Command Mode to Data Mode ............................................... 38
    5.6 +++ Switch from Data Mode to Command Mode ............................................... 38
    5.7 AT+CLIP Display Caller ID ................................................................................... 39
    5.8 AT+COLP Display Connected Line ID ................................................................. 40
    5.9 AT+CLIR Caller ID Restriction .............................................................................. 41
    5.10 AT+CCFC Set Call Forwarding ........................................................................... 43
    5.11 AT+CCWA Call Waiting ...................................................................................... 45
    5.12 AT+DLST Redial Last Call ................................................................................. 47
    5.13 AT+CLCC Query Current Call Status .................................................................. 49
    5.14 AT+CHLD Call Hold and Multiparty Call ........................................................... 51
    5.15 AT+CRSL Incoming Call Volume Level ............................................................. 52
    5.16 AT+CMUT Mute Control ..................................................................................... 54
    5.17 AT+CUSD Unstructured Supplementary Service Data ....................................... 55
    5.18 AT+CSSN Supplementary Service Notifications ................................................. 56
    5.19 AT+CDTMF Play DTMF Tones ........................................................................... 57
    5.20 AT+VTS Send DTMF Tones ............................................................................... 58
    5.21 AT+VTD Set DTMF Tone Duration .................................................................... 59
    5.22 AT+CRC Enable or Disable Extended Result Codes .......................................... 60
    5.23 AT+SNFS Switch Audio Channel ....................................................................... 62
    5.24 AT+AUST Start Audio Loopback Test ................................................................ 63
6. **SMS Related Commands** ....................................................................................... 65
    6.1 AT+CMGF Set SMS Message Format ................................................................... 65
    6.2 AT+CPMS Select SMS Message Storage .............................................................. 66
    6.3 AT+CSCA SMS Service Center Address ............................................................... 67
    6.4 AT+CSMP Set Text Mode Parameters ................................................................... 68
    6.5 AT+CNMI New SMS Indication ............................................................................ 69
    6.6 AT+CMGL List SMS Messages ............................................................................ 72
    6.7 AT+CMGR Read SMS Message ............................................................................ 76
    6.8 AT+CMGS Send SMS Message ............................................................................. 80
    6.9 AT+CMGW Save SMS Message to Storage .......................................................... 82
    6.10 AT+CMSS Send SMS Message from Storage .................................................... 83
    6.11 AT+CMGD Delete SMS Message ....................................................................... 84
7. **Phonebook Related Commands** ............................................................................. 86
    7.1 AT+CPBS Select Phonebook Memory Storage Unit ............................................ 86
    7.2 AT+CPBW Write/Delete Phonebook Entry .......................................................... 87
    7.3 AT+CPBR Query Phonebook Entry ...................................................................... 88
    7.4 AT+CPBF Find Phonebook Entry ......................................................................... 89
    7.5 AT+CNUM Get MSISDN Associated with User ................................................... 91
8. **GPRS Related Commands** ..................................................................................... 93
    8.1 AT+CREG Get Network Registration Status ........................................................ 93
    8.2 AT+COPS Select Operator .................................................................................... 94
    8.3 AT+COPN Query Operator Names ....................................................................... 96
    8.4 AT+CSQ Get Signal Quality .................................................................................. 96
    8.5 AT+CGDCONT Define PDP Context ................................................................... 97
    8.6 AT+CGATT GPRS Attach and Detach .................................................................. 99
    8.7 AT+CGACT PDP Context Activation and Deactivation ....................................... 100
    8.8 AT+CGPADDR Show PDP Address ..................................................................... 101
    8.9 AT+CGCLASS GPRS Mobile Station Class .......................................................... 102
    8.10 AT+ECSQ Control Whether to Proactively Report Signal Quality ................... 103
9. **GPS Related Commands** ...................................................................................... 105
    9.1 AT+GPS Enable or Disable GPS .......................................................................... 105
    9.2 AT+AGPS Enable or Disable AGPS ..................................................................... 105
    9.3 AT+GPSRD Set NMEA Data Output Interval ....................................................... 107
    9.4 AT+GPSMD GPS Mode ........................................................................................ 108
    9.5 AT+GPSLP GPS Chip Low Power Mode ............................................................. 109
    9.6 AT+LOCATION Positioning Function ................................................................. 110
    9.7 AT+GPNT Control GPRS Indicator Light Status .................................................. 112
10. **TCP/IP Related Commands** ................................................................................ 115
    10.1 AT+CIPSTART Start TCP/UDP Connection ........................................................ 115
    10.2 AT+CIPMUX Enable/Disable Multiple Connection Mode ................................ 116
    10.3 AT+CIPSEND Send Data via TCP/UDP ............................................................... 117
    10.4 AT+CIPCLOSE Close Connection ...................................................................... 118
    10.5 AT+CIPSTATUS Query Network Connection Status ......................................... 119
    10.6 AT+CIFSR Get Local IP ...................................................................................... 120
    10.7 AT+CDNSGIP DNS Domain Name Resolution .................................................. 121
    10.8 AT+CIPHCFG Set Heartbeat Packet Parameters ............................................... 122
    10.9 AT+CIPHMODE Start Heartbeat Packet ............................................................. 123
    10.10 AT+CIPTCFG Set Transparent Transmission Parameters ............................... 124
    10.11 AT+CIPTMODE Start Transparent Transmission Mode ................................... 126
    10.12 AT+HTTPGET Network Connection ................................................................ 127
    10.13 AT+HTTPOST Request to Server ..................................................................... 129
11. **Ai-Thinker Cloud Related Commands (A6 Cannot Use)** ................................... 131
    11.1 AT+GIZSTART Connect to Ai-Thinker Cloud .................................................... 131
    11.2 AT+GIZSTOP Disconnect from Ai-Thinker Cloud ............................................. 132
    11.3 AT+GIZSEND Send Data to Ai-Thinker Cloud Server ....................................... 133
    11.4 AT+GIZQRCODE ............................................................................................... 134
    11.5 AT+GIZTRACKER Tracker Function ................................................................ 135
12. **MQTT Related Commands** ................................................................................. 137
    12.1 AT+MQTTCONN Send Connection Packet ........................................................ 137
    12.2 AT+MQTTSUB Send Subscription Packet ........................................................ 138
    12.3 AT+MQTTPUB Send Message Transmission Packet ........................................ 139
    12.4 AT+MQTTDISCONN Send Disconnection Packet ............................................. 141
**Appendix** ....................................................................................................................... 143
    1. CME ERRORS ......................................................................................................... 143
    2. CMS ERRORS ......................................................................................................... 146
    Examples .................................................................................................................... 149
    Get Latest Documentation ......................................................................................... 149
    Problem Feedback ...................................................................................................... 149

