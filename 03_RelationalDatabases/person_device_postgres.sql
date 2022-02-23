-- Create and populate tables for people and devices

-- PostgreSQL Schema

CREATE TABLE person (
    id SERIAL PRIMARY KEY,
    name VARCHAR(50) NOT NULL UNIQUE
);

CREATE TABLE device (
    id SERIAL PRIMARY KEY,
    name VARCHAR(50) NOT NULL UNIQUE,
    person_id INTEGER NOT NULL,
    FOREIGN KEY (person_id) REFERENCES person(id)
);

GRANT SELECT ON TABLE person TO public;
GRANT SELECT ON TABLE device TO public;

-- Data
INSERT INTO person (id, name) VALUES (1, 'Ricardo');
INSERT INTO device (name, person_id) VALUES ('device_10', 1);
INSERT INTO person (id, name) VALUES (2, 'Nicholas');
INSERT INTO device (name, person_id) VALUES ('device_11', 2);
INSERT INTO person (id, name) VALUES (3, 'Chloe');
INSERT INTO device (name, person_id) VALUES ('device_12', 3);
INSERT INTO person (id, name) VALUES (4, 'Meijie');
INSERT INTO device (name, person_id) VALUES ('device_13', 4);
INSERT INTO person (id, name) VALUES (5, 'Andres');
INSERT INTO device (name, person_id) VALUES ('device_14', 5);
INSERT INTO person (id, name) VALUES (6, 'MK');
INSERT INTO device (name, person_id) VALUES ('device_15', 6);
INSERT INTO person (id, name) VALUES (7, 'Thomas');
INSERT INTO device (name, person_id) VALUES ('device_16', 7);
INSERT INTO person (id, name) VALUES (8, 'Nati');
INSERT INTO device (name, person_id) VALUES ('device_17', 8);
INSERT INTO person (id, name) VALUES (9, 'Gracy');
INSERT INTO device (name, person_id) VALUES ('device_18', 9);
INSERT INTO person (id, name) VALUES (10, 'Jiyun');
INSERT INTO device (name, person_id) VALUES ('device_19', 10);
INSERT INTO person (id, name) VALUES (11, 'Don');
INSERT INTO device (name, person_id) VALUES ('device_00', 11);
-- extra devices
INSERT INTO device (name, person_id) SELECT 'device_01', id FROM person WHERE name = 'Don';
INSERT INTO device (name, person_id) SELECT 'device_02', id FROM person WHERE name = 'Don';
INSERT INTO device (name, person_id) SELECT 'device_03', id FROM person WHERE name = 'Don';
INSERT INTO device (name, person_id) SELECT 'device_04', id FROM person WHERE name = 'Don';
INSERT INTO device (name, person_id) SELECT 'device_05', id FROM person WHERE name = 'Don';
INSERT INTO device (name, person_id) SELECT 'device_06', id FROM person WHERE name = 'Don';
INSERT INTO device (name, person_id) SELECT 'device_07', id FROM person WHERE name = 'Don';
INSERT INTO device (name, person_id) SELECT 'device_08', id FROM person WHERE name = 'Don';
INSERT INTO device (name, person_id) SELECT 'device_09', id FROM person WHERE name = 'Don';
