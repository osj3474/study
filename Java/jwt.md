구글로 로그인

OAuth2는 어플리케이션 기술로 구현할 수 있다.

accessToken이 인증 역할을 한다.

필요한 용어

- Resource Server & Authorization Server : 구글
- Resource Owner : 사용자
- Client : 우리 서비스

 

우리가 실제 등록하려면,

- client ID
- client secret
- authorized redirect URIs



인증 과정은,

Resource Server 는 client ID, client secret, redirect url 정보를 가지고 있음.

 









Spring Security가 지원하는 OAuth2.0의 토큰 방식은 

1. JWT
2. Opaque Tokens

이다.





1. 내가 개발하려는 것은 jwt를 확인하고 admin이면, 관리자 페이지 열어주는 것임.

2. request header에 jwt가 있는지 확인하고 get메서드 처리하는 부분을 할 것임.

3. 그 전에, 토큰이 올바른지 검증하는 로직이 필요함.

4. spring security의 경우, dispatcherServlet 앞단에 filter를 두고, 이 filter로 권한에 따른 포워딩을 해줌.

5. 애초에 토큰 자체를 생성하는 jwtProvider랑 권한 검증을 위한 filter의 존재가 선행되어야 함.

   (그리고 filter안에는 REST api이기 때문에 인증 권한 없다는 오류 JSON 내보내주는 처리를 filter에 넣어야 함.)

6. 이렇게 된 이상 다음 클래스를 먼저 생성할 예정

   - JwtTokenProvider : 토큰을 생성
   - JwtAuthenticationFilter : 권한을 검증
   - SecurityConfiguration : 권한에 따른 리소스 접근 결정





spring-boot-starter-security를 추가하면,

기본적으로 

1) 모든 페이지는 인증이 필요해지고,

2) 기본 유저가 생기도록 해준다.



WebSecurityConfigurerAdapter를 상속받은 config를 정의해주는 순간, 부트가 제공하는 securityAutoConfiguration은 적용이 안됨.

여기서 페이지별 권한 설정을 해주면 됨.



userdetailsService를 재정의해줘야 한다. 

@Service에 implements UserDetailsService 로 설정을 해주면 된다. 그러면 더이상 스프링이 임시 user를 생성해주지 않는다.

핵심이 되는 메서드는 loadUserByUsername인데, id에 해당하는 string값을 받아서 사용자가 입력한 비밀번호와 repo에 가지고 있는 비밀번호가 맞는지 확인하여 예외를 던지거나 로그인 처리를 해준다.



security가 passwordEncoding을 요구한다. 









### JwtTokenProvider

```java
package com.rest.api.config.security;

// import 생략

@RequiredArgsConstructor
@Component
public class JwtTokenProvider { // JWT 토큰을 생성 및 검증 모듈

    @Value("spring.jwt.secret")
    private String secretKey;

    private long tokenValidMilisecond = 1000L * 60 * 60; // 1시간만 토큰 유효

    private final UserDetailsService userDetailsService;

    @PostConstruct
    protected void init() {
        secretKey = Base64.getEncoder().encodeToString(secretKey.getBytes());
    }

    // Jwt 토큰 생성
    public String createToken(String userPk, List<String> roles) {
        Claims claims = Jwts.claims().setSubject(userPk);
        claims.put("roles", roles);
        Date now = new Date();
        return Jwts.builder()
                .setClaims(claims) // 데이터
                .setIssuedAt(now) // 토큰 발행일자
                .setExpiration(new Date(now.getTime() + tokenValidMilisecond)) // set Expire Time
                .signWith(SignatureAlgorithm.HS256, secretKey) // 암호화 알고리즘, secret값 세팅
                .compact();
    }

    // Jwt 토큰으로 인증 정보를 조회
    public Authentication getAuthentication(String token) {
        UserDetails userDetails = userDetailsService.loadUserByUsername(this.getUserPk(token));
        return new UsernamePasswordAuthenticationToken(userDetails, "", userDetails.getAuthorities());
    }

    // Jwt 토큰에서 회원 구별 정보 추출
    public String getUserPk(String token) {
        return Jwts.parser().setSigningKey(secretKey).parseClaimsJws(token).getBody().getSubject();
    }

    // Request의 Header에서 token 파싱 : "X-AUTH-TOKEN: jwt토큰"
    public String resolveToken(HttpServletRequest req) {
        return req.getHeader("X-AUTH-TOKEN");
    }

    // Jwt 토큰의 유효성 + 만료일자 확인
    public boolean validateToken(String jwtToken) {
        try {
            Jws<Claims> claims = Jwts.parser().setSigningKey(secretKey).parseClaimsJws(jwtToken);
            return !claims.getBody().getExpiration().before(new Date());
        } catch (Exception e) {
            return false;
        }
    }
}
```





### JwtAuthenticationFiler

```java
package com.rest.api.config.security;

// import 생략

public class JwtAuthenticationFilter extends GenericFilterBean {

    private JwtTokenProvider jwtTokenProvider;

    // Jwt Provier 주입
    public JwtAuthenticationFilter(JwtTokenProvider jwtTokenProvider) {
        this.jwtTokenProvider = jwtTokenProvider;
    }

    // Request로 들어오는 Jwt Token의 유효성을 검증(jwtTokenProvider.validateToken)하는 filter를 filterChain에 등록합니다.
    @Override
    public void doFilter(ServletRequest request, ServletResponse response, FilterChain filterChain) throws IOException, ServletException {
        String token = jwtTokenProvider.resolveToken((HttpServletRequest) request);
        if (token != null && jwtTokenProvider.validateToken(token)) {
            Authentication auth = jwtTokenProvider.getAuthentication(token);
            SecurityContextHolder.getContext().setAuthentication(auth);
        }
        filterChain.doFilter(request, response);
    }
}
```





### SecurityConfiguration

```java
package com.rest.api.config.security;

// import 생략

@RequiredArgsConstructor
@Configuration
public class SecurityConfiguration extends WebSecurityConfigurerAdapter {

    private final JwtTokenProvider jwtTokenProvider;

    @Bean
    @Override
    public AuthenticationManager authenticationManagerBean() throws Exception {
        return super.authenticationManagerBean();
    }

    @Override
    protected void configure(HttpSecurity http) throws Exception {
        http
            .httpBasic().disable() // rest api 이므로 기본설정 사용안함. 기본설정은 비인증시 로그인폼 화면으로 리다이렉트 된다.
            .csrf().disable() // rest api이므로 csrf 보안이 필요없으므로 disable처리.
            .sessionManagement().sessionCreationPolicy(SessionCreationPolicy.STATELESS) // jwt token으로 인증하므로 세션은 필요없으므로 생성안함.
            .and()
                .authorizeRequests() // 다음 리퀘스트에 대한 사용권한 체크
                    .antMatchers("/*/signin", "/*/signup").permitAll() // 가입 및 인증 주소는 누구나 접근가능
                    .antMatchers(HttpMethod.GET, "helloworld/**").permitAll() // hellowworld로 시작하는 GET요청 리소스는 누구나 접근가능
                    .anyRequest().hasRole("USER") // 그외 나머지 요청은 모두 인증된 회원만 접근 가능
            .and()
                .addFilterBefore(new JwtAuthenticationFilter(jwtTokenProvider), UsernamePasswordAuthenticationFilter.class); // jwt token 필터를 id/password 인증 필터 전에 넣는다

    }

    @Override // ignore check swagger resource
    public void configure(WebSecurity web) {
        web.ignoring().antMatchers("/v2/api-docs", "/swagger-resources/**",
                "/swagger-ui.html", "/webjars/**", "/swagger/**");

    }
}
```





### CustomUserDetailService

```java
@RequiredArgsConstructor
@Service
public class CustomUserDetailService implements UserDetailsService {

    private final UserJpaRepo userJpaRepo;

    public UserDetails loadUserByUsername(String userPk) {
        return userJpaRepo.findById(Long.valueOf(userPk)).orElseThrow(CUserNotFoundException::new);
    }
}
```





### User

```java
@Builder // builder를 사용할수 있게 합니다.
@Entity // jpa entity임을 알립니다.
@Getter // user 필드값의 getter를 자동으로 생성합니다.
@NoArgsConstructor // 인자없는 생성자를 자동으로 생성합니다.
@AllArgsConstructor // 인자를 모두 갖춘 생성자를 자동으로 생성합니다.
@Table(name = "user") // 'user' 테이블과 매핑됨을 명시
public class User implements UserDetails {
    @Id // pk
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private long msrl;
    @Column(nullable = false, unique = true, length = 30)
    private String uid;
    @JsonProperty(access = JsonProperty.Access.WRITE_ONLY)
    @Column(nullable = false, length = 100)
    private String password;
    @Column(nullable = false, length = 100)
    private String name;

    @ElementCollection(fetch = FetchType.EAGER)
    @Builder.Default
    private List<String> roles = new ArrayList<>();

    @Override
    public Collection<? extends GrantedAuthority> getAuthorities() {
        return this.roles.stream().map(SimpleGrantedAuthority::new).collect(Collectors.toList());
    }

    @JsonProperty(access = JsonProperty.Access.WRITE_ONLY)
    @Override
    public String getUsername() {
        return this.uid;
    }

    @JsonProperty(access = JsonProperty.Access.WRITE_ONLY)
    @Override
    public boolean isAccountNonExpired() {
        return true;
    }

    @JsonProperty(access = JsonProperty.Access.WRITE_ONLY)
    @Override
    public boolean isAccountNonLocked() {
        return true;
    }

    @JsonProperty(access = JsonProperty.Access.WRITE_ONLY)
    @Override
    public boolean isCredentialsNonExpired() {
        return true;
    }

    @JsonProperty(access = JsonProperty.Access.WRITE_ONLY)
    @Override
    public boolean isEnabled() {
        return true;
    }
}
```





### UserJpaRepo

```java
public interface UserJpaRepo extends JpaRepository<User, Integer> {
    Optional<User> findByUid(String email);
}
```





### CEmailSigninFailedException

```java
public class CEmailSigninFailedException extends RuntimeException {
    public CEmailSigninFailedException(String msg, Throwable t) {
        super(msg, t);
    }

    public CEmailSigninFailedException(String msg) {
        super(msg);
    }

    public CEmailSigninFailedException() {
        super();
    }
}
```





### SignController

```java
@Api(tags = {"1. Sign"})
@RequiredArgsConstructor
@RestController
@RequestMapping(value = "/v1")
public class SignController {

    private final UserJpaRepo userJpaRepo;
    private final JwtTokenProvider jwtTokenProvider;
    private final ResponseService responseService;
    private final PasswordEncoder passwordEncoder;

    @ApiOperation(value = "로그인", notes = "이메일 회원 로그인을 한다.")
    @PostMapping(value = "/signin")
    public SingleResult<String> signin(@ApiParam(value = "회원ID : 이메일", required = true) @RequestParam String id,
                                       @ApiParam(value = "비밀번호", required = true) @RequestParam String password) {
        User user = userJpaRepo.findByUid(id).orElseThrow(CEmailSigninFailedException::new);
        if (!passwordEncoder.matches(password, user.getPassword()))
            throw new CEmailSigninFailedException();

        return responseService.getSingleResult(jwtTokenProvider.createToken(String.valueOf(user.getMsrl()), user.getRoles()));

 }

 @ApiOperation(value = "가입", notes = "회원가입을 한다.")
 @PostMapping(value = "/signup")
 public CommonResult signin(@ApiParam(value = "회원ID : 이메일", required = true) @RequestParam String id,
                               @ApiParam(value = "비밀번호", required = true) @RequestParam String password,
                               @ApiParam(value = "이름", required = true) @RequestParam String name) {

        userJpaRepo.save(User.builder()
                .uid(id)
                .password(passwordEncoder.encode(password))
                .name(name)
                .roles(Collections.singletonList("ROLE_USER"))
                .build());
        return responseService.getSuccessResult();
    }
}
```







### UserController

```java
@Api(tags = {"2. User"})
@RequiredArgsConstructor
@RestController
@RequestMapping(value = "/v1")
public class UserController {

    private final UserJpaRepo userJpaRepo;
    private final ResponseService responseService; // 결과를 처리할 Service

    @ApiImplicitParams({
            @ApiImplicitParam(name = "X-AUTH-TOKEN", value = "로그인 성공 후 access_token", required = true, dataType = "String", paramType = "header")
    })
    @ApiOperation(value = "회원 리스트 조회", notes = "모든 회원을 조회한다")
    @GetMapping(value = "/users")
    public ListResult<User> findAllUser() {
        // 결과데이터가 여러건인경우 getListResult를 이용해서 결과를 출력한다.
        return responseService.getListResult(userJpaRepo.findAll());
    }

    @ApiImplicitParams({
            @ApiImplicitParam(name = "X-AUTH-TOKEN", value = "로그인 성공 후 access_token", required = false, dataType = "String", paramType = "header")
    })
    @ApiOperation(value = "회원 단건 조회", notes = "회원번호(msrl)로 회원을 조회한다")
    @GetMapping(value = "/user")
    public SingleResult<User> findUserById(@ApiParam(value = "언어", defaultValue = "ko") @RequestParam String lang) {
        // SecurityContext에서 인증받은 회원의 정보를 얻어온다.
        Authentication authentication = SecurityContextHolder.getContext().getAuthentication();
        String id = authentication.getName();
        // 결과데이터가 단일건인경우 getSingleResult를 이용해서 결과를 출력한다.
        return responseService.getSingleResult(userJpaRepo.findByUid(id).orElseThrow(CUserNotFoundException::new));
    }

    @ApiImplicitParams({
            @ApiImplicitParam(name = "X-AUTH-TOKEN", value = "로그인 성공 후 access_token", required = true, dataType = "String", paramType = "header")
    })
    @ApiOperation(value = "회원 수정", notes = "회원정보를 수정한다")
    @PutMapping(value = "/user")
    public SingleResult<User> modify(
            @ApiParam(value = "회원번호", required = true) @RequestParam int msrl,
            @ApiParam(value = "회원이름", required = true) @RequestParam String name) {
        User user = User.builder()
                .msrl(msrl)
                .name(name)
                .build();
        return responseService.getSingleResult(userJpaRepo.save(user));
    }

    @ApiImplicitParams({
            @ApiImplicitParam(name = "X-AUTH-TOKEN", value = "로그인 성공 후 access_token", required = true, dataType = "String", paramType = "header")
    })
    @ApiOperation(value = "회원 삭제", notes = "userId로 회원정보를 삭제한다")
    @DeleteMapping(value = "/user/{msrl}")
    public CommonResult delete(
            @ApiParam(value = "회원번호", required = true) @PathVariable int msrl) {
        userJpaRepo.deleteById(msrl);
        // 성공 결과 정보만 필요한경우 getSuccessResult()를 이용하여 결과를 출력한다.
        return responseService.getSuccessResult();
    }
}
```



pr 리뷰 요청드립니다~!

(요청주신 API명세서에서는 Request Header로 token이 있지만, 현재 JWT 관련코드가 미완성이므로 /**에 대하여 permitAll()로 API 완성하였습니다.)



| index | Method | Route                   | Request Body                                                 | Description          |
| ----- | ------ | ----------------------- | ------------------------------------------------------------ | -------------------- |
| 1     | GET    | /admin/v1/devGroups     |                                                              | 개발자 행사 전체조회 |
| 2     | GET    | /admin/v1/devGroups/:id |                                                              | 개발자 행사 단일조회 |
| 3     | POST   | /admin/v1/devGroups     | {<br />"name": 모임이름,<br />"description": 모임설명,<br />"link1": 링크1,<br />"link1": 링크2,<br />"link1": 링크3<br />} | 개발자 모임 생성     |



테스트 코드는 작성 전이고, 검증은 postman을 활용하였습니다.

감사합니다.









### 정상일 때

```json
{
  "data": [
		{
      "id": 1,
      "name": "이름1",
      "description": "설명1"
    },
    {
      "id": 2,
      "name": "이름2",
      "description": "설명2"
    }
  ],
  "error": {
  	"message": null
	}
}
```



### 비정상일 때

```json
{
  "data": null,
  "error": {
  	"message": "오류 메세지"
	}
}
```



처리해야하는 예외들

1. 

   

2. 

3. 







예외 처리한 내용 말씀드립니다. 

DevGroup에 대한 예외처리를 여러 Controller에서 할 필요가 있기 때문에, @RestControllerAdvice를 사용하여 전역 컨트롤러를 만들어두었습니다.

또한, ErrorResponse JSON을 통일하기 위해 ErrorResponse를 만들었습니다.

상세 처리 내용은 다음과 같습니다.



1. id가 누락되는 경우

   => id를 넘겨주지 않는 경우에는 '/' 에 해당하는 메서드(getDevGroups)가 있으므로 따로 처리가 필요 없다고 생각합니다. 

2. 데이터가 존재하지 않는 경우

   => id에 해당되는 데이터가 없는 경우에는, "Cant find any DevGroup given ID" 라는 메세지(상태코드: 400)를 응답하도록 하였습니다. 

   ![image-20210719221004645](/Users/sangjin/Library/Application Support/typora-user-images/image-20210719221004645.png)

3. PostMapping에서 필수값에 해당하는 값이 없는 경우

   => name, description, link1은 `nullable=false` 로, 값이 없는 상태로 들어오면, "Cant create DevGroup" 라는 메세지(상태코드: 400)를 응답하도록 하였습니다. 

   ![image-20210719221036577](/Users/sangjin/Library/Application Support/typora-user-images/image-20210719221036577.png)

























