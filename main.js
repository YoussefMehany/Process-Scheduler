const { app, BrowserWindow, ipcMain } = require("electron");
const puppeteer = require("puppeteer");
const path = require("path");
const { spawn } = require("child_process");

let mainWindow;
let file;
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
  const notRR = argInput === "2" || argInput === "3";
  const trimmedArgs = notRR
    ? [argInput.trim()]
    : argInput.map((str) => str.trim());
  console.log(trimmedArgs);
  if (trimmedArgs !== "") {
    const command = "./process_generator.out";
    const args = trimmedArgs;
    args.push(file);
    console.log(command, args);

    try {
      const childProcess = spawn(command, args, {
        stdio: "pipe",
        shell: true,
      });

      process.on("SIGINT", () => {
        process.stdout.write("");
        setTimeout(() => {
          takeScreenshotAndExit();
        }, 2000);
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
ipcMain.on("gotoAlgorithmSelection", (event, filePath) => {
  file = filePath;
  console.log(file);
  mainWindow.loadFile(path.join(__dirname, "index.html"));
});

ipcMain.on("gotoInputFile", () => {
  mainWindow.loadFile(path.join(__dirname, "input.html"));
});

ipcMain.on("gotoquantumInput", () => {
  mainWindow.loadFile(path.join(__dirname, "quantum.html"));
});

ipcMain.on("gototaskManager", async () => {
  await mainWindow.loadFile(path.join(__dirname, "manager.html"));
  mainWindow.webContents.send("loadTableHeader");
});

async function takeScreenshotAndExit() {
  try {
    const browser = await puppeteer.launch();
    const page = await browser.newPage();
    await page.goto(`file://${__dirname}/scheduler.log`);
    const contentHeight = await page.evaluate(() => {
      return document.body.scrollHeight;
    });

    await page.setViewport({
      width: 1920,
      height: contentHeight,
      deviceScaleFactor: 2,
    });
    await page.screenshot({ path: "output_screenshot.png", fullPage: true });
    await browser.close();
    process.exit(0);
  } catch (error) {
    console.error(`Error taking screenshot: ${error.message}`);
    process.exit(1);
  }
}
