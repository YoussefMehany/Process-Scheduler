const { ipcRenderer } = require("electron");

document.addEventListener("DOMContentLoaded", () => {
  const gotoAlgorithmBtn = document.getElementById("start-rr-btn");
  const inputElement = document.querySelector(".input");
  gotoAlgorithmBtn.addEventListener("click", () => {
    const quantum = inputElement.value;
    const args = ["1", quantum];
    ipcRenderer.send("submit", args);
    ipcRenderer.send("gototaskManager");
  });
});
