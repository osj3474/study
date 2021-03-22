# Mac tips

## 1) 기본 프로그램 설정

![image](https://user-images.githubusercontent.com/42775225/80672036-819eca80-8ae6-11ea-8dad-8dba6165b1c7.png)



## 2) USB 키보드 사용

![image](https://user-images.githubusercontent.com/42775225/80672171-ebb76f80-8ae6-11ea-9707-9e3351d5a49b.png)



## 3) 마우스로 데스크탑 보기 설정

![image](https://user-images.githubusercontent.com/42775225/80672226-11447900-8ae7-11ea-9f91-028a76a6f9f6.png)





## 4) 강제종료

![image](https://user-images.githubusercontent.com/42775225/81674204-c6801500-9487-11ea-9b07-e58b218a5b38.png)





## 5) 이모티콘

MacOS : `ctrl + cmd + space bar`

Windows : `window key + semi colon || window key + . (period)`



## 6) 한글 폴더명 분리되는 경우

예시)

![image](https://user-images.githubusercontent.com/42775225/91250763-c5ec5f80-e794-11ea-8a70-7a630ccf7e5d.png)

해결법) iterm2 -> preferences -> profile -> text -> Unicode normalization form: NFC

![image](https://user-images.githubusercontent.com/42775225/91250828-eb796900-e794-11ea-95f3-f63f32eea7a8.png)





## 7) 맨 위의 쓸데없는 아이콘 버리는 방법

![image](https://user-images.githubusercontent.com/42775225/96942822-2ab40400-1511-11eb-97bf-1c46a7ac8782.png)

cmd 누른 상태로, 아이콘 잡고 바탕화면에 드래그 앤 드롭하면 됩니다.







## 8) esc 밑에 `대신 ₩가 입력될 때

1. ```
   vi ~/Library/KeyBindings/DefaultkeyBinding.dict
   ```

   (디렉토리나 파일이 없다면, 만들면 됩니다.)

2. ```
   {
       "₩" = ("insertText:", "`");
   }
   ```

   

