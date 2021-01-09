# Visual studio Code Tips

visual studio code를 사용하면서, 필요한 내용들을 정리하려 합니다.



## ❌ 당황스러운 경우

#### 1. 에디터 창이 하나로 보이는 경우

control + cmd + w 로 토글을 합니다.



![vscode](https://user-images.githubusercontent.com/42775225/89252068-a8275100-d653-11ea-9ad7-eab2f298ef03.gif)





#### 2. 터미널에서 code . 이 안 먹히는 경우

```shell
vi .zshrc   # 맥 os 카탈리나 이후는 zshrc라고 합니당!
```

```shell
code () {
if [[ $# = 0 ]]
then
    open -a "Visual Studio Code"
else
    [[ $1 = /* ]] && F="$1" || F="$PWD/${1#./}"
    open -a "Visual Studio Code" --args "$F"
fi
}
```





## ✅ 추천하는 Extension

#### 1. prettier - code formatter

extension 검색 창에 prettier 를 입력해서 설치합니다. ![image](https://user-images.githubusercontent.com/42775225/87129880-fe130e00-c2cc-11ea-9cd0-9bec2721fadc.png)

그리고, settings에서 editor format on save 를 입력해서 체크 표시합니다. 그러면 저장할 때마다 예쁘게 포맷을 맞춰줍니다. 그러면 끝입니다. ![image](https://user-images.githubusercontent.com/42775225/87129850-f3f10f80-c2cc-11ea-8782-5bd1228d0f78.png)

***(여기는 안하셔도 되요!)\* 이렇게 하면 끝이지만, settings에서 json 을 입력하고 json파일에서 직접 수정도 가능합니다. ![image](https://user-images.githubusercontent.com/42775225/87130168-6f52c100-c2cd-11ea-8f32-d52ac2beb267.png)**

**![image](https://user-images.githubusercontent.com/42775225/87131042-cad17e80-c2ce-11ea-842d-980bb6a7795c.png)**



#### **2. material theme : mattia astorino**

**이건 그냥 설치만 하면 됩니다.**







## ✅ 저만의 커스터마이징

### 1. 단축키 지정

**code --> preference --> keyboard shorcuts**



#### 1) 대소문자

- To uppercase : cmd + k  
- To lowercase : cmd + l

둘다 기존에 단축키가 다른 것으로 설정되어 있기는 하지만, 둘다 사용하지 않는 단축키라서 덮어씌웠습니다.

![image](https://user-images.githubusercontent.com/42775225/89746797-a3044f00-daf6-11ea-9966-e8405d6d09e4.png)



#### 2) 창 이동

아톰이나 다른 에디터에서는 바로 됐었던게 **cmd + 숫자** 로 에디터 이동하는 거였는데, 그게 vscode에서는 **control + 숫자** 로 설정되어 있습니다. 그래서 바꿔줍니다.

- workbench.action.openEditorAtIndex : cmd + 숫자

![image](https://user-images.githubusercontent.com/42775225/89746783-954ec980-daf6-11ea-94e6-85306136f274.png)



에디터를 많이 켜놓게 되면, 왼쪽, 오른쪽으로도 이동하고 싶어서 다음도 수정하였습니다.

- View: Open Next Editor : option + cmd + k
- View: Open Previous Editor : option + cmd + j

원래는 방향키인데, j, k로 하는게 더 편해서 바꾸었습니다.



#### 3) 터미널

- terminal: create new : cmd + t



### 2. 폰트변경

iterm에서 세팅했던대로 vscode에서 호환이 되어서 좋지만, oh my zsh 사용으로 vscode의 터미널이 깨지는 현상이 있을 수 있기 때문에 폰트를 변경합니다. (저는 iterm에서 D2Coding 사용 중)

**code --> preference --> settings --> font**

![image](https://user-images.githubusercontent.com/42775225/90868193-75aa8180-e3d1-11ea-8023-c30b700c3e4b.png)



### 3. html 자동 완성

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

   





## 터미널 default 바꾸기 (윈도우 기준)

settings -> integrated terminal

<img src="https://user-images.githubusercontent.com/42775225/104085430-ba588b80-5292-11eb-8fe3-dd61ff4c2d10.png" alt="image" style="zoom:67%;" />

<img src="https://user-images.githubusercontent.com/42775225/104085449-d9571d80-5292-11eb-9dcc-add027775898.png" alt="image" style="zoom:70%;" />