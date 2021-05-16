## **개발 환경**

사용 언어 : C++

그래픽 라이브러리 : Direct X

물리엔진 : physx

기타 스크립트 언어 : lua-script( 맵 제작)

---

## **구현**

-   **Export Import 기능 구현  
    **
-   Effect 기능 구현
-   UI 기능 구현
-   Terrain 기능 구현
-   Nvidia PhysX Library를 이용한 물리 엔진 구현
-   Lua Script 연동

---

## **상세 설명**

### Max Exporter

[##_Image|kage@cSOIFS/btq4buK1iFg/X8z76WCyko6SOJA4gw0EQK/img.png|alignCenter|data-origin-width="1602" data-origin-height="528" data-ke-mobilestyle="widthContent"|||_##]

-   3D Max Script를 이용해서 Model, Animation을 XML로 Export 한다.
-   Game Engine에서 Export한 XML을 Import하여 3D Model Data로 사용한다.

---

### Effect

[##_Image|kage@5FB7x/btq4bYLD5l8/Ra80kWGCFqoIfBgbwNKp80/img.png|alignCenter|data-origin-width="1518" data-origin-height="664" data-ke-mobilestyle="widthContent"|||_##]

-   Post Effect
-   렌더링 완료된 화면에 Vertex Shader, Pixel Shader를 이용하요 화면의 후처리를 한다.
-   Star Effect, Bloom Effect 사용 예제

---

[##_Image|kage@n36Bz/btq4bjweW9N/V2rJIJ2F0gT9f4cejwfIM1/img.png|alignLeft|data-origin-width="1486" data-origin-height="428" data-ke-mobilestyle="widthContent"|||_##]

-   Star Effect
-   빛을 6방향으로 확장하여 한 이미지로 합병한다. 빛의 회절 표현

---

[##_Image|kage@mp1kD/btq4cvWOl70/KYZJJJhWKDzmUVLOuSzVkK/img.png|alignCenter|data-origin-width="1368" data-origin-height="452" data-ke-mobilestyle="widthContent"|||_##]

-   Bloom Effect
-   추출된 빛의 밝은 부분을 가로 방향과 세로 방향으로 흐리게 하고 원래 이미지에 합성한다.

---

[##_Image|kage@6gZps/btq4bs7wimZ/Dx0o1I0kXS3ynNkOTsIhB1/img.png|alignCenter|data-origin-width="1212" data-origin-height="570" data-ke-mobilestyle="widthContent"|||_##]

-   Water
-   Plane에 물결과 반사 효과를 추가해서 Water 표현

---

[##_Image|kage@bYGDQM/btq4dLqYxko/M6kdbZyhQ4j8nC2UIUwkH1/img.png|alignCenter|data-origin-width="1492" data-origin-height="574" data-ke-mobilestyle="widthContent"|||_##]

-   Vertex Shader, Normal Map을 이용하여 물결 표현

---

[##_Image|kage@3ZMe8/btq4cNJE1oU/te4XU6m7KgXepJHpYkPCFK/img.png|alignCenter|data-origin-width="1478" data-origin-height="546" data-ke-mobilestyle="widthContent"|||_##]

-   Sky Box를 Water Plane에 렌더링해서 물위에 하늘이 반사되는듯한 효과 구현

---

[##_Image|kage@bgaa1N/btq4cuXSFzR/GTctKc0leWoeppDkFlk96K/img.png|alignCenter|data-origin-width="1230" data-origin-height="580" data-ke-mobilestyle="widthContent"|||_##]

-   Tail 효과
-   모델의 일정 움직인 거리만큼 Vertex를 생성하여 Tail을 만든다. 일정 시간이 지나면 Vertex를 제거 하여 Tail의 길이를 결정한다.
-   모델의 움직이는 궤도를 따라다니는 꼬리 같은 효과를 준다.

---

[##_Image|kage@bCCNYA/btq4bt6tAYI/lzUBJtkttzPcjmvjUNv8Gk/img.png|alignCenter|data-origin-width="1206" data-origin-height="582" data-ke-mobilestyle="widthContent"|||_##]

-   particle effect
-   입자의 위치, 크기, 회전, 색 등을 변형하여 불꽃, 빛 등 다양한 효과를 만든다.

---

### Terrain

[##_Image|kage@rFJcA/btq4aZxWwR4/d1YVgw0RWDS5BHDdriHlxK/img.png|alignCenter|data-origin-width="1472" data-origin-height="516" data-ke-mobilestyle="widthContent"|||_##]

-   Height Map Texture에서 높이맵을 로딩해서 지형의 높이를 변형한다.
-   Height Map Texture
    -   픽셀당 0~255 까지 Gray Color로 만들어진다.
    -   여기서 로딩한 0~255값이 지형의 높이가 된다.

---

### UI

[##_Image|kage@bDt0Dl/btq4divP2l7/PK5aeGk2TrwMCmRmeEFUQ0/img.png|alignCenter|data-origin-width="1690" data-origin-height="546" data-ke-mobilestyle="widthContent"|||_##]

-   2D Spirte Rendering
-   버튼 이벤트 핸들링 기능 추가
