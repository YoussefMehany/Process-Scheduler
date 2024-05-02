const { ipcRenderer } = require("electron");

document.addEventListener("DOMContentLoaded", () => {
  console.log("render.js is loaded");

  const gotoAlgorithmBtn = document.getElementById("gotoAlgorithmBtn");

  gotoAlgorithmBtn.addEventListener("click", () => {
    ipcRenderer.send("gotoInputFile");
  });
});
