# Setting up the InfluxDB database

Create a new DigitalOcean virtual machine (or other VM.) I chose Ubuntu 20.04 LTS x64 and the $5/month droplet size.

 * NY data center
 * Add a ssh key for logging in
 * hostname: influx

I have DigitalOcean running DNS for the itpdtd.com domain, so I set up a new domain record for influx.itpdtd.com pointing to the new VM.

Since I set up my SSH key when I created the machine, I can ssh into root@influx.itpdtd.com

    ssh root@influx.dev2db.com

Turn on the firewall and restrict incoming traffic to ssh, port 22

    ufw allow ssh
    ufw enable

Update the vm

    apt update
    apt dist-upgrade -y

Reboot, just in case the kernel was upgraded

    shutdown -r now

SSH back in

    ssh root@influx.dev2db.com

Install the compiler tools and nginx 

    apt install build-essential nginx -y

Open the firewall for web traffic

    ufw allow http
    ufw allow https
	
Open your http://influx.dev2db.com in a web browser to ensure nginx is running

Install the letsencrypt.org certbot

	apt install python3-certbot-nginx -y

Get a TLS certificate from letsencrypt.org. Enter your email. Agree to the terms. Answer yes when it asks you to redirect all http traffic to https.

    certbot --nginx -d influx.dev2db.com

Now that we have a TLS certificate, we can install InfluxDB

    wget https://dl.influxdata.com/influxdb/releases/influxdb_1.8.4_amd64.deb
    sudo dpkg -i influxdb_1.8.4_amd64.deb

Copy the keys into the the influxdb directory

    cd /etc/influxdb
    cp /etc/letsencrypt/live/influx.dev2db.com/fullchain.pem .
    cp /etc/letsencrypt/live/influx.dev2db.com/privkey.pem .

Change the owner of the private key to influxdb

    chown influxdb /etc/influxdb/privkey.pem

Edit /etc/influxdb and adjust the configuration in the appropriate sections

    auth-enabled = true	
    https-enabled = true
    https-certificate = "/etc/influxdb/fullchain.pem"
    https-private-key = "/etc/influxdb/privkey.pem"
  
Restart influx

    service influxdb restart

Open the firewall for influx

    ufw allow 8086

Login into influx and create a password

    influx -host influx.dev2db.com -ssl
    create user admin with password [REDACTED] with all privileges;
    exit

Confirm the username and password work

    influx -host influx.dev2db.com -ssl -username admin -password [REDACTED]
