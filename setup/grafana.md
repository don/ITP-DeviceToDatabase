# Setting up a Grafana server

Create a new DigitalOcean virtual machine (or other VM.) I chose Ubuntu 19.10 x64 and the $5/month droplet size.

 * NY data center
 * Add a ssh key for logging in
 * hostname: grafana

I have DigitalOcean running DNS for the itpdtd.com domain, so I set up a new domain record for grafana.itpdtd.com pointing to the new VM.

Since I set up my SSH key when I created the machine, I can ssh into root@grafana.itpdtd.com

    ssh root@grafana.dev2db.com

Turn on the firewall and restrict incoming traffic to ssh, port 22

    ufw allow ssh
    ufw enable

Update the vm

    apt update
    apt dist-upgrade -y

Reboot, just in case the kernel was upgraded

    shutdown -r now

SSH back in

    ssh root@grafana.dev2db.com

Install the nginx 

    apt install nginx -y

Open the firewall for web traffic

    ufw allow http
    ufw allow https
	
Open your http://grafana.itpdtd.com in a web browser to ensure nginx is running

Install the letsencrypt.org certbot

    apt install python3-certbot-nginx -y

Get a TLS certificate from letsencrypt.org. Enter your email. Agree to the terms. Answer yes when it asks you to redirect all http traffic to https.

    certbot --nginx -d grafana.dev2db.com

Now that we have a TLS certificate, we can install Grafana based on https://grafana.com/grafana/download

    sudo apt-get install -y adduser libfontconfig1
    wget https://dl.grafana.com/oss/release/grafana_7.4.3_amd64.deb
    sudo dpkg -i grafana_7.4.3_amd64.deb

Start Grafana

    service grafana-server start
    
Grafana is running on port 3000. Next we configure Nginx to proxy traffic traffic to Grafana. This allows us to use Nginx for TLS. Exit `/etc/nginx/sites-enabled/default`

In the SSL server section, comment out (or delete) `location /`

        location / {
           # First attempt to serve request as file, then
           # as directory, then fall back to displaying a 404.
           try_files $uri $uri/ =404;
       }

Example of commenting out root location 

        #location / {
        #   # First attempt to serve request as file, then
        #   # as directory, then fall back to displaying a 404.
        #   try_files $uri $uri/ =404;
        #}

Add a new root location that redirects all traffic to Grafana on port 3000

        location / { 
            proxy_pass http://localhost:3000/;
        }

Note that it's often useful to redirect another url like /grafana to the Grafana server. This requires that you edit grafana.conf. This server is only running Grafana, so I'm redirecting all traffic to Grafana.

Restart nginx

    service ngnix restart
    
Open web browser. Log into https://grafana.itpdtd.com as user admin. The default password is admin. Grafana will prompt you to change the password when you login. Use the Granfa control panel to create configure the [database connections](https://grafana.itpdtd.com/datasources/new). You should create read only database users to use with the Grafana datasources. Create a [dashboard](https://grafana.itpdtd.com/dashboard/new), then add some [user accounts](https://grafana.itpdtd.com/org/users). 


