# Thymeleaf 정리

지금의 정리는 <a href="https://www.thymeleaf.org/doc/tutorials/3.0/usingthymeleaf.html" target="_blank"> thymeleaf 공식 문서 </a>를 참고하였습니다.



스프링 프레임워크의 View에 사용됩니다. 기존에 대표적으로 사용되는 jsp와 thymeleaf가 어떻게 다른지 코드로 확인해보겠습니다.

<br>

##### *jsp*

```html
<form:inputText name="userName" value="${user.name}" />
```

공식 문서의 설명으로는 jsp로 했을 때는, 브라우저에서 표시할 수 없는 코드가 포함될 수 있다고 합니다.

<br>

##### *thymeleaf*

```html
<input type="text" name="userName" value="James Carrot" th:value="${user.name}" />
```

익숙한 문법으로 디자이너와 협업할 때도 장점이 있습니다.

<br>

<br>

## 텍스트 사용하기

#### 1) Variable Expressions: `${...}`

```html
<p>Today is: <span th:text="${today}">13 february 2011</span></p>
```

서버에서 `today` 값을 넘겨주면, `13 february 2011` 가 넘겨받은 `today` 값으로 대체됩니다.



<br>

#### 2) Selection Variable Expressions: `*{...}`

```html

```

객체의 속성 값을 나타낼 수 있습니다. 



<br>

#### 3) Message Expressions: `#{...}`

```html
<p th:text="#{home.welcome}">Welcome to our grocery store!</p>
```

여러 나라의 언어로 텍스트를 지원하고 싶을 때, 위처럼 `home.welcome` 에 여러 나라 버전으로 텍스트를 준비해두면, 다국어를 지원할 수 있습니다. 

cf) 만약에 `tag` 안에 또 `tag` 를 넣고 싶다면, `th:text` 대신 `th:utext` 를 사용하면 안전하게 사용할 수 있습니다.



<br>

#### 4) Link URL Expressions: `@{...}`

```html
<link rel="stylesheet" type="text/css" media="all" 
          href="../../css/gtvg.css" th:href="@{/css/gtvg.css}" />
```

html파일을 그냥 열면 `href` 가 적용되고, 서버에서는 `th:href` 를 사용합니다.

```html
<a href="details.html" th:href="@{/order/details(orderId=${o.id})}">view</a>
```

그리고 위와 같이 파라미터도 넣을 수 있다.



<br>

#### 5) Fragment Expressions: `~{...}`

```html

```

레이아웃을 짤 때, 중복되는 코드를 이것으로 막을 수 있습니다. 



<br>

<br>

## if, for문

#### 1) if문

```html
<div th:if="${user.isAdmin()} == false"> ...
```

if문이 참이면 `div` 가 살아있고, 아니면 없어집니다.

```html
<div th:if="${variable.something} == null"> ...
```

`null` 체크도 가능합니다.



#### 







<br>

#### 2) 





<br>

<br>

## 연산자

`gt` (`>`), `lt` (`<`), `ge` (`>=`), `le` (`<=`), `not` (`!`). Also `eq` (`==`), `neq`/`ne` (`!=`).







<br>

<br>

## 각종 팁

#### 1) 문자열 `|` 이용하기

```html
<span th:text="'Welcome to our application, ' + ${user.name} + '!'">
```

위와 같은 경우, `|` 을 이용해서 다음과 같이 사용할 수 있다.

```html
<span th:text="|Welcome to our application, ${user.name}!|">
```





<br>

#### 2) 네이밍을 할 때 `부모-자식` 관계를 다음과 같이 나타낼 수 있다.

##### ☝ `th:object` 사용

```html
<div th:object="${session.user}">
   <p>Name: <span th:text="*{firstName}">Sebastian</span>.</p>
   <p>Surname: <span th:text="*{lastName}">Pepper</span>.</p>
   <p>Nationality: <span th:text="*{nationality}">Saturn</span>.</p>
</div>
```

##### ✌ `th:object` 미 사용

```html
<div>
   <p>Name: <span th:text="${session.user.firstName}">Sebastian</span>.</p>
   <p>Surname: <span th:text="${session.user.lastName}">Pepper</span>.</p>
   <p>Nationality: <span th:text="${session.user.nationality}">Saturn</span>.</p>
</div>
```

