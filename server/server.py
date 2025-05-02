from flask import Flask, request, jsonify, render_template
import csv
import os
from datetime import datetime

app = Flask(__name__, template_folder="templates")

CSV_FILE = "data_log.csv"

# 建立 CSV 檔案並加上標頭（如果還沒存在）
if not os.path.exists(CSV_FILE):
    with open(CSV_FILE, mode="w", newline="") as f:
        writer = csv.writer(f)
        writer.writerow(["timestamp", "cpu_usage", "memory_usage", "temperature"])

@app.route('/data', methods=['POST'])
def receive_data():
    data = request.get_json()

    # 輸出完整資料 JSON
    print("Received data:", data)

    # 個別欄位檢查
    cpu = data.get('cpu_usage')
    mem = data.get('memory_usage')
    temp = data.get('temperature')

    print(f"🔍 CPU Usage: {cpu:.2f}%")
    print(f"🔍 Memory Usage: {mem:.2f}%")

    if temp is None:
        print("⚠️ 無法讀取 CPU 溫度（可能是虛擬機）")
    else:
        print(f"🌡️ CPU Temperature: {temp:.2f}°C")
        if temp >= 90.0:
            print("🔥 CRITICAL: CPU temperature dangerously high!")
        elif temp >= 80.0:
            print("⚠️ WARNING: CPU temperature high")

    # 寫入 CSV
    timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    with open(CSV_FILE, mode="a", newline="") as f:
        writer = csv.writer(f)
        writer.writerow([timestamp, cpu, mem, temp])

    print("-" * 40)
    return jsonify({'status': 'ok'}), 200

@app.route('/history', methods=['GET'])
def history():
    records = []
    with open(CSV_FILE, mode="r") as f:
        reader = csv.DictReader(f)
        for row in reader:
            records.append(row)
    return jsonify(records)

@app.route('/dashboard')
def dashboard():
    return render_template("dashboard.html")

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=8000)
