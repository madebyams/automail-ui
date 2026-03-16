CREATE TABLE IF NOT EXISTS credentials (
   id       INTEGER PRIMARY KEY,
   username TEXT    NOT NULL,
   password TEXT    NOT NULL
);

CREATE TABLE IF NOT EXISTS config (
  id             INTEGER PRIMARY KEY,
  hostname       TEXT    NOT NULL,
  port           INTEGER NOT NULL,
  ssl            INTEGER NOT NULL DEFAULT 1,
  mailbox        TEXT    NOT NULL DEFAULT 'INBOX',
  client_type    TEXT    NOT NULL DEFAULT 'IMAP',
  credential_id  INTEGER NOT NULL REFERENCES credentials(id)
);

CREATE TABLE IF NOT EXISTS sender_blocklist (
    id    INTEGER PRIMARY KEY,
    email TEXT    NOT NULL UNIQUE
);