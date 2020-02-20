-- Create and populate tables for people and devices
-- SQLite and PostgreSQL have different sytax for
-- auto incrementing primary keys

-- SQLite
CREATE TABLE person (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name VARCHAR(50) NOT NULL UNIQUE      
);

-- SQLite
CREATE TABLE device (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name VARCHAR(50) NOT NULL UNIQUE,
    person_id INTEGER NOT NULL,
    FOREIGN KEY (person_id) REFERENCES person(id)
);

-- tell sqlite to enforce foreing keys
PRAGMA foreign_keys=1;

-- -- PostgreSQL
-- CREATE TABLE person (
--     id SERIAL PRIMARY KEY,
--     name VARCHAR(50) NOT NULL UNIQUE
-- );
-- 
-- -- PostgreSQL
-- CREATE TABLE device (
--     id SERIAL PRIMARY KEY,
--     name VARCHAR(50) NOT NULL UNIQUE,
--     person_id INTEGER NOT NULL,
--     FOREIGN KEY (person_id) REFERENCES person(id)
-- );

-- Data
INSERT INTO person (id, name) VALUES (1, 'Don');
INSERT INTO device (name, person_id) VALUES ('device_00', 1);
INSERT INTO person (id, name) VALUES (2, 'Neta');
INSERT INTO device (name, person_id) VALUES ('device_10', 2);
INSERT INTO person (id, name) VALUES (3, 'KJ');
INSERT INTO device (name, person_id) VALUES ('device_11', 3);
INSERT INTO person (id, name) VALUES (4, 'Aditya');
INSERT INTO device (name, person_id) VALUES ('device_12', 4);
INSERT INTO person (id, name) VALUES (5, 'Nikhil');
INSERT INTO device (name, person_id) VALUES ('device_13', 5);
INSERT INTO person (id, name) VALUES (6, 'Yongkun');
INSERT INTO device (name, person_id) VALUES ('device_14', 6);
INSERT INTO person (id, name) VALUES (7, 'Martin');
INSERT INTO device (name, person_id) VALUES ('device_15', 7);
INSERT INTO person (id, name) VALUES (8, 'Erkin');
INSERT INTO device (name, person_id) VALUES ('device_16', 8);
INSERT INTO person (id, name) VALUES (9, 'Beste');
INSERT INTO device (name, person_id) VALUES ('device_17', 9);
INSERT INTO person (id, name) VALUES (10, 'Bilal');
INSERT INTO device (name, person_id) VALUES ('device_18', 10);
INSERT INTO person (id, name) VALUES (11, 'Jake');
INSERT INTO device (name, person_id) VALUES ('device_19', 11);
INSERT INTO person (id, name) VALUES (12, 'Yunze');
INSERT INTO device (name, person_id) VALUES ('device_20', 12);
INSERT INTO person (id, name) VALUES (13, 'Stacy');
INSERT INTO device (name, person_id) VALUES ('device_22', 13);
INSERT INTO person (id, name) VALUES (14, 'Helen');
INSERT INTO device (name, person_id) VALUES ('device_23', 14);
-- extra devices
INSERT INTO device (name, person_id) SELECT 'device_01', id FROM person WHERE name = 'Don';
INSERT INTO device (name, person_id) SELECT 'device_02', id FROM person WHERE name = 'Don';
INSERT INTO device (name, person_id) SELECT 'device_03', id FROM person WHERE name = 'Don';
INSERT INTO device (name, person_id) SELECT 'device_04', id FROM person WHERE name = 'Don';
INSERT INTO device (name, person_id) SELECT 'device_24', id FROM person WHERE name = 'Bilal';
