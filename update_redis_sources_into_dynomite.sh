#!/bin/bash

set -e
set -x

perl -pi -e 's|__DICT_H|__DYN_DICT_H|g;' src/dyn_dict.h

perl -pi -e 's@^.*include.*(dict|zmalloc|redisassert).h.*$@@g;' src/dyn_dict.c

perl -pi -e 's@^.*fmacros.h.*$@

/* map functions from redis to dynomite */
\#define zcalloc(a) dn_calloc(1, a)
\#define zmalloc(a) dn_zalloc(a)
\#define zfree(a) dn_free(a)

\#include <assert.h>
@' src/dyn_dict.c

