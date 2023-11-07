import { useEffect, useState } from 'react'
import reactLogo from './assets/react.svg'
import viteLogo from '/vite.svg'
import './App.css'
import axios from 'axios'

function App() {

  const [player, setPlayer] = useState(0)
  const [newPosition, setNewPosition] = useState("")

  useEffect(() => {

    switch (window.location.pathname) {
      case "/slong/player1": setPlayer(1); break;
      case "/slong/player2": setPlayer(2); break;
      default: break;
    }

  }, [window.location.pathname])

  return (
    <div className='flex justify-center'>
      <div className='max-w-60r w-full p-4 text-center'>
        <div className='flex flex-col'>
          <span className='text-3xl font-bold'>Slong</span>
          <span className='text-base font-bold'>(Slow Pong)</span>

          <div className='flex flex-col mt-4 p-4 border border-solid rounded-md border-gray-400'>

            <span className='font-bold my-4'>Player {player}</span>

            <div className='flex flex-col w-full'>
              <form className='flex flex-col items-center w-full' onSubmit={(e) => {
                e.preventDefault()
                axios.post("http://jepmac:8080/move/player" + player, { position: +newPosition }).then((res) => {
                  console.log("JEP response:", res.data)
                }).catch((err) => {
                  console.error("JEP error:", err)
                })
              }}>
                <input
                  name='position-input'
                  type="text"
                  placeholder="enter position"
                  className="input input-bordered w-full max-w-sm"
                  maxLength={2}
                  onChange={(e) => {
                    setNewPosition(e.target.value)
                  }} />
                <div className='flex justify-end max-w-sm w-full mt-2'>
                  <button className="btn btn-primary">Move Paddle</button>
                </div>
              </form>
            </div>

          </div>
        </div>
      </div>
    </div>
  )
}

export default App
