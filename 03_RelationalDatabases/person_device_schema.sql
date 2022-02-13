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

-- tell sqlite to enforce foreign keys
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
INSERT INTO person (id, name) VALUES (1, 'Jingxi');
INSERT INTO device (name, person_id) VALUES ('device_10', 1);
INSERT INTO person (id, name) VALUES (2, 'Justin');
INSERT INTO device (name, person_id) VALUES ('device_11', 2);
INSERT INTO person (id, name) VALUES (3, 'Wasif');
INSERT INTO device (name, person_id) VALUES ('device_12', 3);
INSERT INTO person (id, name) VALUES (4, 'Name');
INSERT INTO device (name, person_id) VALUES ('device_14', 4);
INSERT INTO person (id, name) VALUES (5, 'Kyungah');
INSERT INTO device (name, person_id) VALUES ('device_15', 5);
INSERT INTO person (id, name) VALUES (6, 'Abby');
INSERT INTO device (name, person_id) VALUES ('device_16', 6);
INSERT INTO person (id, name) VALUES (7, 'Paulami');
INSERT INTO device (name, person_id) VALUES ('device_18', 7);
INSERT INTO person (id, name) VALUES (8, 'Yonatan');
INSERT INTO device (name, person_id) VALUES ('device_19', 8);
INSERT INTO person (id, name) VALUES (9, 'Tianjun');
INSERT INTO device (name, person_id) VALUES ('device_20', 9);
INSERT INTO person (id, name) VALUES (10, 'Alan');
INSERT INTO device (name, person_id) VALUES ('device_21', 10);
INSERT INTO person (id, name) VALUES (11, 'Youming');
INSERT INTO device (name, person_id) VALUES ('device_23', 11);
INSERT INTO person (id, name) VALUES (12, 'Lanni');
INSERT INTO device (name, person_id) VALUES ('device_24', 12);
INSERT INTO person (id, name) VALUES (13, 'Yilin');
INSERT INTO device (name, person_id) VALUES ('device_25', 13);
INSERT INTO person (id, name) VALUES (14, 'Don');
INSERT INTO device (name, person_id) VALUES ('device_00', 14);
-- extra devices
INSERT INTO device (name, person_id) SELECT 'machine', id FROM person WHERE name = 'Don';
INSERT INTO device (name, person_id) SELECT 'device_01', id FROM person WHERE name = 'Don';
INSERT INTO device (name, person_id) SELECT 'device_02', id FROM person WHERE name = 'Don';
INSERT INTO device (name, person_id) SELECT 'device_03', id FROM person WHERE name = 'Don';
INSERT INTO device (name, person_id) SELECT 'device_04', id FROM person WHERE name = 'Don';
INSERT INTO device (name, person_id) SELECT 'device_05', id FROM person WHERE name = 'Don';
INSERT INTO device (name, person_id) SELECT 'device_06', id FROM person WHERE name = 'Don';
INSERT INTO device (name, person_id) SELECT 'device_27', id FROM person WHERE name = 'Jingxi';

