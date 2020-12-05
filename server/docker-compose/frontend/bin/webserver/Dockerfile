FROM litespeedtech/openlitespeed:1.6.9-lsphp74
LABEL maintainer="Sashwat K <sashwat0001@gmail.com>"

# Update and upgrade linux container
RUN apt-get -y update
RUN apt-get upgrade -y

# Install useful tools
RUN apt-get -y install apt-utils nano wget

# Install important libraries
RUN apt-get -y install --fix-missing apt-utils build-essential git curl libcurl4 zip openssl

# Clone the project
RUN git clone --depth 1 https://github.com/sashuu6/home-weather-station-mini.git /root/station-mini/


RUN apt-get autoremove -y