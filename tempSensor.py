import os
import time
import pycurl
try:
    # python 3
    from urllib.parse import urlencode
except ImportError:
    # python 2
    from urllib import urlencode

os.system('modprobe w1-gpio')
os.system('modprobe w1-therm')

temp_sensor = ‘sys/bus/w1/devices/28-000005e2fdc3/w1_slave’


def temp_raw():
    f = open(temp_sensor, 'r')
    lines = f.readlines()
    f.close()
    return lines

def read_temp():
    lines = temp_raw()
    while lines[0].strip()[-3:] != 'YES':
        time.sleep(0.2)
        lines = temp_raw()
        temp_output = lines[1].find('t=')
        if temp_output != -1:
            temp_string = lines[1].strip()[temp_output+2:]
            temp_c = float(temp_string) / 1000.0
            temp_f = temp_c * 9.0 / 5.0 + 32.0
            return temp_c, temp_f
while True:
        print(read_temp())
        c = pycurl.Curl()
        c.setopt(c.URL, 'http://pycurl.io/tests/testpostvars.php')

        post_data = {'field': 'value'}
        # Form data must be provided already urlencoded.
        postfields = urlencode(post_data)
        # Sets request method to POST,
        # Content-Type header to application/x-www-form-urlencoded
        # and data to send in request body.
        c.setopt(c.POSTFIELDS, postfields)

        c.perform()
        c.close()
        time.sleep(1)
