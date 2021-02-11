-- CREATE DATABASE itp;
-- \c itp

CREATE TABLE mqtt_message (
    id SERIAL PRIMARY KEY,
    topic TEXT NOT NULL,
    payload TEXT,
    recorded_at TIMESTAMPTZ NOT NULL DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE sensor_data (
    id SERIAL PRIMARY KEY,
    device VARCHAR(50),      
    measurement VARCHAR(50),
    reading NUMERIC(6, 2),
    recorded_at TIMESTAMPTZ NOT NULL DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE sensor_data_text (
    id SERIAL PRIMARY KEY,
    device VARCHAR(50),      
    measurement VARCHAR(50),
    reading TEXT,
    recorded_at TIMESTAMPTZ NOT NULL DEFAULT CURRENT_TIMESTAMP
);
