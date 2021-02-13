# Git

```bash
git init . : 현재 디렉토리 버전관리 시작 // .git 파일에 버전관리와 관련된 파일들이 있음.
git init 저장소이름 : 저장소이름 이라는 repository를 생성
git status : 현재 상태 버전관리 상태를 알 수 있다. // untracted 파일은 버전관리 안 들어간 것.
git add : untracted 파일을 staging area에 올리는 것. 그러면 committed file
git commit : 기본 에디터를 띄운다.
git config --global core.editor "vim" : 기본 에디터를 빔으로 설정한다.
git commit -m "a": "a"라는 이름의 repository 버전을 만든다.
git commit -am "a" : staging area에 있는 파일의 수정사항을 저장하고, "a"라는 이름의 repository 버전을 만든다.
git commit --amend : 에디터로 들어가서 commit message를 바꿀수 있다.
git log : 버전의 역사를 볼 수 있다. // q를 누르면 나갈 수 있다.
git log -stat : 버전마다 어떤 파일이 연루되어 있는지 확인할 수 있다.
git log -p : 각 버전마다 어떤 수정이 일어났는지 확인할 수 있다.
git log --all --graph --oneline : 모든 브랜치를 보고, 시각적으로 보고, 장황하지 않게 log보기
git checkout 버전주소 : 해당 버전으로 갈 수 있다.
git checkout 브랜치이름 : 해당 브랜치를 HEAD가 가리키게 함과 동시에 그 때의 버전으로 간다.
git checkout master : 가장 최신 버전으로 갈 수 있다.
git diff : 마지막 버전과 working tree 사이의 차이를 볼 수 있다.(commit 전 마지막 기회로 이전상태를 보여준다고 생각하면 된다.)
git reset --hard : local에서 수정했지만 이를 무시하고 마지막 버전 상태로 돌아가게 하는 것.
git reset --hard 버전주소 : master버전을 버전주소에 해당하는 버전으로 만드는 것.
git revert 버전주소 : 버전주소에서 수정된 사항들을 모두 삭제하여 버전으로 남긴다. 이전 상태로 돌리기는 돌리는데 역사를 남기기 위해서 사용한다.
cf) 버전주소는 기억하기 힘드니까 tag를 사용할 수도 있다.
git branch 브랜치이름 : 브랜치이름 라는 이름으로 브랜치를 만든다. 
// 브랜치 : 하나의 repository 안에서 공통의 버전을 공유하면서도 각자의 버전을 또 가질 수 있는 개념
cf) git log해서 브랜치들의 가지를 살펴보면 버전단위로 가지를 뻗어나감을 알 수 있다. 참고.
git merge 해당브랜치 : 해당 브랜치를 master브랜치와 병합한 새로운 버전을 만든다.
```

* working tree : 기본 작업 공간, 새로 만든 파일이 여기에 소속된다.(untracked file)
* staging area : 버전관리가 필요한 파일을 올려두는 곳, git add를 한 파일이 여기에 소속된다. 이 공간부터 git의 관할 아래 있게 된다. 그리고 파일을 git commit 하게 되면 버전관리가 시작된 것이다. 
* repository : 버전이 저장되어 있는 곳(.git 파일), 



##### cf) HEAD는 기본적으로 master branch를 포인팅하고 있고, master branch는 마지막 버전을 가지고 있다.



### 실제 사용해보기

1. Local => Repository

```bash
git add 해당파일   // 혹은 git add .
git commit -m "Commit Message"
git push
```



2. Merge (각 브랜치가 다른 파일을 수정한 경우, 같은 파일이라도 다른 부분을 수정한 경우)

```bash
git checkout master
git merge 합칠브랜치    // 실수했다면 git reset --hard 버전주소
```



3. Merge (각 브랜치가 같은 파일의 같은 부분을 수정한 경우)

```bash
git checkout master
git merge 합칠브랜치    // 이러면 CONFLICT 가 뜬다
```

```bash
그 파일을 에디터로 열고 들어가보면 같은 부분을 수정한 것에 대해 다음과 같은 내용으로 채워져있다.

<<<<<<<<<<<<<< HEAD
불라불라1
==========
불라불라2
>>>>>>>>>>>>>> 합칠브랜치

불라불라1은 HEAD에서 수정을 한 내용이고, 불라불라2는 합칠브랜치에서 수정을 한 내용이다.
그러면 다음과 같이 내가 최종 수정을 해주면 된다.

<<<<<<<<<<<<<<< HEAD
>>>>>>>>>>>>>>> 합칠브랜치
==========

이거 3개를 기본적으로 지우고, 정말 들어갈 내용을 채운다.
```

```bash
git add 해당파일  
git commit
끝.
```



4. git clone해서 해당 내용을 나의 저장소에서 계속 작업하고 싶은 경우

   ```bash
   나의 저장소 하나 말들어 놓고 시작
   
   git clone otherrepository_url
   cd otherrepository
   git remote -v
   git remote remove origin
   git remote add origin myrepository_url
   git remote -v
   git push -u origin master
   ```

   



-----------------------------------------------------------------------------------------------------------------------------------------------------------

### 각종 문제 및 팁

-------------------------------------------

#### 1. 로그인 없이 원격 저장소 이용하기

 : https가 아닌 ssh를 이용하여 자동로그인의 편익을 얻을 것이다. repository에 public key를 두고, local에 private key를 두는 식으로 이를 구현한다.

1) 로그인 없이 git push를 하고 싶은 pc의 home 디렉토리에서

```bash
ssh-keygen
```





#### 2. git pull을 하지 않고 작업을 했다가..

 : 집에서 작업하고 push한 다음에, 다음날 회사에 가서 pull을 하지 않고 작업을 해버린 경우는 어떻게 해야할까?

회사에서 작업을 마치고 push를 했더니 git pull을 하라는 메세지를 받았다. 헉! 그러면, git pull을 하면 오늘 작업한 내용이 다 사라지는게 아닐까 하지만. 뒤늦게 git pull을 하게 되더라도 이전 상태를 받아오고 내 local에서 작업한 내용도 add되지 않은채로 남아있다. 

따라서, 다른 곳에서 작업을 시작하기 전에 git pull을 꼭 하는게 맞으나, 깜빡했다고 하더라도 그 날 작업한 내용이 남아있으니 걱정하지 않아도 되겠다.



#### 3. 100MB넘는 걸 올리려고 했다가,,

: 물론, 100MB가 넘는 파일을 올릴 수도 있다. 하지만, 지금은 설치파일이나 필요없는 (100MB가 넘는)파일을 모르고 push했다가 생기는 문제를 해결한다.

일단, commit을 해버린 상태라면, 로컬에서 100MB가 넘는 파일을 지운다고 할지라도 push는 되지 않는다. 그러면 commit 을 취소해야한다.

commit 을 취소하는 방법은 reset과 revert가 있고, reset의 경우를 알아보자.

```shell
git log
```

를 해보고, 이전 커밋 코드를 카피해서 reset한다.

```shell
git reset [돌아가고 싶은 commit코드] --hard
```



#### 4. 특정 브랜치 클론

```shell
git clone -b sangjin --single-branch https://github.com/BAECHOOs/Spring-toyproject.git
```

