# Node.js

* 생활코딩으로 공부한거 정리한 것입니다. OS환경에 따른 설치는 좋은 블로그들이 많아서 생략합니다. 저는 다음과 같이 진행하였습니다.

  ```bash
  curl https://raw.githubusercontent.com/creationix/nvm/v0.30.2/install.sh | bash
  쉘 재실행
  nvm install stable
  ```

  

* 터미널에서 사용되는 기본 명령어

  1. 파일 실행

  ```bash
  node main.js  // 실행하고자 하는 파일이 main.js인 경우
  ```

  

 



## 문법

### 1. Template literal ` (그레이브 엑센트)

<img width="306" alt="스크린샷 2020-02-01 오후 8 46 39" src="https://user-images.githubusercontent.com/42775225/73591606-fbe5a000-4533-11ea-8e47-52f84a823643.png">

 : esc 밑에 있는 `로 string을 감싸면, 

1. 변수를 ${}로 감싸서 사용할 수 있다. 
2. '\n' 대신 엔터를 치면 된다.
3. html코드를 그대로 사용할 수 있다.

```javascript
var name = 'oh';
var str = `My name is ${name}`;
```



### 2. URL

<img width="784" alt="스크린샷 2020-02-01 오후 9 13 43" src="https://user-images.githubusercontent.com/42775225/73591903-c0e56b80-4537-11ea-912b-56abe008268e.png">

### 

### 3. File System

#### 1) fs.readFile(path[, options], callback)

##### [폴더 구조]

<img width="426" alt="스크린샷 2020-02-01 오후 10 00 34" src="https://user-images.githubusercontent.com/42775225/73592482-571c9000-453e-11ea-95cd-433d0d500da8.png">

##### main.js

```javascript
var fs = require('fs');
fs.readFile('sample.txt', 'utf8', (err, data) => {
  console.log(data);
})
```





### 4. 연산자

 `==`  : Equal Operator 

예) `a==b` 의 경우는 `a`와 `b`의 '값'이 같은지를 판단한다.

`===` : Strict Equal Operator

예) `a===b `의 경우는 `a`와 `b`의 '값'과 '타입'이 정확하게 같은지를 판단한다.

##### ps) 되도록이면 비교 연산자는 === 를 사용합시다.



[ 전체코드 ]

```javascript
var http = require('http');
var fs = require('fs');
var url = require('url');

app.createServer(function(req, res){
  var _url = req.url;
  var queryData = url.parse(_url, true).query; // url모듈의 parse메서드로 return값은 query string
  if(_url == '/'){
    _url = '/index.html';
  }
  if(_url == '/favicon.ico'){
    return res.writeHead(404);
  }
  res.writeHead(200);
  res.end(fs.readFileSync(__dirname+_url)); 
  // end메서드의 파라미터로 html코드를 담은 변수를 줘도 된다. 지금은 사용자가 접속한 url에 따라서 파일을 읽고 있다.
  
})
app.listen(3000)  // 3000포트 대기 중
```



