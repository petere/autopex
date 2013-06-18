#include <postgres.h>
#include <commands/event_trigger.h>
#include <lib/stringinfo.h>
#include <miscadmin.h>

#include <stdlib.h>

PG_MODULE_MAGIC;

PG_FUNCTION_INFO_V1(autopex);
Datum autopex(PG_FUNCTION_ARGS);

Datum
autopex(PG_FUNCTION_ARGS)
{
	EventTriggerData *trigdata = (EventTriggerData *) fcinfo->context;

	if (IsA(trigdata->parsetree, CreateExtensionStmt))
	{
		CreateExtensionStmt *cestmt = (CreateExtensionStmt *) trigdata->parsetree;

		char *my_exec_path_copy = pstrdup(my_exec_path);
		char *p = strrchr(my_exec_path_copy, '/');
		*p = '\0';

		if (strchr(my_exec_path_copy, '\''))
			elog(ERROR, "invalid character in executable directory");
		if (strchr(cestmt->extname, '\''))
			elog(ERROR, "invalid character in extension name");

		StringInfoData buf;
		initStringInfo(&buf);
		appendStringInfo(&buf, "pex -g '%s' install --if-not-exists '%s'",
						 my_exec_path_copy,
						 cestmt->extname);
		elog(NOTICE, "autopex: downloading and building extension; this might take a moment");
		int rc = system(buf.data);
		if (rc == -1)
			elog(ERROR, "fork or waitpid failed");
		else if (rc == 127)
			elog(ERROR, "execution of shell failed");
		else if (WIFEXITED(rc) && WEXITSTATUS(rc))
			ereport(ERROR,
					(errmsg("pex install failed with exit status %d", WEXITSTATUS(rc)),
					 errdetail("Check the server log for details.")));
		else if (WIFSIGNALED(rc))
			elog(ERROR, "pex install was terminated by signal %d", WTERMSIG(rc));
	}

	return NULL;
}
