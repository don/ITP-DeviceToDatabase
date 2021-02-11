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
	
-- which device(s) does KJ own?
SELECT d.name 
	FROM device d, person p
	WHERE d.person_id = p.id
	AND p.name = 'KJ';

-- who owns device_23
SELECT p.name 
	FROM device d, person p
	WHERE d.person_id = p.id
	AND d.name = 'device_23';
	
-- show person name with sensor data	
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
	AND p.name = 'Stacy'
	LIMIT 10;

-- query sensor data by person name and measurement	
SELECT p.name AS person, s.*
	FROM sensor_data s, person p, device d
	WHERE s.device = d.name
	AND d.person_id = p.id
	AND p.name = 'Martin'
	AND s.measurement = 'soil'
	LIMIT 10;
	