#  SmartPlantCareSystem
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

##  Overview
SmartPlantCareSystem is an innovative project aimed at automating the care of plants using advanced technologies such as AI and IoT. This system is designed to monitor plant health and provide necessary care through automated processes.

##  Features
- Automated plant care system
- AI-based plant disease detection
- Remote monitoring and control via mobile app
- Detailed component list and system design

##  Tech Stack
- ESP32
- Python (**Flask**, **TensorFlow**)
- Arduino
- MIT App Inventor

##  Screenshots/Demo
<!-- Người dùng sẽ tự thêm ảnh vào đây. Gợi ý: tạo thư mục 'assets/' hoặc 'images/' trong repo để chứa ảnh. Ví dụ: ![Mô tả ảnh](assets/screenshot.png) -->

##  Project Structure

```text
/
├── arduino/
│   ├── CHAM_SOC_CAY.ino
│   └── ESP32_Code.md
├── app/
│   └── CHAM_SOC_CAY_TRONG.aia
├── assets/
│   ├── poster-khkt.png
│   └── Data_Structure.png
├── designs/
│   └── System_Design.fzz
├── docs/
│   ├── Report.md
│   ├── Component_List.xlsx
│   ├── Project_Overview.md
│   └── Presentation.md
├── models/
│   └── CayBenh_model.h5
├── scripts/
│   ├── Converter_Image.py
│   ├── Train_AI_Code.md
│   └── DoGetPost_Script.md
└── server/
└── AI_Detection_Server.py

```

##  Prerequisites
- Python 3.x
- Arduino IDE
- MIT App Inventor
- Git LFS

##  Installation
1. Clone the repository:

```bash
git clone https://github.com/YOUR_NAME/SmartPlantCareSystem.git

```
2. Install required Python packages:

```bash
pip install -r requirements.txt

```
3. Set up Arduino environment and upload the code to ESP32.

##  Usage
1. Run the AI detection server:

```bash
python server/AI_Detection_Server.py

```
2. Use the mobile app to monitor and control the plant care system.

##  Contributing
Contributions are welcome! Please fork the repository and submit a pull request for review.

##  Author(s)
- chicc12 ([GitHub Profile](https://github.com/chicc12))

##  License
Dự án này được cấp phép theo Giấy phép MIT - xem file `LICENSE` để biết chi tiết.
