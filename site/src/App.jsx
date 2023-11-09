import { useEffect, useState } from 'react'
import reactLogo from './assets/react.svg'
import viteLogo from '/vite.svg'
import './App.css'
import axios from 'axios'

function App() {

  const [player, setPlayer] = useState(0)
  const [newPosition, setNewPosition] = useState("")
  const [formError, setFormError] = useState("")
  const [moving, setMoving] = useState(false)

  useEffect(() => {

    switch (window.location.pathname) {
      case "/player1": setPlayer(1); break;
      case "/player2": setPlayer(2); break;
      default: break;
    }

  }, [window.location.pathname])

  const getBase = () => window.location.host == "jepmac.local" ? window.location.host : "jepmac"

  const movePaddle = () => {

    setMoving(true)

    axios.post(`http://${getBase()}:8080/move/player` + player, { position: +newPosition })
      .then((res) => {
        setMoving(false)
        console.log(res.data)
      })
      .catch((err) => {
        setMoving(false)
        console.log("error code:", err?.code)
        console.log("error message:", err?.message)
        console.log("error status:", err?.response?.status)
        console.log("error result:", err?.response?.data?.result)
        setFormError(err?.response?.data?.result || "an error occured while processing your request")
      })
  }

  return (
    <div className='flex justify-center'>
      <div className='max-w-60r w-full p-4 text-center'>
        <div className='flex flex-col'>
          <span className='text-3xl font-bold'>Slong</span>
          <span className='text-base font-bold'>(Slow Pong)</span>

          {player > 0 &&
            <div className='flex flex-col mt-4 p-4 border border-solid rounded-md border-gray-400'>

              <span className='font-bold my-4'>Player {player}</span>

              <div className='flex flex-col w-full'>
                <form className='flex flex-col items-center w-full' onSubmit={(e) => {
                  e.preventDefault()
                  if (moving) {
                    return
                  }
                  movePaddle()
                }}>
                  <div className='flex flex-col w-full items-center'>
                    <input
                      name='position-input'
                      type="text"
                      placeholder="enter position"
                      className={`input border border-solid
                        ${formError ? "border-red-500" : "border-gray-400"}
                        w-full max-w-sm`}
                      maxLength={2}
                      onChange={(e) => {
                        setFormError("")
                        setNewPosition(e.target.value)
                      }} />
                    <span className='text-sm text-red-500 font-bold w-full max-w-sm text-start'>
                      {formError}
                    </span>
                  </div>
                  <div className='flex justify-end max-w-sm w-full mt-4'>
                    <button className="btn btn-primary w-full md:w-36">
                      {
                        moving
                          ? <span class="loading loading-spinner loading-sm"></span>
                          : "Move Paddle"
                      }
                    </button>
                  </div>
                </form>
              </div>

            </div>
          }
        </div>
      </div>
    </div>
  )
}

export default App
