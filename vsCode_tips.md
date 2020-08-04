# Visual studio Code Tips

visual studio code를 사용하면서, 필요한 내용들을 정리하려 합니다.



## 당황스러운 경우

#### 1. 에디터 창이 하나로 보이는 경우

control + cmd + w 로 토글을 합니다.



![vscode](https://user-images.githubusercontent.com/42775225/89252068-a8275100-d653-11ea-9ad7-eab2f298ef03.gif)







## 추천하는 Extension

#### 1. prettier - code formatter

extension 검색 창에 prettier 를 입력해서 설치합니다. ![image](https://user-images.githubusercontent.com/42775225/87129880-fe130e00-c2cc-11ea-9cd0-9bec2721fadc.png)

그리고, settings에서 editor format on save 를 입력해서 체크 표시합니다. 그러면 저장할 때마다 예쁘게 포맷을 맞춰줍니다. 그러면 끝입니다. ![image](https://user-images.githubusercontent.com/42775225/87129850-f3f10f80-c2cc-11ea-8782-5bd1228d0f78.png)

***(여기는 안하셔도 되요!)\* 이렇게 하면 끝이지만, settings에서 json 을 입력하고 json파일에서 직접 수정도 가능합니다. ![image](https://user-images.githubusercontent.com/42775225/87130168-6f52c100-c2cd-11ea-8f32-d52ac2beb267.png)**

**![image](https://user-images.githubusercontent.com/42775225/87131042-cad17e80-c2ce-11ea-842d-980bb6a7795c.png)**



#### **2. material theme : mattia astorino**

**이건 그냥 설치만 하면 됩니다.**







## 저만의 커스터마이징

#### 1. 단축키 지정

**code --> preference --> keyboard shorcuts --> trans검색 --> 다음과 같이 지정**

- **to uppercase : cmd + k**  
- **to lowercase : cmd + l**

**둘다 기존에 단축키가 다른 것으로 설정되어 있기는 하지만, 둘다 사용하지 않는 단축키라서 덮어씌웠습니다.**



#### 2. html 자동 완성

1. html: 를 치고 엔터를 누르면, 바로 기본 세팅 완료.

2. div-class 쉽게 생성하기 (다음의 경우 .box 엔터치면 끝. (\*2 하고 엔터도 가능함.))

   ```html
   <div class="box"></div>
   ```

3. span-class 쉽게 생성하기 (다음의 경우 span.box 엔터치면 끝.)

   ```html
   <span class="box"></span>
   ```

4. li.nav\_\_list-item\*4>a.nav\_\_list-link를 치면

   ```html
   <li class="nav__list-item"><a href="|" class="nav__list-link">|</a></li>
   ```

   

## 