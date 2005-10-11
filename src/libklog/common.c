/*
 * (c)opyright 2005 - KoanLogic S.r.l.
 */

#include "conf.h"
#include <time.h>
#include <u/libu.h>
#include <klone/klog.h>
#include <klone/klogprv.h>

/* each klog_*_open() will push its private data afterwards */
int klog_new (int type, int threshold, klog_t **pkl)
{
    klog_t *kl;

    dbg_return_if (pkl == NULL, ~0);

    kl = u_zalloc(sizeof(klog_t));
    dbg_err_if (kl == NULL);

    kl->threshold = threshold;

    /* check the supplied type */
    switch (type)
    {
        case KLOG_TYPE_MEM:
        case KLOG_TYPE_FILE:
        case KLOG_TYPE_SYSLOG:
            kl->type = type;
            break;
        default:
            warn_err("bad klog_t type !");
    }

    /* push out the klog_t object */
    *pkl = kl;
    
    return 0;
err:
    u_free(kl);
    return ~0;
}

const char *klog_to_str (int lev)
{
    return (lev < KLOG_DEBUG || lev > KLOG_EMERG) ? "?" : kloglev[lev];
}

