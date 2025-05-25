doGet:
// Đảm bảo bạn thay đổi ID của Google Sheets cho phù hợp
var SS = SpreadsheetApp.openById('1yG19k1C7d2ZngNFQads9KEvVcU-N5020ZtvBXgFe8cg');
var sheet = SS.getSheetByName("Sheet1"); 

// Hàm xử lý dữ liệu gửi tới Google Sheets
function doGet(e) {
  // Lấy các tham số từ URL (ESP32 gửi tham số trong URL GET)
  var muc_nuoc = e.parameter.muc_nuoc;
  var mua = e.parameter.mua;
  var anh_sang = e.parameter.anh_sang;
  var nhiet_do = e.parameter.nhiet_do;
  var do_am_kk = e.parameter.do_am_kk;
  var do_am_dat = e.parameter.do_am_dat;

  
  // Ghi dữ liệu vào các ô trong Google Sheets (dòng mới)
  
sheet.getRange(2, 4).setValue(muc_nuoc);  // Cột K: Trạng thái (ring)
  sheet.getRange(2, 5).setValue(mua);  // Cột L: Điện áp (tilt)
  sheet.getRange(2, 6).setValue(anh_sang);  // Cột K: Trạng thái (ring)
  sheet.getRange(2, 7).setValue(nhiet_do);  // Cột K: Trạng thái (ring)
  sheet.getRange(2, 8).setValue(do_am_kk);  // Cột L: Điện áp (tilt)
  sheet.getRange(2, 9).setValue(do_am_dat);  // Cột K: Trạng thái (ring)

  var command = sheet.getRange("C2").getValue();
  var ai = sheet.getRange("B2").getValue();
  
  // Trả về dữ liệu dưới dạng JSON
  var result = {
    "command": command,
    "ai": ai,
  };
  
  // Trả về kết quả
  return ContentService.createTextOutput(JSON.stringify(result))
    .setMimeType(ContentService.MimeType.JSON);
}



doPost:
var SS = SpreadsheetApp.openById('1yG19k1C7d2ZngNFQads9KEvVcU-N5020ZtvBXgFe8cg');
var sheetname = SS.getSheetByName("Sheet1"); 
var str;
function doPost(e) {
// sheet name to publish data to is specified in Arduino code
var data=  e.parameter.image; // creates an array of the values to publish
sheetname.getRange('A2').setValue(data); // publish value0 from Arduino code to cell C2
str = "Success"; // string to return back to Arduino serial console

return ContentService.createTextOutput(str);
}// endif (parsedData !== undefined)
