# Timescale DB

Timescale DB is installed in an [AWS EC2](https://aws.amazon.com/ec2/) instance. See the [instructions from 2020](https://github.com/don/ITP-DeviceToDatabase/blob/Spring-2020/setup/timescaledb.md) for more details about installing on Digital Ocean. The Timescale website has lots of documenation about [installing Timescale](https://docs.timescale.com/latest/getting-started/installation). I used the [AWS AMI instructions](https://docs.timescale.com/latest/getting-started/installation/ami/installation-ubuntu-ami). The AMI is an image of a machine that is configured with PostgreSQL and the TimescaleDB extension.

Create a new EC2 instance in the us-east region using ami-0e84add0567582adb. I'm starting with t3.small which is $0.0208/hr or about $15/month. I might need to upgrade to a t3.medium for $0.0416/hr or $30/month. Add 30GB of SSD. Create a new security group and allow SSH (22) access from my IP address and PostgreSQL (5432) access from anywhere.

I added a new DNS entry timescale.dev2db.com for the EC2 intance's IP address 18.188.17.70.

Log into the machine and update ubuntu

    ssh -i ~/.ssh/dev2db.pem ubuntu@timescale.dev2db.com

    sudo apt update
    sudo apt dist-upgrade -y
    sudo shutdown -r now

Time the database with Timescale's script

    sudo timescaledb-tune --yes
    
Restart Postgresql

    sudo service postgresql restart
    
Make sure you can login as user postgres

    sudo -i -u postgres
    psql
    
Quit psql with \q and type exit to get back to a ubuntu user prompt
  
    \q
    exit
    
Configure postgres to listen on all addreses, not just localhost. Edit **/etc/postgresql/12/main/postgresql.conf** and add the following line

    listen_addresses = '*'

Configure posgres to allow users to login with passwords. Edit **/etc/postgresql/12/main/pg_hba.conf** and add

    hostssl  all all 0.0.0.0/0 md5
  
Restart postgres

    service postgresql restart
  
If postgres doesn't start, check the logs with 

    journalctl -u postgresql@10-main.service
  
Login as postgres user and create other users using psql or the command line tools like createuser

    sudo -i -u postgres
    psql
    CREATE DATABASE samantha;
    CREATE USER samantha with ENCRYPTED PASSWORD 'secret';
    GRANT ALL PRIVILEGES ON DATABASE samantha TO samanatha;
    ALTER USER samantha CREATEDB;
    \q
    exit
    
## Creating the databases

I copied the ITP and Farm databases from pg.dev2db.com to the new timescale.dev2db.com

Use psql on the server as the postgres user to create my account

    ssh -i ~/.ssh/dev2db.pem ubuntu@timescale.dev2db.com 
    
    sudo -i -u postgres
    psql
    CREATE DATABASE don;
    CREATE USER don with ENCRYPTED PASSWORD [REDACTED];
    GRANT ALL PRIVILEGES ON DATABASE don TO don;
    ALTER USER don CREATEDB;

Now I can log into timescale.dev2db.com with psql from my laptop. Create 2 new databases tsitp and tsfarm. I added the ts prefix to the exiting database names so we don't confuse them the the postgres databases.

    psql -h timescale.dev2db.com -U don 

    CREATE DATABASE tsitp;
    CREATE DATABASE tsfarm;

Next, we need to enable the timescaledb extension in each of these databases. The extension must be enabled by super user. I use the postgres user.

    ssh -i ~/.ssh/dev2db.pem ubuntu@timescale.dev2db.com 
    
    sudo -i -u postgres
    psql

    \c tsitp
    CREATE EXTENSION IF NOT EXISTS timescaledb CASCADE;

    \c tsfarm
    CREATE EXTENSION IF NOT EXISTS timescaledb CASCADE;

### ITP Data

Now I can switch back my user to create tables. The tables look very similar to the PostgreSQL versions, except I've removed the id column. TimescaleDB uses the recorded_at time column instead.

    psql -h timescale.dev2db.com -U don 

    \c tsitp

    CREATE TABLE sensor_data (
        device VARCHAR(50),      
        measurement VARCHAR(50),
        reading DOUBLE PRECISION,
        recorded_at TIMESTAMPTZ NOT NULL DEFAULT CURRENT_TIMESTAMP
    );

    CREATE TABLE sensor_data_text (
        device VARCHAR(50),      
        measurement VARCHAR(50),
        reading TEXT,
        recorded_at TIMESTAMPTZ NOT NULL DEFAULT CURRENT_TIMESTAMP
    );

Convert the new tables to hypertables

    SELECT create_hypertable('sensor_data', 'recorded_at');
    SELECT create_hypertable('sensor_data_text', 'recorded_at');

Export the data from PostgreSQL 

    psql -h pg.dev2db.com -U don itp
    \COPY (select device, measurement, reading, recorded_at from sensor_data) TO '/tmp/itp.csv' DELIMITER ',' CSV HEADER;
    \COPY (select device, measurement, reading, recorded_at from sensor_data_text) TO '/tmp/itp_t.csv' DELIMITER ',' CSV HEADER;

Import the data into TimescaleDB

    psql -h timescale.dev2db.com -U don tsitp
    \COPY sensor_data(device, measurement, reading, recorded_at) FROM '/tmp/itp.csv' DELIMITER ',' CSV HEADER;
    \COPY sensor_data_text(device, measurement, reading, recorded_at) FROM '/tmp/itp_t.csv' DELIMITER ',' CSV HEADER;

After importing the data, I enable my script that moves data from MQTT into TimescaleDB. Once the script is running, I verify that new data is arriving into the sensor_data table.

    SELECT * FROM sensor_data WHERE recorded_at > now() - interval '20 seconds';

Create the person and device tables using the PostgreSQL code in [person_device_schema.sql](https://github.com/don/ITP-DeviceToDatabase/blob/main/03_RelationalDatabases/person_device_schema.sql). Create the owner function using [function_owner.sql](https://github.com/don/ITP-DeviceToDatabase/blob/main/03_RelationalDatabases/function_owner.sql).

As the postgres user, run the script to create all the users. Then as the database owner, grant any user permission to SELECT data from the tsitp tables.

    GRANT SELECT ON ALL TABLES IN SCHEMA public TO public;

### Farm Data

Connect to TimescaleDB and create the sensor_data table

    psql -h timescale.dev2db.com -U don

    \c tsfarm

    CREATE TABLE sensor_data (
        device VARCHAR(50),      
        measurement VARCHAR(50),
        reading DOUBLE PRECISION,
        recorded_at TIMESTAMPTZ NOT NULL DEFAULT CURRENT_TIMESTAMP
    );
`
Convert the new table to a TimescaleDB hypertable

    SELECT create_hypertable('sensor_data', 'recorded_at');

Export the data from PostgreSQL 

    psql -h pg.dev2db.com -U don farm
    \COPY (select device, measurement, reading, recorded_at from sensor_data) TO '/tmp/farm.csv' DELIMITER ',' CSV HEADER;
    exit

Import the data into TimescaleDB and grant select permissions

    psql -h timescale.dev2db.com -U don tsfarm
    \COPY sensor_data(device, measurement, reading, recorded_at) FROM '/tmp/farm.csv' DELIMITER ',' CSV HEADER;
    GRANT SELECT ON ALL TABLES IN SCHEMA public TO public;
