# 使用官方 Python 映像作為基底
FROM python:3.10-slim

# 設定工作目錄
WORKDIR /app

# 複製程式碼到容器內
COPY . /app

# 安裝 Python 相依套件
RUN pip install --no-cache-dir -r requirements.txt

# 啟動 Flask 應用程式
CMD ["python", "server.py"]
