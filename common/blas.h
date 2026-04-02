#pragma once

#ifdef __APPLE__
    #ifdef ACCELERATE_NEW_LAPACK
        #include <vecLib/cblas_new.h>
    #else
        #include <vecLib/cblas.h>
    #endif
#else
    #include <cblas.h>
#endif
