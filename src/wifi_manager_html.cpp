//
// Gimi Piggy, Copyright 2026.
//

#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>

#include "wifi_manager_html.h"

// HTML for configuration page
const char htmlPage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Gimi Piggy WiFi Setup</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            margin: 0;
            padding: 20px;
            min-height: 100vh;
            display: flex;
            justify-content: center;
            align-items: center;
        }
        .container {
            background: white;
            padding: 30px;
            border-radius: 10px;
            box-shadow: 0 10px 40px rgba(0,0,0,0.2);
            max-width: 400px;
            width: 100%;
        }
        h1 {
            color: #333;
            text-align: center;
            margin-bottom: 30px;
        }
        label {
            display: block;
            margin-bottom: 8px;
            color: #555;
            font-weight: bold;
        }
        input[type="text"], input[type="password"] {
            width: 100%;
            padding: 12px;
            margin-bottom: 20px;
            border: 2px solid #ddd;
            border-radius: 5px;
            box-sizing: border-box;
            font-size: 16px;
        }
        input[type="text"]:focus, input[type="password"]:focus {
            outline: none;
            border-color: #667eea;
        }
        button {
            width: 100%;
            padding: 12px;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            color: white;
            border: none;
            border-radius: 5px;
            font-size: 16px;
            font-weight: bold;
            cursor: pointer;
            transition: transform 0.2s;
        }
        button:hover {
            transform: translateY(-2px);
        }
        button:active {
            transform: translateY(0);
        }
        .info {
            background: #f0f7ff;
            padding: 15px;
            border-radius: 5px;
            margin-bottom: 20px;
            border-left: 4px solid #667eea;
        }
        .scan-btn {
            background: linear-gradient(135deg, #f093fb 0%, #f5576c 100%);
            margin-bottom: 20px;
        }
        #networks {
            margin-bottom: 20px;
        }
        .network-item {
            padding: 10px;
            margin: 5px 0;
            background: #f5f5f5;
            border-radius: 5px;
            cursor: pointer;
            transition: background 0.2s;
        }
        .network-item:hover {
            background: #e0e0e0;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>📡 Gimi Piggy WiFi Setup</h1>
        <div class="info">
            Connect your Piggy to your WiFi network
        </div>
        
        <button class="scan-btn" onclick="scanNetworks()">Scan Networks</button>
        <div id="networks"></div>
        
        <form action="/save" method="POST">
            <label for="ssid">WiFi Network Name (SSID):</label>
            <input type="text" id="ssid" name="ssid" required>
            
            <label for="password">WiFi Password:</label>
            <input type="password" id="password" name="password">
            
            <button type="submit">Save & Connect</button>
        </form>
    </div>
    
    <script>
        function scanNetworks() {
            document.getElementById('networks').innerHTML = '<p>Scanning...</p>';
            fetch('/scan')
                .then(response => response.json())
                .then(data => {
                    let html = '';
                    data.networks.forEach(network => {
                        html += '<div class="network-item" onclick="selectNetwork(\'' + 
                                network.ssid + '\')">' + network.ssid + ' (' + 
                                network.rssi + ' dBm)' + 
                                (network.encryption ? ' 🔒' : '') + '</div>';
                    });
                    document.getElementById('networks').innerHTML = html;
                })
                .catch(err => {
                    document.getElementById('networks').innerHTML = '<p>Scan failed</p>';
                });
        }
        
        function selectNetwork(ssid) {
            document.getElementById('ssid').value = ssid;
        }
    </script>
</body>
</html>
)rawliteral";

const char successPage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Success</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            margin: 0;
            padding: 20px;
            min-height: 100vh;
            display: flex;
            justify-content: center;
            align-items: center;
        }
        .container {
            background: white;
            padding: 40px;
            border-radius: 10px;
            box-shadow: 0 10px 40px rgba(0,0,0,0.2);
            text-align: center;
        }
        h1 {
            color: #4caf50;
        }
        p {
            color: #555;
            margin: 20px 0;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>✓ Configuration Saved!</h1>
        <p>Piggy is now connecting to your WiFi network.</p>
        <p>The device will restart automatically.</p>
    </div>
</body>
</html>
)rawliteral";


