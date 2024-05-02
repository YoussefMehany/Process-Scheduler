// button_handlers.js
const { ipcRenderer } = require("electron");

document.addEventListener("DOMContentLoaded", () => {
  const rrBtn = document.getElementById("rrBtn");
  const srtnBtn = document.getElementById("srtnBtn");
  const hpfBtn = document.getElementById("hpfBtn");

  rrBtn.addEventListener("click", () => {
    console.log("RR Button clicked");
    ipcRenderer.send("gotoquantumInput");
  });

  srtnBtn.addEventListener("click", () => {
    console.log("SRTN Button clicked");
    ipcRenderer.send("submit", "2");
    ipcRenderer.send("gototaskManager");
  });

  hpfBtn.addEventListener("click", () => {
    console.log("HPF Button clicked");
    ipcRenderer.send("submit", "3");
    ipcRenderer.send("gototaskManager");
  });
});
