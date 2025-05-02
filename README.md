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
│   └── requirements.txt
```

---

## 🚀 Features

- 📡 Real-time CPU & Memory usage monitoring
- 🌡️ CPU temperature collection (with fallback handling on virtual machines)
- 🔄 Data sent every 5 seconds to Flask server via HTTP POST
- 🧠 Flask server logs and parses incoming data
- ⚠️ Temperature warning and critical thresholds (>= 80°C / 90°C)
- 🛠️ Designed to be easily extendable to Docker, CI/CD, or database storage

---

## 🧰 Technologies Used

- C++17 (`std::thread`, `std::ifstream`, `libcurl`, `nlohmann/json`)
- Python 3 (`Flask`)
- Linux system APIs (`/proc/stat`, `/proc/meminfo`, `/sys/class/thermal`)
- Git (version control)
- JSON over HTTP (REST-style communication)

---

## 📥 How to Run

### 🔧 Server (Python Flask)

```bash
cd server
python3 -m venv venv
source venv/bin/activate
pip install flask
python server.py
```

> Server will run on `http://0.0.0.0:8000/data`

---

### ⚙️ Client (C++)

```bash
cd client
make
./monitor_client
```

> Make sure to update the server IP in `main.cpp` to match your Flask server address.

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

- [ ] Dockerfile & docker-compose support
- [ ] SQLite / CSV data storage
- [ ] Real-time web dashboard (Chart.js + Flask)
- [ ] GitHub Actions CI for build & linting
- [ ] Multi-device support via `device_id`

---

## 📄 License

This project is open-sourced under the MIT License.

---

## 🙋‍♂️ Author

Developed by [Kobayashi](https://github.com/a9722399100)
