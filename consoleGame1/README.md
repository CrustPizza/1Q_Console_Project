### 1. Engine
	+ Controller ( 작업 영역 )
		- User Input을 처리하는 함수들을 모아둔 파일
		- xinput으로 XBOX 게임패드 연동
		- GetAsyncKeyState의 상태를 DOWN, UP, STAY, TOGGLE 4가지 상태로 구별
		- 멀티 게임 환경에서 패킷으로 사용하게 Input을 문자열로 변환
	+ Render ( 작업 영역 )
		-