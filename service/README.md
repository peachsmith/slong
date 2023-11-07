# Slong Service
A web API for playing Slong.
<br/>
## Running Locally
The service can be run with the following command:
```shell
node server.js
```

## Endpoints

This service contains two endpoints:
<br/>
`/move/player1` updates the position of player 1
<br/>
`/move/player2` updates the position of player 2
<br/>
<br/>
They both have the same request and response structure:
<br/>
**Method:** POST
<br/>
**Example Request:**
```json
{
    "position": 40
}
```

<br/>

**Example Response:**
```json
{
    "result": "success"
}
```
