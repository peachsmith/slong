/** @type {import('tailwindcss').Config} */
export default {
  content: ["./src/**/*.{html,js,jsx}"],
  theme: {
    extend: {
      maxWidth: {
        "60r": "60rem"
      }
    },
  },
  plugins: [
    require("daisyui"),
  ],
}
