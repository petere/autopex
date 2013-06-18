PG_CONFIG = pg_config

MODULE_big = autopex
OBJS = autopex.o
DATA = autopex--0.sql
EXTENSION = autopex

PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
