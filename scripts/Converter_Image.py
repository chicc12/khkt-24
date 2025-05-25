import base64
from PIL import Image
import io

def resize_and_convert_to_base64(image_path, target_size=(512, 512)):
    with Image.open(image_path) as img:
        img_resized = img.resize(target_size, Image.LANCZOS)
        
        buffered = io.BytesIO()
        img_resized.save(buffered, format="PNG")
        img_byte = buffered.getvalue()
        img_base64 = base64.b64encode(img_byte).decode("utf-8")
        
        return img_base64

# Đường dẫn tới ảnh cần chuyển đổi, sử dụng chuỗi raw string hoặc thoát các ký tự backslash
image_path = r"C:\Users\PC\Downloads\fire_dataset-20240528T054427Z-001\fire_dataset\fire_images\fire.411.png"
# Gọi hàm và in kết quả
image_base64 = resize_and_convert_to_base64(image_path)
print(image_base64)
