# Arduino alarm system
Simple alarm system developed as part of an individual project for the Embedded Systems course. System was designed and tested using the Tinkercad simulator.

## Sample system layout
![Circuit](https://github.com/Mr-Victor16/arduino-alarm-system/blob/main/circuit.png?raw=true)

Above image represents an example layout of the alarm system, created in the Tinkercad simulator. It showcases the arrangement and wiring of the components used in the project.

## Components used
- Arduino Uno R3,
- 4x4 Keypad,
- LCD Display,
- Potentiometer,
- Resistor,
- LED Strip,
- Piezo Buzzer,
- PIR Sensor,
- Slide Switch,
- Tilt Sensor.

## System capabilities
- Arming/disarming the alarm via a PIN code entered on a matrix keypad (2 types of codes: user and administrator).
- Support for over 100 subsystems, with multiple sensors configurable for each subsystem.
- Ability to change both user and administrator access codes.
- Protection against excessive incorrect code entries.
- Data stored in EEPROM memory, ensuring system status is preserved during power outages.
- Detection of communication loss with subsystems, with the ability to add new subsystems while the system is operational.
- Operation modes selectable via letters A-D on the matrix keypad:
    - A: Arm the system;
    - B: Change the user PIN;
    - C: Change the administrator PIN;
    - D: Reconfigure the system;
- Status signaling through an LCD display, an LED strip and a piezo buzzer (sound).

## FAQ
#### What are the default PIN codes for users and administrators?
The default PIN for users is 4321, while for administrators, it is 1234..

#### What do the LED strip colors indicate?
- Blue (entire strip): The alarm is armed.
- Green (entire strip): The alarm is disarmed.
- Yellow (single LED): No connection to the corresponding subsystem.
- Red (single LED): Motion detected in the corresponding subsystem. 
- Red (entire strip): Alarm triggered due to motion detection.
