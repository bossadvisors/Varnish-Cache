/*-
 * Copyright (c) 2006 Verdens Gang AS
 * Copyright (c) 2006-2014 Varnish Software AS
 * All rights reserved.
 *
 * Author: Poul-Henning Kamp <phk@phk.freebsd.dk>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * Director APIs
 *
 * A director ("VDI") is an abstract entity which can either satisfy a
 * backend fetch request or select another director for the job.
 *
 * In theory a director does not have to talk HTTP over TCP, it can satisfy
 * the backend request using any means it wants, although this is presently
 * not implemented.
 *
 */

/*--------------------------------------------------------------------
 * A director is a piece of code which selects one of possibly multiple
 * backends to use.
 */

typedef int vdi_getfd_f(const struct director *, struct busyobj *);
typedef unsigned vdi_healthy_f(const struct director *, const struct busyobj *,
    double *changed);
typedef const struct director *vdi_resolve_f(const struct director *,
    struct worker *, struct busyobj *);
typedef int vdi_gethdrs_f(const struct director *, struct worker *,
    struct busyobj *);
typedef int vdi_getbody_f(const struct director *, struct worker *,
    struct busyobj *);
typedef void vdi_finish_f(const struct director *, struct worker *,
    struct busyobj *);
typedef struct suckaddr *vdi_suckaddr_f(const struct director *,
    struct worker *, struct busyobj *);

struct director {
	unsigned		magic;
#define DIRECTOR_MAGIC		0x3336351d
	const char		*name;
	char			*vcl_name;
	vdi_getfd_f		*getfd;
	vdi_healthy_f		*healthy;
	vdi_resolve_f		*resolve;
	vdi_gethdrs_f		*gethdrs;
	vdi_getbody_f		*getbody;
	vdi_finish_f		*finish;
	vdi_suckaddr_f		*suckaddr;
	void			*priv;
};

/* cache_director.c */
int VDI_GetHdr(struct worker *wrk, struct busyobj *bo);
int VDI_GetBody(const struct director *d, struct worker *wrk,
    struct busyobj *bo);
void VDI_Finish(const struct director *d, struct worker *wrk,
    struct busyobj *bo);
int VDI_GetFd(const struct director *d, struct worker *wrk, struct busyobj *);
int VDI_Healthy(const struct director *, const struct busyobj *);
struct suckaddr *VDI_Suckaddr(const struct director *d, struct worker *wrk,
    struct busyobj *bo);
void VDI_AddHostHeader(struct http *to, const struct vbc *vbc);
void VBE_Init(void);

