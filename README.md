WiringPi-php7
========
php7 WiringPi extension

Building
========
```
install php-dev
phpize
configure
make
make install
```
/etc/php/7.3/apache2/conf.d/wiringpi.ini
```
extension=wiringpi.so
```
/etc/php/7.3/cli/conf.d/wiringpi.ini
```
extension=wiringpi.so
```

todo
```
sudo adduser www-data i2c
sudo adduser www-data spi
sudo adduser www-data gpio
```
