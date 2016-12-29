# Introduction
TODO

## Configuration

By default Http server is listening on 8000 port.


## Rest API

This section contains description of rest api endpoints and data which can be send/received from it. All data are encoded using JSON protocol.

### Home plan management (path /plans)
Allows to read and current home plan, along with all sensors placed in rooms.

**Method: GET**
**Params: NONE**
Returns current plan stored in this server.

Example:
http://server/plans

```json
[
  "room" : {
    "id" : 8,
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
        "x" : 10, 
        "y" : 10,
        "name" : "sensor name",
        "address" : "physical sensor address",
        "type" : "sensor_type"
      },
      ....
      {
        "id" : 34,
        "x" : 12, 
        "y" : 12,
        "name" : "sensor name",
        "address" : "physical sensor address",
        "type" : "sensor_type"
      }
    ]
  }
]

```

### Rooms management (path /rooms)

Allows to add, remove and update single room instance.

**Method: GET**
**Params: id**
Returns room with given id, to obtain all rooms use /plans endpoint.

Example:
http://server/rooms?id=12

```json
{
  "id" : 12,
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
      "x" : 12, 
      "y" : 12,
      "name" : "sensor name",
      "address" : "physical sensor address",
      "type" : "sensor_type"
    },
    ....
    {
      "id" : 34,
      "x" : 10, 
      "y" : 10,
      "name" : "sensor name",
      "address" : "physical sensor address",
      "type" : "sensor_type"
    }
  ]
}
```

## 3rd Party libraries and frameworks

This project uses following libraries:
* [Mongoose](https://github.com/cesanta/mongoose)
* [JSON](https://github.com/nlohmann/json)

I would like to thanks their creators & contributors. Such people make live of other programmers much easier. Again thanks a lot!
