import network, socket
from machine import Pin, reset
import camera
from neopixel import NeoPixel
from time import sleep_ms

color_dict = {  b'GREEN':(255, 0, 0)
         , b'RED'  :(0, 255, 0)
         , b'BLUE' :(0, 0, 255)
         , b'BLACK':(0, 0, 0)
           }
# count = 0
# camera initialization
camera.init(0, format=camera.JPEG, fb_location=camera.PSRAM)
# camera.framesize(camera.FRAME_QVGA)    # 320x240
# camera.framesize(camera.FRAME_QQVGA)   # 160x120
camera.framesize(camera.FRAME_96X96)
camera.quality(10)
#
np = NeoPixel(Pin(12), 1)
np[0] = color_dict[b'BLACK']
np.write()
#
onboard_led = Pin(4)
#
led = Pin(4, Pin.OUT)
for _ in range(2):
    led.on()
    sleep_ms(300)
    led.off()
    sleep_ms(300)
# activate access point
ap = network.WLAN(network.AP_IF)
ap.active(True)
ap.config(essid="ESP32-cam-1", password="")
# activate server
s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR,1)
s.bind(('192.168.4.1', 10086))
# 
while True:
    try:
        cmd = s.recvfrom(100)
        color = color_dict[cmd[0]]
        np[0] = color
        np.write()
        buf = camera.capture()
        s.sendto(buf, cmd[1])
#        count += 1
#        print('image ', count)
    except KeyboardInterrupt:
        break
#
s.close()
del s
np[0] = color_dict[b'BLACK']
np.write()
reset()