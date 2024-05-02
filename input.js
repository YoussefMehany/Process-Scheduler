const { ipcRenderer } = require("electron");

document.addEventListener("DOMContentLoaded", () => {
  const gotoAlgorithmBtn = document.getElementById("start-rr-btn");
  const inputElement = document.querySelector(".input");
  gotoAlgorithmBtn.addEventListener("click", () => {
    ipcRenderer.send("gotoAlgorithmSelection", inputElement.value);
  });
});
