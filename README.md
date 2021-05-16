## **개발 환경**

사용 언어 : C++

그래픽 라이브러리 : Direct X

물리엔진 : physx

기타 스크립트 언어 : lua-script( 맵 제작)

---

## **구현**

-   Export Import 기능 구현  
-   Effect 기능 구현
-   UI 기능 구현
-   Terrain 기능 구현
-   Nvidia PhysX Library를 이용한 물리 엔진 구현
-   Lua Script 연동

---

## **상세 설명**

### Max Exporter

![](https://img1.daumcdn.net/thumb/R1280x0/?scode=mtistory2&fname=https%3A%2F%2Fblog.kakaocdn.net%2Fdn%2FcSOIFS%2Fbtq4buK1iFg%2FX8z76WCyko6SOJA4gw0EQK%2Fimg.png)

-   3D Max Script를 이용해서 Model, Animation을 XML로 Export 한다.
-   Game Engine에서 Export한 XML을 Import하여 3D Model Data로 사용한다.

---

### Effect

![](https://img1.daumcdn.net/thumb/R1280x0/?scode=mtistory2&fname=https%3A%2F%2Fblog.kakaocdn.net%2Fdn%2F5FB7x%2Fbtq4bYLD5l8%2FRa80kWGCFqoIfBgbwNKp80%2Fimg.png)

-   Post Effect
-   렌더링 완료된 화면에 Vertex Shader, Pixel Shader를 이용하요 화면의 후처리를 한다.
-   Star Effect, Bloom Effect 사용 예제

---

![](https://img1.daumcdn.net/thumb/R1280x0/?scode=mtistory2&fname=https%3A%2F%2Fblog.kakaocdn.net%2Fdn%2Fn36Bz%2Fbtq4bjweW9N%2FV2rJIJ2F0gT9f4cejwfIM1%2Fimg.png)

-   Star Effect
-   빛을 6방향으로 확장하여 한 이미지로 합병한다. 빛의 회절 표현

---

![](https://img1.daumcdn.net/thumb/R1280x0/?scode=mtistory2&fname=https%3A%2F%2Fblog.kakaocdn.net%2Fdn%2Fmp1kD%2Fbtq4cvWOl70%2FKYZJJJhWKDzmUVLOuSzVkK%2Fimg.png)

-   Bloom Effect
-   추출된 빛의 밝은 부분을 가로 방향과 세로 방향으로 흐리게 하고 원래 이미지에 합성한다.

---

![](https://img1.daumcdn.net/thumb/R1280x0/?scode=mtistory2&fname=https%3A%2F%2Fblog.kakaocdn.net%2Fdn%2F6gZps%2Fbtq4bs7wimZ%2FDx0o1I0kXS3ynNkOTsIhB1%2Fimg.png)

-   Water
-   Plane에 물결과 반사 효과를 추가해서 Water 표현

---

![](https://img1.daumcdn.net/thumb/R1280x0/?scode=mtistory2&fname=https%3A%2F%2Fblog.kakaocdn.net%2Fdn%2FbYGDQM%2Fbtq4dLqYxko%2FM6kdbZyhQ4j8nC2UIUwkH1%2Fimg.png)

-   Vertex Shader, Normal Map을 이용하여 물결 표현

---

![](https://img1.daumcdn.net/thumb/R1280x0/?scode=mtistory2&fname=https%3A%2F%2Fblog.kakaocdn.net%2Fdn%2F3ZMe8%2Fbtq4cNJE1oU%2Fte4XU6m7KgXepJHpYkPCFK%2Fimg.png)

-   Sky Box를 Water Plane에 렌더링해서 물위에 하늘이 반사되는듯한 효과 구현

---

![](https://img1.daumcdn.net/thumb/R1280x0/?scode=mtistory2&fname=https%3A%2F%2Fblog.kakaocdn.net%2Fdn%2Fbgaa1N%2Fbtq4cuXSFzR%2FGTctKc0leWoeppDkFlk96K%2Fimg.png)

-   Tail 효과
-   모델의 일정 움직인 거리만큼 Vertex를 생성하여 Tail을 만든다. 일정 시간이 지나면 Vertex를 제거 하여 Tail의 길이를 결정한다.
-   모델의 움직이는 궤도를 따라다니는 꼬리 같은 효과를 준다.

---

![](https://img1.daumcdn.net/thumb/R1280x0/?scode=mtistory2&fname=https%3A%2F%2Fblog.kakaocdn.net%2Fdn%2FbCCNYA%2Fbtq4bt6tAYI%2FlzUBJtkttzPcjmvjUNv8Gk%2Fimg.png)

-   particle effect
-   입자의 위치, 크기, 회전, 색 등을 변형하여 불꽃, 빛 등 다양한 효과를 만든다.

---

### Terrain

![](https://img1.daumcdn.net/thumb/R1280x0/?scode=mtistory2&fname=https%3A%2F%2Fblog.kakaocdn.net%2Fdn%2FrFJcA%2Fbtq4aZxWwR4%2Fd1YVgw0RWDS5BHDdriHlxK%2Fimg.png)

-   Height Map Texture에서 높이맵을 로딩해서 지형의 높이를 변형한다.
-   Height Map Texture
    -   픽셀당 0~255 까지 Gray Color로 만들어진다.
    -   여기서 로딩한 0~255값이 지형의 높이가 된다.

---

### UI

![](https://img1.daumcdn.net/thumb/R1280x0/?scode=mtistory2&fname=https%3A%2F%2Fblog.kakaocdn.net%2Fdn%2FbDt0Dl%2Fbtq4divP2l7%2FPK5aeGk2TrwMCmRmeEFUQ0%2Fimg.png)

-   2D Spirte Rendering
-   버튼 이벤트 핸들링 기능 추가
