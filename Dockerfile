FROM ubuntu:18.04
MAINTAINER a.medved

RUN apt-get update          && \
    apt-get -y install wget 
RUN cd usr/lib              && \
    wget https://github.com/Tyill/SVisual/releases/download/1.1.4/SVDocker.tar.gz && \
	tar -zxvf SVDocker.tar.gz && \
	mv SVDocker /usr/bin      && \
	mv web /usr/bin           && \
	mv svdocker.ini  /home

CMD ["./usr/bin/SVDocker", "/home/svdocker.ini"]