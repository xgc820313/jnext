#include "soci.h"
#include "soci-sqlite3.h"

using namespace SOCI;

const char* g_szDatabase	= "SQLite3"; // The only class supported by this plugin
BackEndFactory const &g_backEnd = sqlite3;

