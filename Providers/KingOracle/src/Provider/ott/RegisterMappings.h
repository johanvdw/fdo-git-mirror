#ifndef REGISTERMAPPINGS_ORACLE
# define REGISTERMAPPINGS_ORACLE

#ifndef OCCI_ORACLE
# include <occi.h>
#endif

#ifndef SDO_GEOMETRY_ORACLE
# include "SDO_GEOMETRY.h"
#endif

void RegisterClasses(oracle::occi::Environment* envOCCI_);

#endif
