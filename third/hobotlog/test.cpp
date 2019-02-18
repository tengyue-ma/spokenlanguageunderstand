#include "logger.h"
#include "comm_def.h"

using namespace hobotlog;
using std::string;

LogModule * loginstance;

int hobot_print()
{
	string log_file = "log.conf";
	loginstance = new hobotlog::LogModule(log_file);
	loginstance->debug("---------start debug-------");
	loginstance->error("---------start error-------");
	loginstance->info("---------start info-------");
	loginstance->warn("---------start warn-------");
	return 0;
}
