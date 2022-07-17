# DoS-attack-on-Wearable-Device-
Code of Infra of DoS attack on Wearable Device 

- study in 2021
- 구성 : 
  - C&C 서버 : Botnet에게 피해자 정보 전달하는 소켓 통신 프로그램 (Window 10)
  - Botnet (삼성 웨어러블) : C&C 서버와 피해자 PC와 통신하는 소켓 통신 프로그램 (Tizen Studio)
  - 피해자 (Linux) : 최종 공격 타겟으로 Tread 기반 소켓 통신 가능 (Linux) 


- 개발 담당 : 
  - C&C서버와 피해자 소켓 통신 코드 개발
  - C&C서버에서 정보를 받아 피해자에게 공격하는 전반적 흐름의 Botnet 통신 코드 개발 
