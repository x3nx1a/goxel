#include "vec.h"
#include <float.h>

void mat3_normalize_(mat3_t *m)
{
    int i;
    for (i = 0; i < 3; i++)
        vec3_normalize(&m->vecs[i]);
}

static void mat3_to_eul2_(const mat3_t *m, vec3_t *e1, vec3_t *e2)
{
    real_t cy = hypot(m->v2[0][0], m->v2[0][1]);
    if (cy > 16.0f * FLT_EPSILON) {
        e1->x = atan2(m->v2[1][2], m->v2[2][2]);
        e1->y = atan2(-m->v2[0][2], cy);
        e1->z = atan2(m->v2[0][1], m->v2[0][0]);
        e2->x = atan2(-m->v2[1][2], -m->v2[2][2]);
        e2->y = atan2(-m->v2[0][2], -cy);
        e2->z = atan2(-m->v2[0][1], -m->v2[0][0]);
    } else {
        e1->x = atan2(-m->v2[2][1], m->v2[1][1]);
        e1->y = atan2(-m->v2[0][2], cy);
        e1->z = 0.0;
        *e2 = *e1;
    }
}

void mat3_to_eul_(const mat3_t *m, vec3_t *e)
{
    vec3_t e1, e2;
    mat3_t n = *m;
    mat3_normalize_(&n);

    mat3_to_eul2_(&n, &e1, &e2);

    // Pick best.
    if (    fabs(e1.x) + fabs(e1.y) + fabs(e1.y) >
            fabs(e2.x) + fabs(e2.y) + fabs(e2.z)) {
        *e = e1;
    } else {
        *e = e2;
    }
}

void quat_to_mat3_(const quat_t *q, mat3_t *m)
{
    real_t q0, q1, q2, q3, qda, qdb, qdc, qaa, qab, qac, qbb, qbc, qcc;

    q0 = M_SQRT2 * q->w;
    q1 = M_SQRT2 * q->x;
    q2 = M_SQRT2 * q->y;
    q3 = M_SQRT2 * q->z;

    qda = q0 * q1;
    qdb = q0 * q2;
    qdc = q0 * q3;
    qaa = q1 * q1;
    qab = q1 * q2;
    qac = q1 * q3;
    qbb = q2 * q2;
    qbc = q2 * q3;
    qcc = q3 * q3;

    m->v2[0][0] = (1.0 - qbb - qcc);
    m->v2[0][1] = (qdc + qab);
    m->v2[0][2] = (-qdb + qac);

    m->v2[1][0] = (-qdc + qab);
    m->v2[1][1] = (1.0 - qaa - qcc);
    m->v2[1][2] = (qda + qbc);

    m->v2[2][0] = (qdb + qac);
    m->v2[2][1] = (-qda + qbc);
    m->v2[2][2] = (1.0 - qaa - qbb);
}

void eul_to_quat_(const vec3_t *e, quat_t *q)
{
    real_t ti, tj, th, ci, cj, ch, si, sj, sh, cc, cs, sc, ss;
    ti = e->x * 0.5f;
    tj = e->y * 0.5f;
    th = e->z * 0.5f;
    ci = cos(ti);
    cj = cos(tj);
    ch = cos(th);
    si = sin(ti);
    sj = sin(tj);
    sh = sin(th);
    cc = ci * ch;
    cs = ci * sh;
    sc = si * ch;
    ss = si * sh;

    q->w = cj * cc + sj * ss;
    q->x = cj * sc - sj * cs;
    q->y = cj * ss + sj * cc;
    q->z = cj * cs - sj * sc;
}
