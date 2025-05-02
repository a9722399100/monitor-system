from flask import Flask, request, jsonify

app = Flask(__name__)

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

    # 溫度欄位處理
    if temp is None:
        print("⚠️ 無法讀取 CPU 溫度（可能是虛擬機）")
    else:
        print(f"🌡️ CPU Temperature: {temp:.2f}°C")
        if temp >= 90.0:
            print("🔥 CRITICAL: CPU temperature dangerously high!")
        elif temp >= 80.0:
            print("⚠️ WARNING: CPU temperature high")

    print("-" * 40)
    return jsonify({'status': 'ok'}), 200

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=8000)


