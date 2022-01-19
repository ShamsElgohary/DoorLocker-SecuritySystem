# DoorLocker-SecuritySystem

## Overview
![image](https://user-images.githubusercontent.com/68311964/150090212-d1f288ba-43cc-4157-80b1-e8d214cb9b6e.png)

## Implementation  

• Door Locker Security System consists of two ECU’s. The first ECU called HMI responsible for interfacing with the user and the
second ECU called control ECU which is responsible for the system operations and control.  
• Implemented the following drivers Keypad, LCD, DC Motor, UART, Timer, I2C and External
EEPROM.


1- The system asks the user to enter a password and verify the password entered.  
2- The next phase the system asks the user if he wants to open the door or change the password.  
3- If the user chooses to change password then he is capable of changing the password and then is redirected to phase 2.  
4- If the user chooses to open the door, he has 3 chances to enter the password correctly,  
- If the password is entered correctly within the 3 chances the door will open in 30 seconds and then stay opened for an extra 3 seconds and then close again in 30 seconds.  
- Else the buzzer is activated and the whole system is locked for 60 seconds.  



![image](https://user-images.githubusercontent.com/68311964/116821283-8d015780-ab79-11eb-83f7-ec6c7a1a7cf3.png)


