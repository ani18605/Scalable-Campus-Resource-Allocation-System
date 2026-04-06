const express = require('express');
const { spawn } = require('child_process');
const path = require('path');

const app = express();
app.use(express.json());
app.use(express.static('public'));

// Spawn the C++ backend process
const cppProcess = spawn('./build_campus.exe', [], { stdio: ['pipe', 'pipe', 'inherit'] });

let pendingRequests = [];
let buffer = '';

cppProcess.stdout.on('data', (data) => {
    buffer += data.toString();
    
    // Process all completed responses separated by our IPC delimiter
    while (buffer.includes('===END===\n') || buffer.includes('===END===\r\n')) {
        let delimiter = buffer.includes('===END===\r\n') ? '===END===\r\n' : '===END===\n';
        let splitIdx = buffer.indexOf(delimiter);
        let response = buffer.substring(0, splitIdx).trim();
        buffer = buffer.substring(splitIdx + delimiter.length);
        
        if (pendingRequests.length > 0) {
            let req = pendingRequests.shift();
            req.res.json({ output: response });
        }
    }
});

cppProcess.on('exit', (code) => {
    console.log("C++ Backend process exited with code " + code);
});

app.post('/api/command', (req, res) => {
    const cmd = req.body.command;
    if (!cmd) return res.status(400).json({ error: "Missing command" });
    
    pendingRequests.push({ req, res });
    cppProcess.stdin.write(cmd + '\n');
});

const PORT = 3000;
app.listen(PORT, () => {
    console.log(`Server running on http://localhost:${PORT}`);
});
