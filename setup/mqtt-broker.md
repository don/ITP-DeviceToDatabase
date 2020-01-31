# Setting up the Mosquitto MQTT broker

## macOS

Install Homebrew https://brew.sh/

    brew install mosquitto

Launch mosquitto from a terminal:
	
	/usr/local/sbin/mosquitto -c /usr/local/etc/mosquitto/mosquitto.conf


To have launchd start mosquitto now and restart at login:

	  brew services start mosquitto

This starts a mosquitto broker on your machine on the unencrypted port 1883. By default username and password are not required. You can make changes to the configuration by editing the mosquitto configuration file: /usr/local/etc/mosquitto/mosquitto.conf.

## Windows10

Download and install from [https://mosquitto.org/download/](https://mosquitto.org/download/)

Using cmd or PowerShell run from C:\Program Files\mosquitto

## Linux

We want to use TLS certificates to encrypt all traffic to between our MQTT broker and the clients. Let's Encrypt letsencrypt.org is a automated certificate authority that makes it easy to get free TLS certificates. We set up the Nginx web server and use the letsencypt bot to get a SSL certificate. Then we install mosquitto, configure it to use the TLS certificate and require users to login. These instructions are based on Digital Ocean's excellent documentation. https://www.digitalocean.com/community/tutorials/how-to-install-and-secure-the-mosquitto-mqtt-messaging-broker-on-ubuntu-18-04

Create a new DigitalOcean virtual machine (or other VM.) I chose Ubuntu 18.10 x64 and the $5/month droplet size.

 * Choose NY data center
 * Add a ssh key for logging in
 * Choose a hostname

I have DigitalOcean running DNS for the itpdtd.com domain, so I can set up a new domain record for itpdtd.com pointing to my new VM.

Since I set up my SSH key when I created the machine, I can ssh into root@itpdtd.com

Turn on the firewall and restrict incoming traffic to ssh, port 22

    ufw allow ssh
    ufw enable

Update the vm

    apt update
    apt dist-upgrade -y

Reboot, just in case the kernel was upgraded

    shutdown -r now

SSH back in

    ssh root@itpdtd.com

Install the compiler tools, mosquitto broker, mosquitto clients, and nginx 

	apt install build-essential mosquitto mosquitto-clients nginx -y

Open the firewall for web traffic

    ufw allow http
    ufw allow https
	
Open your http://itpdtd.com in a web browser to ensure nginx is running

Install the letsencrypt.org certbot

	add-apt-repository ppa:certbot/certbot
	apt install python-certbot-nginx -y

Get a TLS certificate from letsencrypt.org. Enter your email. Agree to the terms. Answer yes when it asks you to redirect all http traffic to https.

    certbot --nginx -d itpdtd.com

Now you're ready to configure Mosquitto to use TLS for MQTT and WebSockets. Create a new configuration file /etc/mosquitto/conf.d/default.conf. This does 3 things 1) disables anonymous access. 2) use the TLS certificate for MQTTS on port 8883 and 3) enables websockets over TLS on port 8083.

	# /etc/mosquitto/conf.d/default.conf

	# require passwords
	allow_anonymous false
	password_file /etc/mosquitto/passwd

	# only localhost can connect without encryption
	listener 1883 localhost

	# MQTTS - TLS connection on 8883
	listener 8883
	# adjust itpdtd.com to match your domain
	certfile /etc/letsencrypt/live/itpdtd.com/cert.pem
	cafile /etc/letsencrypt/live/itpdtd.com/chain.pem
	keyfile /etc/letsencrypt/live/itpdtd.com/privkey.pem

	# Secure Websocket connection on 8083
	listener 8083
	protocol websockets
	# point to the same SSL certifciates
	certfile /etc/letsencrypt/live/itpdtd.com/cert.pem
	cafile /etc/letsencrypt/live/itpdtd.com/chain.pem
	keyfile /etc/letsencrypt/live/itpdtd.com/privkey.pem

	# make sure this file ends with a trailing newline
		
add a user for mosquitto

	touch /etc/mosquitto/passwd
	mosquitto_passwd -b /etc/mosquitto/passwd samantha secret
	(repeat to add more users)

restart mosquitto

	service mosquitto stop
	service mosquitto start
	
Allow 8883 and 8083 through the firewall

    ufw allow 8883
    ufw allow 8083
