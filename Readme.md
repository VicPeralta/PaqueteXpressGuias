# PaqueteXpressGuias

This an interface to Paquetexpress's REST services to generate a shipping label.

## Development
This program is written in C++ using Visual Studio 2019

It performs these operations:
* Open an JSON file with the information of the shipment, it must contain all the information required by the Server:
    * Billing
    * Consignee
    * Shipment Details
    * Shipper

* Connect to Paquetexpress to get a valid token
* Request a carta porte (airwaybill)
* Get label as PDF or ZPL as requeried
* Keep a record of request-carta porte generated


### Prerequisites
* [Visual Studio 2019](https://visualstudio.microsoft.com/es/) Community Edition
* [POCO](https://pocoproject.org/)  library if additional functionality is required

## Usage
```
PaqueteXpressGuias.exe [json] [ZPL] [output directory]
Example:
PaqueteXpressGuias.exe 2804722021.json PDF c:\WMS\out
```

## Sample captures
![alt text](captures/airwaybill.png?raw=true "Airwaybill")
![alt text](captures/call.png?raw=true "available")

## Authors
* **Victor Peralta** -(victor.peralta.gomez@gmail.com)
