 Assignment 1 — ESP32 Implementation


Name: Laiba Taseen
Registration No: 23-NTU-CS-1258
Course: IoT / Embedded Systems
Board Used: NodeMCU-32S (ESP32)


Task A — Multi-Mode LED Control

Description:
Used Button 1 to cycle through LED modes (displayed on OLED):
Both OFF
Alternate Blink
Both ON
PWM Fade
Used Button 2 to reset to OFF mode.
The current state is displayed on the OLED display.

Components Used:
2 Push Buttons
2 LEDs
1 Buzzer
1 OLED Display (SSD1306)

Task B — Button Press-Type Detection

Description:
Used a single button to detect type of press:
Short press (< 1.5 s) → Toggle LED
Long press (> 1.5 s) → Play buzzer tone
OLED displays the detected event.

Components Used:
1 Push Button
1 LED
1 Buzzer
1 OLED Display (SSD1306)

     Pin Map
Component	  GPIO Pin	   WokwiLabel (D-number)
LED1	         2	         D2
LED2	         4	         D4
Buzzer	       12          D12
Button1	       13          D13
Button2	       14	         D14
OLED SDA	     21	         D21
OLED SCL	     22	         D22


💻 Wokwi Project
Task A:  https://wokwi.com/projects/445581165646638081
Task B:  https://wokwi.com/projects/445868312274780161

