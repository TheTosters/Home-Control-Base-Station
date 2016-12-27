# Introduction
TODO

## Configuration

By default Http server is listening on 8000 port.


## Rest API

This section contains description of rest api endpoints and data which can be send/received from it. All data are encoded using JSON protocol.

### Home plan management (path /plans)
Allows to read and current home plan, along with all sensors placed in rooms.

**Method: GET**
Returns current plan stored in this server.

Example:
```json
[
  "room" : {
    "name" : "room name",
    "floor" : 1,
    "shape" : [
      { 
        "id" : 1,
        "x" : 10,
        "y" : 10
      }, 
        ...
      { 
        "id" : 12,
        "x" : 20,
        "y" : 20
      }
    ],
    "sensors" : [
      {
        "id" : 33,
        "position" : { "x" : 10, "y" : 10},
        "name" : "sensor name",
        "address" : "address",
        "type" : "sensor_type"
      },
      ....
      {
        "id" : 34,
        "position" : { "x" : 12, "y" : 12},
        "name" : "sensor name",
        "address" : "address",
        "type" : "sensor_type"
      }
    ]
  }
]
```

## 3rd Party libraries and frameworks

This project uses following libraries:
* [Mongoose](https://github.com/cesanta/mongoose)
* [JSON](https://github.com/nlohmann/json)

I would like to thanks their creators & contributors. Such people make live of other programmers much easier. Again thanks a lot!
