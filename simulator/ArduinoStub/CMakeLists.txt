

add_library(ArduinoStub 
	Arduino.h
        Arduino.cpp
        Servo.h
        Servo.cpp
        Serial.h
        Serial.cpp
	)

target_include_directories(ArduinoStub PUBLIC .)
