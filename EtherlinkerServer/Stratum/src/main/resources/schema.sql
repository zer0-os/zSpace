CREATE TABLE IF NOT EXISTS user_accounts
(
  user_account_id uuid NOT NULL,
  login VARCHAR NOT NULL UNIQUE,
  password VARCHAR NOT NULL,
  encryption_keys bytea NOT NULL,
  wallet_data bytea NOT NULL,
  date_create timestamp with time zone NOT NULL,
  CONSTRAINT user_accounts_pk PRIMARY KEY (user_account_id)
);