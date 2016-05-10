# IoT-QtClient

IoT-Ticket Qt client provides an easy-to-use library and examples so that Your application can take advantage of the versatile IoT-Ticket cloud tools.

## System requirements

This Qt library can be used on all platforms Qt supports. Library uses Qt's JSon libraries so minimum compliant Qt version is 5.0. Library uses also c++11 features.

## Getting started
1. Create your own IoT-Ticket account at https://www.iot-ticket.com/ (Request an invitation)
2. Wait for the account activation email
3. Build the library
4. Start using it in your application

### Building the library

Library is a standard qmake project. Debug builds also build example application and unitests. Unitests support QtCreator's Auto Test plugin.
Compiler needs to support c++11 features.


### Example code

The library contains a demo which provides an example application. Html documentation in the doc folder contains also examples how library API's can be used.

### Providing authentication credentials for the IotTicket server connection

```cpp
iot::Connection* connection = iot::Connection::instance();
connection->setUserName("userName");
connection->setPassword("password");
// Connection is now configured and request can be made.
```

### Registering a device

```cpp
    iot::Device* device = new iot::Device();
    device->setManufacturer("Device manufacturer");
    device->setDescription("Device description");
    device->setName("Device name");
    device->setType("Device type");
    QVariantMap attributes;
    attributes["id"] = 123;
    device->setAttributes(attributes);
    connect(device, &iot::Device::registerFinished, [=](bool success) {
        if (success) {
            qDebug() << "done";
            QString deviceId = device->deviceId();
            // device id needed in future operations
            device->deleteLater();
        } else {
            qDebug() << "Error";
        }
    } );

    device->registerDevice();

```

### Listing devices
```cpp
```

### Get device datanodes
```cpp
```

### Write data
```cpp
```

### Get device datanodes
```cpp
```

### Read data
```cpp
```

## API documentation
This Qt client library uses the IoT-Ticket REST API. The documentation for the underlying REST service can be found from
https://www.iot-ticket.com/images/Files/IoT-Ticket.com_IoT_API.pdf. 

doc folder contains detailed html API codumentation that is generated from the sources codes.
