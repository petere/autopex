# Autopex

Autopex is PostgreSQL extension installation magic.  It ties together [Pex](https://github.com/petere/pex) with event triggers to automatically download and build extensions that you want to install.

Install Autopex like a normal extension:

    $ make PG_CONFIG=/some/where/pg_config install
    $ psql dbname
    =# CREATE EXTENSION autopex;

Also install Pex and run `pex init` under the PostgreSQL user account (probably `postgres`).

Then you can create any extension, such as

    =# CREATE EXTENSION plsh;
    =# CREATE EXTENSION prefix;

and they will automatically be downloaded and built before the actual `CREATE EXTENSION` command runs.

PostgreSQL 9.3 or later is required.
