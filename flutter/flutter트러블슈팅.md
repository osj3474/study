1. Cocoa pods 오류는 homebrew로 다운로드 하면 됨.







2. 개발자를 확인할 수 없기 때문에 'iproxy' 를 열 수 없습니다.

```
sudo xattr -d com.apple.quarantine /Users/sangjin/Documents/development/flutter/bin/cache/artifacts/usbmuxd/iproxy
```





3. CocoaPods could not find compatible versions for pod "firebase_core":

   => 프로젝트의 ios 폴더를 열고 Runner로 이동하여 Runner.xcodeproj 파일을 두 번 클릭하여 엽니 다.
   xcode가 열리면 왼쪽 창에서 상단에 Runner가 표시되고 클릭하면 일반적으로 배포를 9.0에서 12.0으로 변경해야합니다. 그 후 xcode를 닫고 Android 스튜디오로 돌아가서 터미널에서 ios 프로젝트 폴더로 이동하여 'flutter clean'을 실행 한 다음 'pod update'를 실행합니다. 필요한 firebase 파일을 정리하고 설치합니다. 앱을 다시 실행하십시오.





4. `CocoaPods could not find compatible versions for pod "cloud_firestore":`

   => /ios/Podfile에서 버전 설정, xcode에서 버전 설정 동일하게

   혹시나 참고

   ```
   flutter clean
   pod repo update
   /ios/Pods 삭제, /ios/Podfile.lock 삭제
   pod install
   ```

   