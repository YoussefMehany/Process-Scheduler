const { app, BrowserWindow, ipcMain } = require("electron");
const path = require("path");
const { spawn } = require("child_process");

let mainWindow;
app.disableHardwareAcceleration();
const createWindow = () => {
  mainWindow = new BrowserWindow({
    width: 800,
    height: 600,
    webPreferences: {
      nodeIntegration: true,
      contextIsolation: false,
    },
  });
  // mainWindow.webContents.openDevTools();
  mainWindow.loadFile(path.join(__dirname, "welcome.html"));

  mainWindow.on("closed", () => {
    mainWindow = null;
  });
};

app.on("ready", createWindow);

app.on("activate", () => {
  if (mainWindow === null) {
    createWindow();
  }
});

app.on("window-all-closed", () => {
  if (process.platform !== "darwin") {
    app.quit();
  }
});

ipcMain.on("submit", (event, argInput) => {
  if (argInput.trim() !== "") {
    const command = "./process_generator.out";
    const args = [argInput];
    console.log(command, args);

    try {
      const childProcess = spawn(command, args, {
        stdio: "pipe",
        shell: true,
      });

      process.on("SIGINT", () => {
        process.stdout.write("");
        setTimeout(() => {
          process.exit(0);
        }, 1000);
      });

      childProcess.stdout.on("data", (data) => {
        const result = `${data}`;
        //console.log(result); // Check if mainWindow exists before sending the message

        mainWindow.webContents.send("processData", result);
      });

      childProcess.stderr.on("data", (data) => {
        console.error(`stderr: ${data}`); // Check if mainWindow exists before sending the message
      });

      childProcess.on("error", (error) => {
        console.error(`error: ${error.message}`); // Send the error back to the renderer process
      });

      childProcess.on("close", (code) => {
        console.log(`child process exited with code 0`); // Send the exit code back to the renderer process
      });
    } catch (error) {
      console.error(`Error spawning child process: ${error.message}`); // Send the error back to the renderer process
    }
  }
});
ipcMain.on("gotoAlgorithmSelection", () => {
  mainWindow.loadFile(path.join(__dirname, "index.html")); // Load algorithm selection page
});

ipcMain.on("gototaskManager", () => {
  mainWindow.loadFile(path.join(__dirname, "manager.html")); // Load algorithm selection page
});
