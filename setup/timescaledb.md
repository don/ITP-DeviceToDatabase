# Timescale DB

Create new Digital Ocean droplet named timescale with Ubuntu **19.10**. Create DNS record for timescale.itpdtd.com.

Log into the machine, enable the firewall, update, reboot

    ssh root@timescale.itpdtd.com

    ufw allow ssh
    ufw enable

    apt update
    apt dist-upgrade -y
    shutdown -r now

Install PostgreSQL

    sudo apt install postgresql postgresql-contrib -y
    
Now that PostgreSQL is setup, we need to install Timescale using the instructions from https://docs.timescale.com/latest/getting-started/installation/ubuntu/installation-apt-ubuntu

Add Timescale PPA and install the extensions

    sudo add-apt-repository ppa:timescale/timescaledb-ppa
    sudo apt-get update
    sudo apt install timescaledb-postgresql-11

Configure the database

    sudo timescaledb-tune --yes
    
Restart Postgresql

    sudo service postgresql restart
    
Make sure you can login as user postgres

    sudo -i -u postgres
    psql
    
Quit psql with \q and type exit to get back to a root prompt
  
    \q
    exit
    
Configure postgres to listen on all addreses, not just localhost. Edit **/etc/postgresql/11/main/postgresql.conf** and add the following line

    listen_addresses = '*'

Configure posgres to allow users to login with passwords. Edit **/etc/postgresql/11/main/pg_hba.conf** and add

    hostssl  all all 0.0.0.0/0 md5
  
Restart postgres

    service postgresql restart
  
If postgres doesn't start, check the logs with 

    journalctl -u postgresql@10-main.service
  
Allow incoming postgres connections

    ufw allow 5432

Login as postgres user and create other users using psql or the command line tools like createuser

    sudo -i -u postgres
    psql
    CREATE DATABASE samantha;
    CREATE USER samantha with ENCRYPTED PASSWORD 'secret';
    GRANT ALL PRIVILEGES ON DATABASE samantha TO samanatha;
    ALTER USER samantha CREATEDB;
    \q
    exit
    