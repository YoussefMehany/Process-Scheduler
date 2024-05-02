const { ipcRenderer } = require("electron");

document.addEventListener("DOMContentLoaded", () => {
  // Flag to keep track if column headers have been appended
  let formattedData = "";
  ipcRenderer.on("loadTableHeader", (event, data) => {
    formattedData +=
      "<table><tr><th>ID</th><th>Arrival</th><th>Start Time</th><th>Runtime</th><th>Priority</th><th>Waiting Time</th><th>Remaining Time</th><th>State</th></tr>";
    formattedData += "</table>";
    document.getElementById("output").innerHTML = formattedData; // Clear and set content
    const closingTagIndex = formattedData.lastIndexOf("</table>");

    if (closingTagIndex !== -1) {
      formattedData = formattedData.substring(0, closingTagIndex);
    }
  });

  // Listen for the processData event from the main process
  ipcRenderer.on("processData", (event, data) => {
    const lines = data.split("\n");

    // Append column headers if they haven't been appended yet

    // Append row data for each line of data
    const savedData = [];
    lines.forEach((line) => {
      const parts = line.split(":");
      if (parts.length === 2) {
        const value = parts[1].trim();
        savedData.push(value);
      }
    });
    console.log(savedData.length);
    for (let i = 0; i < savedData.length; i += 8) {
      formattedData += "<tr>";
      for (let j = i; j < i + 8 && j < savedData.length; j++) {
        formattedData += `<td>${savedData[j]}</td>`;
      }
      formattedData += "</tr>";
    }
    formattedData += "</table>";
    document.getElementById("output").innerHTML = formattedData; // Clear and set content
    const closingTagIndex = formattedData.lastIndexOf("</table>");

    if (closingTagIndex !== -1) {
      formattedData = formattedData.substring(0, closingTagIndex);
    }
  });
});
