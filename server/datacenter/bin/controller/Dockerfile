FROM python:3.8.2
LABEL maintainer="Sashwat K <sashwat0001@gmail.com>"

# Update and upgrade linux container
RUN apt-get -y update
RUN apt-get upgrade -y

ADD ./code /var/lib/controller
WORKDIR /var/lib/controller

RUN pip3 install -r requirements.txt
CMD python3 app.py