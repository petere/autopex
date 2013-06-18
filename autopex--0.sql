CREATE FUNCTION autopex() RETURNS event_trigger LANGUAGE C AS '$libdir/autopex';

CREATE EVENT TRIGGER autopex ON ddl_command_start WHEN tag in ('CREATE EXTENSION') EXECUTE PROCEDURE autopex();
