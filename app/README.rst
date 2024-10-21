Platform
********************

Docker Container
********************
docker run --name stm -it -v d:\Zephyr\STM\app\:/home/user/zephyrproject/app zephyr_image /bin/bash
source ~/zephyrproject/.venv/bin/activate

Build
********************
cd ~/zephyrproject/
west build -p always -b nucleo_f446ze app -d ./app/build
********************

Program
********************
D:\Zephyr\STM\app\build\zephyr\zephyr.hex

