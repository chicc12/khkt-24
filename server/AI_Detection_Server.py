
"""base64: Thư viện cho việc mã hóa và giải mã dữ liệu dưới dạng base64.
gspread: Thư viện để làm việc với Google Sheets.
ServiceAccountCredentials: Lớp để xử lý thông tin xác thực OAuth 2.0.
os: Module để tương tác với hệ điều hành.
time: Module cho các hàm liên quan đến thời gian.
tensorflow: Thư viện máy học mã nguồn mở.
PIL: Thư viện Python Imaging cho việc làm việc với ảnh.
numpy: Thư viện cho các phép toán số học."""
import base64
import gspread
from oauth2client.service_account import ServiceAccountCredentials
import os
import time
import tensorflow as tf
print(tf.__version__)
from PIL import ImageTk, Image
import numpy as np
"""attack: Hàm này tải một mô hình TensorFlow đã được huấn luyện trước (emergency.h5) và sử dụng nó để đưa ra dự đoán 
về một hình ảnh (downloaded_image.jpg). Nó trả về kết quả dự đoán (ketqua) và chỉ số cột tương ứng (colunm)."""
kq1=""
model = tf.keras.models.load_model(r"D:\SERVER_AI_PHAT_HIEN_CAY_BI_BENH\CayBenh_model.h5")
def attack():
    global img
    resulf = {
        0:'Hành bị bệnh',1:'Hành không bị bệnh',2: 'Nho bị bệnh',3: 'Nho không bị bệnh',4: 'Táo bị bệnh',5: 'Táo không bị bệnh'
    }
  
    filename=r"D:\SERVER_AI_PHAT_HIEN_CAY_BI_BENH\downloaded_image.jpg"
    imgs = tf.keras.utils.load_img(filename, target_size=(128, 128))
    imgs = tf.keras.utils.img_to_array(imgs)
    imgs = np.expand_dims(imgs, axis=0)  # Add an extra dimension for batch
    imgs = imgs.astype('float32')
    imgs = imgs / 255.0
    index = np.argmax(model.predict(imgs))
    print('Kết quả:', resulf[index].encode('utf-8', 'replace').decode('utf-8'))
    ketqua=resulf[index]
    return ketqua
"""Phần này thiết lập kết nối đến Google Sheets bằng cách sử dụng thông tin xác thực OAuth 2.0 được lưu trữ 
trong một tệp JSON (credentials.json)."""
# Đường dẫn đến file JSON chứa thông tin xác thực
credentials_path = 'key.json'

"""Những dòng này xác định ID của Google Sheet và tên của các bảng làm việc mà script sẽ sử dụng."""
# Phạm vi API bạn muốn sử dụng (ở đây là Google Sheets)
scope = ['https://spreadsheets.google.com/feeds', 'https://www.googleapis.com/auth/drive']

# Tạo một đối tượng creds từ file JSON và phạm vi đã chọn
creds = ServiceAccountCredentials.from_json_keyfile_name(credentials_path, scope)

# Tạo kết nối đến Google Sheets
client = gspread.authorize(creds)

# Đọc dữ liệu từ Google Sheet
spreadsheet_id = '1yG19k1C7d2ZngNFQads9KEvVcU-N5020ZtvBXgFe8cg'
worksheet_name = 'Sheet1'  # Tên của worksheet
worksheet = client.open_by_key(spreadsheet_id).worksheet(worksheet_name)

"""image_path là đường dẫn cục bộ nơi ảnh đã được tải về được lưu trữ, và max_api_calls là 
số lượng tối đa các lời gọi API được phép trong một khoảng thời gian nhất định."""
# Đường dẫn đến file hình ảnh
image_path = 'downloaded_image.jpg'

# Số lần gọi API tối đa trong một khoảng thời gian
max_api_calls = 5

"""Vòng lặp chính này lấy dữ liệu từ một ô cụ thể (B1) trong Google Sheet (Sheet1). 
Nếu có lỗi API, nó sẽ đợi một phút trước khi thử lại."""
while True:
    # Lấy một hình ảnh từ ô trong Google Sheet
    row = 2
    col = 1
    try:
        cell_data = worksheet.cell(row, col).value
    except gspread.exceptions.APIError as e:
        print(f"API Error: {e}")
        print("Waiting for a while before retrying...")
        time.sleep(60)  # Nghỉ 1 phút trước khi tiếp tục vòng lặp
        continue

    # Kiểm tra xem tập tin đã tồn tại hay chưa
    if os.path.exists(image_path):
        with open(image_path, 'rb') as f:
            # Đọc dữ liệu hình ảnh đã lưu trữ
            stored_image_binary = f.read()
        
        # Decode base64 dữ liệu hình ảnh từ Google Sheet
        if cell_data is not None:
            image_data = cell_data.split(",")[1]
            new_image_binary = base64.b64decode(image_data)
        
            # So sánh dữ liệu mới và dữ liệu đã lưu trữ
            if new_image_binary != stored_image_binary:
                # Lưu dữ liệu hình ảnh mới nếu chúng khác nhau
                with open(image_path, 'wb') as f:
                    f.write(new_image_binary)
                print("Đã tải về dữ liệu hình ảnh mới.")
                kq1=attack()
                chuoi_ket_qua =kq1
                success = worksheet.update_cell(2, 2, chuoi_ket_qua)
                print(f"Ghi dữ liệu vào ô F2: {success}")
            else:
                print("Dữ liệu hình ảnh không thay đổi.")
        else:
            # Nếu cell_data là None, thì xử lý theo cách phù hợp với ứng dụng của bạn
            print("Không có dữ liệu hình ảnh trong ô.")
    else:
        # Nếu tập tin không tồn tại, thực hiện tải về dữ liệu hình ảnh từ Google Sheet
        if cell_data is not None:
            image_data = cell_data.split(",")[1]
            image_binary = base64.b64decode(image_data)
            with open(image_path, 'wb') as f:
                f.write(image_binary)
            print("Đã tải về dữ liệu hình ảnh mới.")
            kq1=attack()
            chuoi_ket_qua =kq1
            success = worksheet.update_cell(2, 2, chuoi_ket_qua)
            print(f"Ghi dữ liệu vào ô F2: {success}")
        else:
            # Nếu cell_data là None, thì xử lý theo cách phù hợp với ứng dụng của bạn
            print("Không có dữ liệu hình ảnh trong ô.")

    # Nghỉ 5 giây giữa mỗi lần gọi API
    time.sleep(10)