import express from "express"
import cors from "cors"
import BodyParser from "body-parser"
import mysql from "mysql"

const app = express()
app.use(cors())
app.use(BodyParser.json())
const port = 8080

app.post("/move/player1", (req, res) => {

    let resObj = {
        result: "success"
    }

    let raw = req.body.position

    if (!raw || raw.length > 2) {
        resObj.result = "the position must be a value between 1 and 75"
        res.status(400).send(JSON.stringify(resObj))
        return
    }

    let sInput = parseInt(raw)
    if (!sInput) {
        resObj.result = "the position must be a value between 1 and 75"
        res.status(400).send(JSON.stringify(resObj))
        return
    }
    if (sInput < 1 || sInput > 75) {
        resObj.result = "the position must be a value between 1 and 75"
        res.status(400).send(JSON.stringify(resObj))
        return
    }

    console.log("setting player 1 position:", req.body.position)

    var connection = mysql.createConnection({
        host: "localhost",
        port: 3306,
        user: "player1",
        password: "password1",
        database: "slong",
        socketPath: "/tmp/mysql.sock"
    })

    connection.connect()
    connection.query(`UPDATE paddles set position = ${sInput} WHERE id = 1`, function (error, results, fields) {
        if (error) {
            console.error("failed to updated player 1 paddle", error)
            resObj.result = "an error occurred while updating the data"
            res.status(500).send(JSON.stringify(resObj))
        }
        res.send(JSON.stringify(resObj))
    })
    connection.end()
})

app.post("/move/player2", (req, res) => {
    let resObj = {
        result: "success"
    }

    let raw = req.body.position

    if (!raw || raw.length > 2) {
        resObj.result = "the position must be a value between 1 and 75"
        res.status(400).send(JSON.stringify(resObj))
        return
    }

    let sInput = parseInt(raw)
    if (!sInput) {
        resObj.result = "the position must be a value between 1 and 75"
        res.status(400).send(JSON.stringify(resObj))
        return
    }
    if (sInput < 1 || sInput > 75) {
        resObj.result = "the position must be a value between 1 and 75"
        res.status(400).send(JSON.stringify(resObj))
        return
    }

    console.log("setting player 2 position:", req.body.position)

    var connection = mysql.createConnection({
        host: "localhost",
        port: 3306,
        user: "player2",
        password: "password2",
        database: "slong",
        socketPath: "/tmp/mysql.sock"
    })

    connection.connect()
    connection.query(`UPDATE paddles set position = ${sInput} WHERE id = 2`, function (error, results, fields) {
        if (error) {
            console.error("failed to updated player 2 paddle", error)
            resObj.result = "an error occurred while updating the data"
            res.status(500).send(JSON.stringify(resObj))
        }
        res.send(JSON.stringify(resObj))
    })
    connection.end()
})

app.listen(port, () => {
    console.log(`Slong service listening on port ${port}`)
})

const SlongService = {}

export default SlongService
