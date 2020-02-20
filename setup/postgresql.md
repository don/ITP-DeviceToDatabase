PostgreSQL is running on Amazon Web Services (AWS) Relational Database Service (RSD). Here's how the database was created for this class.

 * Log into AWS management console and go to the RDS dashboard.
 * Choose Databases from the menu on the left.
 * Press the Create Database button
 * Select Easy Create
 * Choose PostgreSQL
 * Choose Free Tier
 * Add an instance identifier, I used `pg`
 * Check "Auto generate a password"
 * Press the Create Database button

It will take a some time to create the database. The first time you click on the database details, there will be a button near the top of the screen to "View credential details". Click this button to see the password that was generated for the postgres user.

The database details will also give you an endpoint to connect to your database that will look something like pg.clhpc43azszs.us-east-2.rds.amazonaws.com. These names are difficult to type and remember. I created a DNS CNAME pg.itpdtd.com that points to pg.clhpc43azszs.us-east-2.rds.amazonaws.com for this class.

The default security restrictions on the database need to be changed to allow users to connect to the database.

 * From the database details page click the Modify button
 * Under Network & Security change Publicly Available to Yes
 * Enable Delete Protection
 * Click continue
 * Choose Apply Immediately
 * Click the orange Modify DB Instance button

You also need to modify the security group to allow connections
 * Click the inbound security group on the database details page
 * Choose the inbound tab on the security group detail page
 * Press the Edit button
 * Add a rule
    * Type: PostgreSQL
    * Source: Anywhere
 * Choose Save

Typically you'd want more restricted access to the database, but allowing connections from anywhere makes things simpler for this class.

