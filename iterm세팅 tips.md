# iterm 세팅



1. zsh, oh my zsh 다운로드

   ```shell
   brew install zsh	
   sh -c "$(curl -fsSL https://raw.github.com/robbyrussell/oh-my-zsh/master/tools/install.sh)"
   ```



2. sanzzy색, agnoster테마, D2폰트 세팅

   **sanzzy**

   ```shell
   https://raw.githubusercontent.com/sindresorhus/iterm2-snazzy/master/Snazzy.itermcolors
   ```

   ```
   preferences -> profile -> colors -> color presets -> import sanzzy
   ```

   **agnoster**

   ```shell
   vi ~/.zshrc
   ```

   ```shell
   ZSH_THEME = "agnoster"
   ```

   **D2**

   ```shell
   https://github.com/naver/d2codingfont
   ```

   해당 파일을 서체관리자에 넣으면 세팅에서 확인할 수 있다.

   ```
   preferences -> profile -> text
   ```

   

3. 터미널 사용자 이름만 남기기

   ```powershell
   vi ~/.zshrc
   ```

   아래 전체를 맨 밑에 추가

   ```
   prompt_context() {
     if [[ "$USER" != "$DEFAULT_USER" || -n "$SSH_CLIENT" ]]; then
       prompt_segment black default "%(!.%{%F{yellow}%}.)$USER"
     fi
   }
   ```

   

4. New line

   ```shell
   vi ~/.oh-my-zsh/themes/agnoster.zsh-theme
   ```

   아래 영역에서 prompt_newline 추가

   ```
   build_prompt() {
     RETVAL=$?
     prompt_status
     prompt_virtualenv
     prompt_context
     prompt_dir
     prompt_git
     prompt_bzr
     prompt_hg
     prompt_newline //이부분을 추가 꼭 순서 지켜서
     prompt_end
   }
   ```

   아래 전체를 맨 밑에 추가

   ```
   prompt_newline() {
     if [[ -n $CURRENT_BG ]]; then
       echo -n "%{%k%F{$CURRENT_BG}%}$SEGMENT_SEPARATOR
   %{%k%F{blue}%}$SEGMENT_SEPARATOR"
     else
       echo -n "%{%k%}"
     fi
    
     echo -n "%{%f%}"
     CURRENT_BG=''
   }
   ```



5. Highlight

   ```shell
   brew install zsh-syntax-highlighting
   source /usr/local/share/zsh-syntax-highlighting/zsh-syntax-highlighting.zsh
   ```

   

6. 이모티콘

   ```
   vi ~/.zshrc
   ```

   ```
   prompt_context() {
     if [[ "$USER" != "$DEFAULT_USER" || -n "$SSH_CLIENT" ]]; then
       prompt_segment black default "%(!.%{%F{yellow}%}.)$USER🔥"
     fi
   }
   ```



출처 : https://ooeunz.tistory.com/21





Cf) conda, jupyter-notebook 명령어 먹게 하기

```shell
vi ~/.zshrc
```

```shell
# >>> conda initialize >>>
# !! Contents within this block are managed by 'conda init' !!
__conda_setup="$('/Users/osangjin/opt/anaconda3/bin/conda' 'shell.bash' 'hook' 2> /dev/null)"
if [ $? -eq 0 ]; then
    eval "$__conda_setup"
else
    if [ -f "/Users/osangjin/opt/anaconda3/etc/profile.d/conda.sh" ]; then
        . "/Users/osangjin/opt/anaconda3/etc/profile.d/conda.sh"
    else
        export PATH="/Users/osangjin/opt/anaconda3/bin:$PATH"
    fi
fi
unset __conda_setup
# <<< conda initialize <<<
```

