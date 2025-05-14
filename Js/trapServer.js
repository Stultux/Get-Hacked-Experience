const express = require('express');
const { exec } = require('child_process');
const app = express();
const PORT = 3000;

// Route, die das C++ Programm startet
app.get('/run-cpp', (req, res) => {
    // Ausführbare Datei starten
    exec('../c++/LF4_Project', (error, stdout, stderr) => {
        if (error) {
            console.error(`Fehler beim Starten des Programms: ${error.message}`);
            res.status(500).send("Fehler beim Ausführen der Datei.");
            return;
        }
        if (stderr) {
            console.error(`Fehlerausgabe: ${stderr}`);
            res.status(500).send(stderr);
            return;
        }
        res.send(`Programm erfolgreich ausgeführt: \n${stdout}`);
    });
});

// Server starten
app.listen(PORT, () => {
    console.log(`Server läuft auf http://localhost:${PORT}`);
});