CREATE TABLE agenda (
    day         INT,
    month       INT,
    description VARCHAR,
    type        VARCHAR,
    id          INTEGER PRIMARY KEY AUTOINCREMENT,
    done        BOOLEAN DEFAULT (FALSE) 
);
