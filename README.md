Here is the updated README.md with your exact parts.csv column structure and GPIO pin configurations (including the notes for both the WaveShare ESP32-S3 Zero on Pin 2 and the ESP32-S3 N16R8 on Pin 18, plus your 50-LED drawer configuration).
markdown
# WLED-Zero Inventory Drawer System

A smart component inventory and LED drawer locator built with **Django** and an **ESP32-S3 Zero** microcontroller. This system helps you organize electronic parts, bulk-manage inventory via CSV, and visually locate storage bins using a 50-LED addressable WS2812B strip.

---

## ✨ Key Features

* **LED Bin Locator:** Integrates with an ESP32-S3 Zero (18-pin tiny board) to light up specific drawer slots across a 50-LED array when searching for parts.
* **Automatic LED Timeout:** Automatically turns off indicator LEDs after a set period to save power and reduce wear.
* **Progressive Web App (PWA):** Installable web interface configured directly within Django for quick mobile and desktop access.
* **CSV Import & Restore:** Easily bulk-import, back up, and restore your parts database using `parts.csv`.
* **Network Auto-Detection & mDNS:** Supports automatic MCU IP detection and mDNS discovery (`ESPmDNS`) for reliable local network communication.

---

## 📁 Project Structure

```text
├── 1_5_sketch_oct25a_copy/   # Arduino firmware sketch (.ino) for the ESP32-S3 Zero
├── inventory/                # Core Django app handling parts, LED triggers, and CSV logic
├── inventory_project/        # Django project settings, URLs, and PWA configuration
├── manage.py                 # Django command-line utility
├── parts.csv                 # Sample/backup CSV file for bulk inventory import & restore
├── requirements.txt          # Python dependencies
└── Readme.txt                # Quick setup notes
```

---

## 🔌 Wiring Diagram (ESP32-S3 to WS2812B LED Strip)

Connect your **WaveShare ESP32-S3 Zero** to the **50-LED WS2812B strip** (`NUM_LEDS = 50`) using the pinout below.

| ESP32-S3 Zero Pin | LED Strip Pin | Wire Color (Typical) | Notes |
| :--- | :--- | :--- | :--- |
| **5V (VBUS)** | **5V / VCC** | Red | Recommended to use an external 5V power supply when driving 50 LEDs. |
| **GND** | **GND** | Black / White | Must share a common ground between the ESP32-S3 and any external 5V power supply. |
| **GP2** (`LED_PIN 2`) | **DIN (Data In)** | Green | Default data pin for **WaveShare ESP32-S3 Zero**. Connect via a 330Ω–470Ω resistor. |

> **Alternative Board Note:** If flashing to an **ESP32-S3 N16R8**, change `#define LED_PIN` to **Pin 18** (`GP18`).
>
> **Power Tip:** Place a 1000µF capacitor across the 5V and GND terminals of the LED strip to prevent voltage spikes on startup.

---

## 🛠️ Hardware Setup (ESP32-S3 Zero)

This firmware is configured for the **WaveShare ESP32-S3 Zero** (18-pin mini board) using the `Adafruit_NeoPixel` and `ESPmDNS` libraries.

1. Open the `.ino` file located inside the `1_5_sketch_oct25a_copy/` folder using the **Arduino IDE**.
2. Ensure **Adafruit NeoPixel** is installed via the Arduino Library Manager.
3. Select **ESP32S3 Dev Module** under **Tools > Board**.
4. Put the board into bootloader mode:
   * Press and hold the **BOOT** button.
   * Plug the USB cable into your computer.
   * Release the **BOOT** button.
5. Click **Upload** in the Arduino IDE and wait for the flashing process to complete.
6. Press the **RESET** button on the board so it connects to your local Wi-Fi network.

---

## 💻 Local Software Setup (Django)

Follow these steps to run the inventory web app on your local machine (tested on Linux/WSL).

### 1. Clone the Repository
```bash
git clone https://github.com
cd WLED-Zero-autocsvpwa-detectip-mDNS-restorecsv
```

### 2. Create and Activate a Virtual Environment
```bash
python3 -m venv myenv
source myenv/bin/activate
```

### 3. Install Dependencies
```bash
pip install -r requirements.txt
```

### 4. Apply Database Migrations & Run the Server
```bash
python manage.py migrate
python manage.py runserver 0.0.0.0:8000
```

---

## 📊 CSV Formatting Guide (`parts.csv`)

Use `parts.csv` to bulk-import new components or restore a backup of your drawer database. Ensure your file uses **UTF-8 encoding** and **comma-separated (`,`)** values.

### Column Specifications

| Column Name | Type | Description | Example |
| :--- | :--- | :--- | :--- |
| `name` | Text | Component name or title. | `10K Resistor` |
| `led_index` | Integer | Target LED position on the 50-LED strip (`0` to `49`). | `3` |
| `quantity` | Integer | Current stock count in the drawer. | `100` |
| `part_number` | Text | Internal SKU or manufacturer part code. | `RES-10K` |
| `value` | Text | Electrical value or rating (leave blank if not applicable). | `10k ohm` |
| `package_type` | Text | Mounting or form factor (e.g., `SMD`, `Through-hole`). | `Through-hole` |
| `storage_location` | Text | Physical drawer or bin label. | `Bin A2` |

### Example `parts.csv`
```csv
name,led_index,quantity,part_number,value,package_type,storage_location
ESP32-S3 Zero,2,5,MCU-S3-01,,SMD,Bin A1
10K Resistor,3,100,RES-10K,10k ohm,Through-hole,Bin A2
WS2812B Strip,4,2,LED-WS-50,,SMD,Bin B1
```

### Import & Restore Tips
* **Keep Exact Headers:** Keep the header row strictly on line 1 using the exact column names above.
* **Empty Optional Values:** Leave fields like `value` empty between commas (e.g., `MCU-S3-01,,SMD`) when a part does not have a specific electrical value.
* **Zero-Based Indexing:** Addressable LED strips start at index `0` for the first LED and go up to `49` for a 50-LED setup.

---

## 🌐 Accessing the Application

Once the server is running, you can access the system from your browser on the same local network:

* **Parts Inventory:** `http://127.0.0` (or `http://<YOUR-LOCAL-IP>:8000/parts/`)
* **Django Admin Panel:** `http://127.0.0` (or `http://<YOUR-LOCAL-IP>:8000/admin/`)

---
ps. in my case:
Test with:
http://192.168.43.244:8000/parts/
http://127.0.0.1:8000/parts/
http://192.168.43.244:8000/admin/
http://192.168.43.244/ page not found landing

for esp32s3 Zero board
http://192.168.43.129:8000/parts/
http://127.0.0.1:8000/parts/
http://192.168.43.129:8000/admin/
http://192.168.43.129/ page not found landing
---

## 📝 Troubleshooting & Notes

* **ESP32-S3 Direct IP:** Visiting the ESP32-S3 Zero's IP address directly in a browser (e.g., `http://192.168.43`) may show a default landing/not-found page if it is configured strictly to listen for API/LED commands from Django.
* **Network Discovery:** Ensure both your host computer and the ESP32-S3 Zero are connected to the same 2.4 GHz Wi-Fi network so mDNS and IP detection work properly.

Use code with caution.
If you'd like to add an API Endpoints section showing the exact HTTP routes your .ino sketch listens to (like /led?index=2 or /off), paste the server.on(...) lines from your sketch and I can document them for you.

