# osa-weatherstation
osa-weatherstation is a project for https://open-source.academy/ - https://github.com/Opensource-Academy


### Resources
https://randomnerdtutorials.com/esp32-web-server-with-bme280-mini-weather-station/


### Worker
For developing purpose a docker container with mariadb 
user:pi
password: framboos

start docker container: 
docker-compose up
container name: osa
connectig command line: docker exec -it osa /bin/sh

to confiure database:
mysql -u pi -p weather < /tmp/database/export_db.sql


Delete old docker containers: docker system prune


