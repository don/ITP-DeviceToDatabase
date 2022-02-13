-- queries using person and device tables
-- these should work in both SQLite and PostgreSQL

-- device name and owner name
SELECT device.name AS device, person.name AS owner
	FROM device, person
	WHERE device.person_id = person.id
	ORDER BY device.name;

-- device name and owner name using table aliases
SELECT d.name AS device, p.name AS owner
	FROM device d, person p
	WHERE d.person_id = p.id
	ORDER BY d.name;
	
-- devices by owner	
SELECT p.name AS owner, d.name AS device
	FROM device d, person p
	WHERE d.person_id = p.id
	ORDER BY p.name;
	
-- which device(s) does Don own?
SELECT d.name 
	FROM device d, person p
	WHERE d.person_id = p.id
	AND p.name = 'Don';

-- who owns device_23
SELECT p.name 
	FROM device d, person p
	WHERE d.person_id = p.id
	AND d.name = 'device_23';
	
-- show the person with sensor data
SELECT p.name AS person, s.*
	FROM sensor_data s, person p, device d
	WHERE s.device = d.name
	AND d.person_id = p.id
	LIMIT 10;

-- query sensor data by person name		
SELECT p.name AS person, s.*
	FROM sensor_data s, person p, device d
	WHERE s.device = d.name
	AND d.person_id = p.id
	AND p.name = 'Lanni'
	LIMIT 10;

-- query sensor data by person name and measurement	
SELECT p.name AS person, s.*
	FROM sensor_data s, person p, device d
	WHERE s.device = d.name
	AND d.person_id = p.id
	AND p.name = 'Yilin'
	AND s.measurement = 'temperature'
	LIMIT 10;
	