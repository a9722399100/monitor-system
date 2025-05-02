# 🖥️ Monitor System

A lightweight system monitoring project using **C++ (client)** and **Python Flask (server)** for collecting and transmitting system data such as CPU usage, memory usage, and temperature (if available).

---

## 📦 Project Structure

```
monitor-system/
├── client/             # C++ program to monitor system stats and send to server
│   ├── main.cpp
│   ├── Makefile
│   └── include/        # nlohmann/json.hpp header
├── server/             # Flask server to receive and process monitoring data
│   ├── server.py
│   ├── requirements.txt
│   ├── Dockerfile
│   ├── data_log.csv
│   └── templates/
│       └── dashboard.html
```

---

## 🚀 Features

- 📡 Real-time CPU & Memory usage monitoring
- 🌡️ CPU temperature collection (with fallback handling on virtual machines)
- 🔄 Data sent every 5 seconds to Flask server via HTTP POST
- 🧠 Flask server logs and parses incoming data to `data_log.csv`
- ⚠️ Temperature warning and critical thresholds (>= 80°C / 90°C)
- 🐳 Containerized Flask server for easy deployment
- 📈 Real-time dashboard with Chart.js at `/dashboard`
- 🛠️ Designed to be easily extendable to Docker, CI/CD, or database storage

---

## 🧰 Technologies Used

- C++17 (`std::thread`, `std::ifstream`, `libcurl`, `nlohmann/json`)
- Python 3 (`Flask`)
- Linux system APIs (`/proc/stat`, `/proc/meminfo`, `/sys/class/thermal`)
- Docker (container deployment)
- Git (version control)
- JSON over HTTP (REST-style communication)
- Chart.js (real-time graph rendering)

---

## 📥 How to Run

### 🔧 Server (Python Flask - Native)

```bash
cd server
python3 -m venv venv
source venv/bin/activate
pip install flask
python server.py
```

> Server will run on `http://localhost:8000`

---

### ⚙️ Client (C++)

```bash
cd client
make
./monitor_client
```

> Make sure to update the server IP in `main.cpp` to match your Flask server address.

---

### 🐳 Run Flask Server with Docker

```bash
cd server
docker build -t monitor-server .
docker run -d -p 8000:8000 monitor-server
```

> Flask API will be available at http://localhost:8000/data

---

### 📁 Docker Build Clean-Up with .dockerignore

This project uses a `.dockerignore` file to prevent unnecessary files from being included in the Docker image, such as:

- `venv/` (Python virtual environment)
- `__pycache__/`, `*.pyc`
- `.git/`, `.vscode/`

This helps reduce image size and speeds up build time.

---

## 🌐 Web Interface

| Endpoint | Description |
|----------|-------------|
| `/dashboard` | Chart.js-based report showing CPU / Memory / Temperature trends |
| `/history`   | JSON API returning full `data_log.csv` records |

To test manually:

```bash
curl -X POST http://localhost:8000/data -H "Content-Type: application/json" -d '{"cpu_usage":22.5, "memory_usage":57.3, "temperature":64.2}'
```

---

## 📋 Sample Output

**Client:**

```
CPU Usage: 12.4%, Memory Usage: 56.7%, CPU Temperature: 49.2°C
[DEBUG] 包含溫度：49.2°C
Data sent successfully!
```

**Server:**

```
Received data: {'cpu_usage': 12.4, 'memory_usage': 56.7, 'temperature': 49.2}
🌡️ CPU Temperature: 49.2°C
```

---

## 🧪 Future Enhancements (Planned)

- [ ] Docker Compose for client/server orchestration
- [ ] SQLite / CSV viewer or browser download
- [ ] GitHub Actions CI for build & linting
- [ ] Multi-device support via `device_id`

---


---

## 📁 Runtime Output Notice

The file `server/data_log.csv` is automatically created by `server.py` to log incoming monitoring data.

This file is intentionally excluded from Git version control via `.gitignore`, because it contains runtime-generated content.

You don't need to manually create this file — it will be created and appended to automatically.


## 📄 License

This project is open-sourced under the MIT License.

---

## 🙋‍♂️ Author

Developed by [Kobayashi](https://github.com/a9722399100)
