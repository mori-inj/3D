# 3D

implementing 3D projection view only with GDI(+).

WASD: rotate view.  
up/down: move back and forth.  

* winapi(gdi)의 함수들 중 화면에 무언가를 그리는 데에는 화면상의 픽셀을 매개변수로 받는 MoveToEx, LineTo 두 함수만 사용하고 그 외의 모든 openGL의 rendering pipeline을 따라해 보는 게 목표
* winapi, C++ 사용
* 이미지  
![alt tag](https://github.com/mori-inj/3D/blob/master/%EC%BA%A1%EC%B2%98.PNG)
![alt tag](https://github.com/mori-inj/3D/blob/master/%EC%BA%A1%EC%B2%982.PNG)
![alt tag](https://github.com/mori-inj/3D/blob/master/%EC%BA%A1%EC%B2%983.PNG)
![alt tag](https://github.com/mori-inj/3D/blob/master/%EC%BA%A1%EC%B2%984.PNG)
  <p align="center">
    <img src="https://raw.githubusercontent.com/mori-inj/mori-inj.github.com/master/sphere.png" width="350" height="323.75"/>
    <img src="https://raw.githubusercontent.com/mori-inj/mori-inj.github.com/master/function.png" width="350" height="323.75"/>
  </p>
* 레이캐스터까지 구현한 상태이며, cpu만 사용 && gdi의 성능상 속도에 한계가 있음
* 그래픽스 교과목 수강 이전에 진행한 프로젝트
