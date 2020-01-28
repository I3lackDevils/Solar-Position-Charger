# Solar-Position-Charger
## The target
For my class Embededd Interfacing I had to make a rather large project with some requirements.
- μprocessor :  Atmega 328P
- Display (LCD ,Touchscreen), control leds
- Interface: SPI, I2C, Digital I/O, Analog I/O
- Wireless connection with internet/cloud
- ISP programming
- Flexibele environment, multi functinal

## The idea
I ended up with inventing a system that would make solar charging more efficiënt. I wanted to be able to move the solar panels straight to the sun. Also I wanted to monitor how much power I am consuming and how much power I'm generating. This way I could make the system smarter. And analyste the power of the sun trough the year. Then I wanted to be able to access all this data live from a dynamic webpage and also put this data in a database to analyse this data later on. 

So with my idea in mind I started thinking about wich components I needed and ordered those. 
- 2 INA219s power measurement IC's
- 1 MCP73831 charging IC
- Atmega 328P
- 16 MHz clock crystal
- FT232RL USB
- Esp8266 WiFi module
- Midas I²C LCD 16x2 (HARD TO FIND LIBRARY)
- Bunch of capacitors/recisotrs/leds
- RaspberryPi

## The execution
So after I got my components I started designing a PCB in Eagle. After triple checking my PCB I printed it and soldered my SMD components with a pick & place and a soldering oven. After my PCB was finished I started programming some test programs with succes. I tested my LCD and tried to read values from my INA sensors. Unfortunately both of the sensor's didn't work. One was reading false values due to wrong connection I made. You need to connect them in series to the whole load to measure there current but I accidently messed up and connected them in parallel. The other sensor died during the soldering process I gues because I coudn't even read it's I²C adress. So I decided to order some INA breakout boards. They worked absolutly fine. After that I started to program the whole project and also made a serial connection between the ESP8266 and the atmega so I could post data to a local RaspberryPi server. I connected all the cables and was very happy with my result.
