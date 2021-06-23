# Hiện tượng lỗi

Ổ cứng khi đang được gắn trên cổng SATA trong laptop, khi bật laptop lên sẽ gặp tình trạng treo , đơ ở logo của máy . Có nghĩa là Power On Self Test gọi tắt là POST đang bị treo (POST là gì ?) . Và tất nhiên là ta sẽ không thể thao tác bất cứ việc gì, ví dụ bấm Del, F11, F2 để chọn thiết bị boot hay vào vào setup BIOS . Sau đó mình thử tắt nóng bằng cách giữ phím nguồn trong 5 giây để thực hiện tắt nóng , Đối với một số máy, ví dụ như HP hay Dell thì đèn Caplock vẫn sáng ??? Mình tưởng là lỗi nguồn hay gì đó, rồi mình tháo nguồn và pin laptop ra thì đèn Caplock cũng đã tắt, đợi 2 phút sau cắm pin vào khởi động lên nhưng vẫn bị lỗi , lần này thì nó không hiện lên LOGO của máy như lần đầu mà màn hình đen xì, đèn Caplock và Numlock nháy một lúc , lại thực hiện tắt nóng, rút nguồn mà vẫn không được

Vì vậy mà mình tháo pin và nguồn, sau đó tháo ổ cứng đang gắn trên cổng SATA trên laptop . Đợi 2 phút sau lắp lại rồi bật lên nhưng vẫn bị cái lỗi này ??? . Ổ cứng bị lỗi tuy đã được tháo ra rồi nhưng tại sao lại vẫn không thể bật máy lên ??? Mọi phần cứng trong laptop đều bình thường, không có hiện tượng hỏng hóc ở đây cả.

- Chiếc ổ cứng được tháo ra và mình sẽ kiếm một cái HDD BOX rồi cho vào , còn cái máy tính đang bị lỗi kia thì mình sẽ chỉ cách phục ở bên dưới , tạm thời ta quan tâm tới cái ổ cứng đã . Tiếp theo mình sẽ cắm cái BOX này vào một cái máy tính đang bình thường, khi cắm vào sau đó khởi động lên thì vẫn bị treo , tuy nhiên thì sau khi tắt nóng và rút BOX ra khỏi máy, bật lại thì máy vẫn lên . Sau khi vào được WinPE , mình cắm lại cái BOX thì thỉnh thoảng nó vẫn sẽ nhận drivers , tức là nó vẫn hiện tên thiết bị, tên của ổ cứng

Click image for larger version  Name: 1.PNG Views: 1 Size: 2,7 KB ID: 22372763

- Tuy nhiên khi vào các công cụ phân vùng như Disk Management, Parition Wizard, Acronis Disk Director, AOMEI, Partition GURU v.v... thì nó lại không nhìn được cái ổ cứng này để mà thao tác .
Việc gây ra lỗi này có thể do virus, các công cụ phân vùng cấp thấp (phân vùng bằng dòng lệnh) thực hiện sai một vài thao tác v.v... . Hãy tìm hiểu nguyên nhân nào !!!

# Nguyên nhân gây lỗi

Quá trình khởi động của máy tính đầu tiên phải qua một trình kiểm tra gọi là Power On Self Test hay POST , Về cơ bản là nó sẽ thực hiện chức năng test phần cứng -> kiểm tra hệ thống boot (MBR, EFI, UEFI with CSM) -> Tìm trên thiết bị lưu trữ file boot , chương trình boot . Việc BIOS bị treo là do POST vẫn còn đang chạy . Công đoạn test phần cứng có lẽ ổn rồi, công đoạn kiểm tra hệ thống boot thì ta đang nói về ổ cứng định dạng MBR bị lỗi, nghĩa là nó sẽ chọn hệ thống boot là MBR 

Tiếp đến nó sẽ tìm đến file boot ( GRUB4DOS, BOOTMBR, NTLDR, SYSLINUX) . Vậy để tìm được file boot thì có nghĩa là hệ thống POST có chức năng liệt kê các phân vùng, liệt kê cấu trúc thư mục và file trên từng phân vùng thì mới có thể tìm được file boot và khởi chạy code của file boot đó.

Quay lại với vấn đề ổ cứng bị lỗi. Chúng ta được biết rằng ổ cứng định dạng MBR có thể chứa tối đa 4 phân vùng PRIMARY hoặc 3 phân vùng PRIMARY và nhiều phân vùng LOGICAL . Sau một vài ngày tìm hiểu thì mình biết được là nguyên nhân gây lỗi xảy ra ở CÁC Ổ CỨNG ĐỊNH DẠNG MBR CÓ 2 PHÂN VÙNG LOGICAL TRỞ LÊN . Ở đây mình không có ổ cứng thật nên mình sẽ dùng ổ cứng ảo VHD trên Windows để test, các bạn có thể kiểm chứng bằng ổ cứng thật nếu có .

Mình có viết sẵn một file thực thi để tạo ra lỗi này, các bạn có thể pull git này về và test trên ổ cứng thật của máy.

# PHÂN TÍCH LỖI

OK , Bây giờ mình sẽ tiến hành phân vùng cái ổ cứng này, mình sẽ chia cho nó 2 phân vùng PRIMARY và 2 phân vùng LOGICAL
Ổ Cứng này sẽ có dung lượng là 320 GB :
Click image for larger version  Name: 2.PNG Views: 1 Size: 50,1 KB ID: 22372764
Chúng ta có 4 phân vùng như hình bên trên . Trước tiên hãy tìm hiểu sơ qua một chút về cách phân vùng trên ổ cứng định dạng MBR nhé, dùng phần mềm BOOTICE để kiểm thử.

Ổ cứng tổ chức thành các sector vòng cung liên tiếp nhau trên mặt đĩa, mỗi sector được định dạng là 512 Byte , tổng số dung lượng ổ cứng được tính bằng tổng số sector x 512 . Định dạng MBR sẽ sử dụng sector đầu để lưu trữ thông tin về chương trình mồi (nạp boot GRUB4DOS, NT 5X, NT 6X sẽ được thao tác trên sector này) và thông tin về 4 phân vùng đầu tiên.

1. Hãy mở BOOTICE -> chọn ổ cứng -> chọn Sector Edit

Click image for larger version  Name: 3.PNG Views: 1 Size: 36,6 KB ID: 22372765

1. Chọn tiếp MBR như hình dưới để xem dữ liệu trên SECTOR 0

Click image for larger version  Name: 4.png Views: 1 Size: 79,7 KB ID: 22372766

Xuyên suốt bài viết , cứ mỗi lần nhảy sang một BOOT SECTOR mới thì chúng ta hãy vẫn chỉ quan tâm đến đoạn dữ liệu được bôi đen ở trên thôi nhé . Cấu trúc của các BOOT SECTOR thường có chữ ký hệ thống ở cuối là 55 AA ở cuối chỉ định SECTOR đó là BOOT SECTOR, BOOT SECTOR đầu tiên , SECTOR 0 (MASTER BOOT SECTOR) thì đoạn dữ liệu ở phía trên phần mình đã bôi đen tức là 446 Byte đầu tiên chính là mã của những loại BOOT chúng ta hay nạp, GRUB4DOS SYSLINUX hay NT 6X, NT5X , đoạn bôi đen gồm 64 Byte (loại bỏ cái chữ ký hệ thống 55 AA kia đi) là thông tin về 4 phân vùng đầu tiên , vậy mỗi phân vùng sẽ có 16 Byte để lưu thông tin từng phân vùng . Nếu bạn nào có kiến thức vững chắc về hệ cơ số và biểu diễn dữ liệu máy tính thì có thể đọc qua bài viết này để hiểu sâu hơn , tạm thời mình sẽ chỉ nói qua thôi nhé.

https://technet.microsoft.com/en-us/.../cc976786.aspx

1. Hai phân vùng PRIMARY đầu tiên sẽ là 32 Byte , Phân vùng thứ 3 sẽ là phân vùng EXTENDED, khi các bạn phân vùng thì để tạo ra phân vùng LOGICAL , người ta sẽ tạo ra một vùng EXTENDED, nó sẽ trỏ tới một BOOT SECTOR khác , BOOT SECTOR này sẽ vẫn dùng 64 Byte ở gần cuối để lưu thông tin phân vùng tiếp theo, tuy nhiên nó chỉ dùng 32 Byte để lưu thông tin phân vùng , 16 Byte đầu sẽ dùng để lưu vị trí SECTOR bắt đầu để lưu dữ liệu , còn 16 Byte tiếp theo sẽ được dùng để trỏ tới BOOT SECTOR khác , "BOOT SECTOR khác" này chính là phân vùng LOGICAL thực chất là EXTENDED tiếp theo được tạo ra, và nếu không có phân vùng EXTENDED nào được tạo ra tiếp theo thì nó chỉ dùng 16 Byte để lưu vị trí SECTOR bắt đầu lưu dữ liệu thôi.

   Click image for larger version  Name: 5.png Views: 1 Size: 77,1 KB ID: 22372767

1. Chuyến tiếp sang phân vùng LOGICAL thứ hai

   Click image for larger version  Name: 6.png Views: 1 Size: 81,0 KB ID: 22372768

## Mình sẽ tạo ra lỗi bằng cách sau :

1. Copy 16 Byte trỏ tới phân vùng LOGICAL thứ 2
1. Nhảy đến BOOT SECTOR chứa phân vùng LOGICAL cuối cùng
1. Copy nó ở sau 16 Byte đầu tiên , sau đó lưu lại BOOT SECTOR này và hiện tượng lỗi sẽ xảy ra ngay lập tức, có thể khởi động lại máy để kiểm tra

   Click image for larger version  Name: 7.png Views: 1 Size: 65,5 KB ID: 22372769

   Click image for larger version  Name: 8.png Views: 1 Size: 73,2 KB ID: 22372770

# NGUYÊN NHÂN GÂY LỖI VÀ CÁCH KHẮC PHỤC :

NGUYÊN NHÂN CHÍNH GÂY RA LỖI XẢY RA VỚI Ổ CỨNG CÓ HAI PHÂN VÙNG LOGICAL TRỞ LÊN, ĐỊNH DẠNG Ổ CỨNG MBR LÀ KIỂU CON TRỎ, MỘT BOOT SECTOR CỦA PHÂN VÙNG LOGICAL VẪN DÙNG 64 BYTE GẦN CUỐI ĐỂ LƯU THÔNG TIN PHÂN VÙNG, TUY NHIÊN THÌ CHỈ CÓ 32 BYTE ĐẦU Ở ĐOẠN 64 BYTE ĐÓ ĐỂ LƯU THÔNG TIN PHÂN VÙNG, 16 BYTE ĐẦU ĐƯỢC TRỎ TỚI VÙNG CHỨA DỮ LIỆU, 16 BYTE TIẾP THEO ĐƯỢC TRỎ TỚI PHÂN VÙNG LOGICAL TIẾP THEO NẾU CÓ, NẾU KHÔNG CÓ THÌ 16 BYTE TIẾP THEO NÀY ĐƯỢC GHI LÀ 00 HẾT . KHI 16 BYTE NÀY ĐƯỢC GHI VỊ TRÍ CỦA BOOT SECTOR CỦA PHÂN VÙNG LOGICAL THỨ 2 THÌ CÁC HỆ THỐNG ĐỌC PHÂN VÙNG SẼ QUAY NGƯỢC LẠI ĐỌC , GÂY RA MỘT VÒNG LẶP VÔ TẬN KHÔNG CÓ ĐIỂM DỪNG.
HỆ THỐNG POST TRÊN BIOS BỊ LỖI NÀY GÂY RA, VÌ VẬY NÓ SẼ ĐỌC THEO CÁI DỮ LIỆU "LỖI" NÀY, KẾT QUẢ LÀ NÓ CỨ LẶP ĐI LẶP LẠI VIỆC ĐỌC CÁC PHÂN VÙNG NÀY, LÚC TẮT MÁY THÌ HỆ THỐNG POST VẪN HOẠT ĐỘNG, CHO NÊN VIỆC KHỞI ĐỘNG MÁY MÀ KHÔNG HIỆN LOGO CHÍNH LÀ NÓ VẪN ĐANG BỊ RƠI VÀO TRONG CÁI VÒNG LẶP NÀY.

## KHẮC PHỤC MÁY TÍNH BỊ TREO

THÁO PIN CMOS TRÊN MÁY LAPTOP, SAU ĐÓ GIỮ PHÍM NGUỒN TRONG 5 GIÂY, ĐỢI 5 PHÚT SAU LẮP LẠI PIN CMOS VÀ BẬT LẠI MÁY

## KHẮC PHỤC Ổ CỨNG BỊ LỖI

LẮP HDD VÀO BOX, SAU ĐÓ TẢI FILE ISO NÀY VỀ (BÊN TRONG ĐÃ CÓ SẴN CODE BOOT)

https://drive.google.com/file/d/0B6Q...ew?usp=sharing

> Phần mềm Partition Wizard phiên bản Bootable chạy trên Linux, nó chỉ chạy dựa vào nhân Linux mà không có bất cứ phần mềm nào thực hiện việc đọc cái ổ cứng lỗi này, tức là nhân Linux chỉ thực hiện load driver cho ổ cứng, còn việc đọc là do phần mềm thực hiện, PARTED MAGIC cũng chạy trên nhân Linux nhưng các phần mềm của nó lại thực hiện việc đọc cái ổ cứng lỗi này, ví dụ File Browser, Gparted. Nên nó sẽ bị treo.

1. Sau khi tải về các bạn tiến hành tạo boot file ISO trên USB.
1. Cắm USB Boot đã tạo và bật máy , sau đó boot vào Parititon Wizard

   Click image for larger version  Name: 9.PNG Views: 1 Size: 530,4 KB ID: 22372775

1. Sau khi xong thì cắm BOX vào máy, bấm F5 và đợi một lúc, chúng ta sẽ lại thấy chiếc ổ cứng vẫn còn y nguyên các phân vùng, lúc chưa lỗi, thực chất đây là một phần mềm có phí, chức năng sửa lỗi phát hiện lỗi cực kỳ tốt, nên khi gặp đoạn bị lặp nó phát hiện và dừng đọc ngay lập tức, và vẫn hiện các phân vùng như bình thường . Khi bạn tạo một thay đổi nhỏ, ví dụ Set Active hay Unset active phân vùng BOOT, Resize lại kích thước của một phân vùng bất kỳ hay thay đổi tên của phân vùng, bất kỳ thay đổi nào, Partition Wizard sẽ tự động ghi lại hệ thống MBR, khi ghi lại thì đoạn dữ liệu trỏ ngược lại về phân vùng LOGICAL gây vòng lặp sẽ được loại bỏ và không được ghi vào .

Đây chỉ một trong nhiều lỗi có thể gây ra các hiện tượng lỗi ổ cứng và ảnh hưởng đến các thiết bị khác, hy vọng qua bài viết này sẽ giúp mọi người hiểu hơn về cách tổ chức của hệ thống MBR, thêm một kinh nghiệm quý báu khi cứu dữ liệu ổ cứng . HỆ THỐNG GPT/UEFI thực sự có thể thay thế chuẩn MBR hiện tại, cấu trúc khác biệt và an toàn hơn so với MBR , mình sẽ viết một bài chi tiết về hệ thống GPT / UEFI, cấu trúc, cách phân vùng hợp lý và cách cài đặt các hệ điều hành Windows, Linux, cài song song . Mong mọi người ủng hộ và thảo luận nhiều hơn .
