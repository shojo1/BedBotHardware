echo "dtoverlay=w1-gpio" >> /boot/config.txt
sudo shutdown -r now
git clone git://git.drogon.net/wiringPi
cd wiringPi
git pull origin
./build
cd..
sudo apt-get install libcurl4-openssl-dev
sudo pip install pycurl
gcc -o gpio gpio.c -l wiringPi
sudo ./gpio
sudo python tempSensor.py
