-- PostgreSQL function to get the owner of a device
-- 
-- Call the function directly
--     SELECT owner('device_11')
--
-- Or include in a query 
--     SELECT owner(device), device, count(*) FROM sensor_data GROUP BY device;
--
CREATE FUNCTION owner(device_name varchar) returns varchar AS $$
    SELECT p.name 
      FROM person p, device d 
      WHERE p.id = d.person_id 
        AND d.name = $1;
$$
LANGUAGE SQL;
