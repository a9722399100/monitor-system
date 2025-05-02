#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <sstream>
#include <string>
#include <curl/curl.h>
#include "include/nlohmann/json.hpp"

using json = nlohmann::json;

float getCpuUsage() {
    std::ifstream file("/proc/stat");
    std::string line;
    std::getline(file, line);
    std::istringstream ss(line);

    std::string cpu;
    long user, nice, system, idle, iowait, irq, softirq, steal;
    ss >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;

    long idle_time = idle + iowait;
    long total_time = user + nice + system + idle + iowait + irq + softirq + steal;

    static long prev_idle_time = 0, prev_total_time = 0;
    long delta_idle = idle_time - prev_idle_time;
    long delta_total = total_time - prev_total_time;

    prev_idle_time = idle_time;
    prev_total_time = total_time;

    if (delta_total == 0) return 0.0f;
    return (1.0f - (float)delta_idle / delta_total) * 100.0f;
}

float getMemoryUsage() {
    std::ifstream file("/proc/meminfo");
    std::string label;
    long total_memory = 0, free_memory = 0, buffers = 0, cached = 0;

    while (file >> label) {
        if (label == "MemTotal:") {
            file >> total_memory;
        } else if (label == "MemFree:") {
            file >> free_memory;
        } else if (label == "Buffers:") {
            file >> buffers;
        } else if (label == "Cached:") {
            file >> cached;
        }
    }

    long used_memory = total_memory - free_memory - buffers - cached;
    return (float)used_memory / total_memory * 100.0f;
}

float getCpuTemperature() {
    std::ifstream file("/sys/class/thermal/thermal_zone0/temp");
    if (!file.is_open()) {
        std::cerr << "⚠️ [DEBUG] 溫度檔案無法開啟" << std::endl;
        return -1.0f;
    }

    float temp;
    file >> temp;

    if (file.fail()) {
        std::cerr << "⚠️ [DEBUG] 溫度讀取失敗 (stream failbit)" << std::endl;
        return -1.0f;
    }

    std::cerr << "[DEBUG] 成功讀取原始溫度數值: " << temp << std::endl;
    return temp / 1000.0f;
}

void sendStatusToServer(float cpu_usage, float mem_usage, float temp) {
    CURL *curl = curl_easy_init();
    if (curl) {
        json j;
        j["cpu_usage"] = cpu_usage;
        j["memory_usage"] = mem_usage;

        if (temp >= 0.0f) {
            j["temperature"] = temp;
            std::cout << "[DEBUG] 包含溫度：" << temp << "°C" << std::endl;
        } else {
            j["temperature"] = nullptr;
            std::cout << "[DEBUG] 未包含溫度欄位（設為 null）" << std::endl;
        }

        std::string json_str = j.dump();

        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, "http://192.168.149.131:8000/data"); // <-- 根據你 Flask 伺服器的 IP 調整
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_str.c_str());

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "Failed to send data: " << curl_easy_strerror(res) << std::endl;
        } else {
            std::cout << json_str << std::endl;
            std::cout << "Data sent successfully!" << std::endl;
        }

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }
}

int main() {
    while (true) {
        float cpu = getCpuUsage();
        float mem = getMemoryUsage();
        float temp = getCpuTemperature();

        std::cout << "CPU Usage: " << cpu << "%, "
                  << "Memory Usage: " << mem << "%";

        if (temp < 0) {
            std::cout << ", CPU Temperature: N/A" << std::endl;
            std::cerr << "⚠️ 無法讀取 CPU 溫度（可能是虛擬機）" << std::endl;
        } else {
            std::cout << ", CPU Temperature: " << temp << "°C" << std::endl;
            if (temp >= 90.0f) {
                std::cerr << "🔥 CRITICAL: CPU temperature dangerously high! (" << temp << "°C)" << std::endl;
            } else if (temp >= 80.0f) {
                std::cerr << "⚠️ WARNING: CPU temperature high (" << temp << "°C)" << std::endl;
            }
        }

        sendStatusToServer(cpu, mem, temp);
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }

    return 0;
}
