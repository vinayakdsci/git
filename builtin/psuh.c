// imports
#include "builtin.h"
#include "gettext.h" // O/P printing
#include "config.h"
#include "wt-status.h"
#include "pretty.h"
#include "commit.h"
#include "strbuf.h"

// the command's entry point
int cmd_psuh(int argc, const char **argv, const char *prefix) {

	const char *cfg_user_name;

	// pull the config
	git_config(git_default_config, NULL);
	// pull the value of user.name from git config file to the variable
	if(git_config_get_string_tmp("user.name", &cfg_user_name) > 0) // non-zero positive error code
		printf(_("No name found from the config\n"));
	else
		printf(_("git recognises you as %s\n"), cfg_user_name); // The underscore only precedes the string that has to be printed

	// just like config, inititalise a status config
	struct wt_status status;
	wt_status_prepare(the_repository, &status);
	// pull the config for the config
	git_config(git_default_config, &status);

	printf(_("You are on branch %s\n"), status.branch);

	// initialise a commit struct
	struct commit *c = NULL;
	// initialise a string buffer
	struct strbuf commitline = STRBUF_INIT;
	// populate the commits array
	c = lookup_commit_reference_by_name("origin/master");

	if(c != NULL) {
		pp_commit_easy(CMIT_FMT_ONELINE, c + 1, &commitline); // print the second most recent commit
		printf(_("Current commit: %s\n"), commitline.buf);
	}

	strbuf_release(&commitline);
	free(status.branch);

	return 0;
}
