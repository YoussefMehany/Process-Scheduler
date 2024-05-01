const { ipcRenderer } = require("electron");

document.addEventListener("DOMContentLoaded", () => {
  // Flag to keep track if column headers have been appended
  let headersAppended = false;
  let formattedData = "";

  // Listen for the processData event from the main process
  ipcRenderer.on("processData", (event, data) => {
    const lines = data.split("\n");

    // Append column headers if they haven't been appended yet
    if (!headersAppended) {
      formattedData +=
        "<table><tr><th>ID</th><th>Arrival</th><th>Start Time</th><th>Runtime</th><th>Priority</th><th>Waiting Time</th><th>Remaining Time</th><th>State</th></tr>";
      headersAppended = true; // Set flag to true after appending headers
    }

    // Append row data for each line of data
    formattedData += "<tr>"; // Start a new row
    lines.forEach((line) => {
      const parts = line.split(":");
      if (parts.length === 2) {
        const value = parts[1].trim();
        formattedData += `<td>${value}</td>`;
      }
    });
    formattedData += "</tr>";
    formattedData += "</table>";
    document.getElementById("output").innerHTML = formattedData; // Clear and set content
    const closingTagIndex = formattedData.lastIndexOf("</table>");

    if (closingTagIndex !== -1) {
      // Remove the closing </table> tag
      formattedData = formattedData.substring(0, closingTagIndex);
    }
  });
});
